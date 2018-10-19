#include "af4caliborgframe.h"

AF4CalibOrgFrame::AF4CalibOrgFrame(QSharedPointer<QComboBox> channelSelection,
                                   QSharedPointer<QMap<QString, AF4ChannelDimsWidget *> > channelConfigWidgets,
                                   QWidget *parent)
   : QFrame(parent), channelSelection(channelSelection) , channelConfigWidgets(channelConfigWidgets)
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   this->setFrameStyle(0x1011);

   lay = new QGridLayout(this);
   lay->addWidget(new QLabel("<b>Calibration</b>", this), 0, 0, Qt::AlignLeft);
   allCalibSelections.clear();
   channelCalibWidgets = QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > >(new QMap<QString, QMap<QString, AF4ChannelCalibWidget*> >);

   qDebug() << "WHY?";
   bool ok;
   uint numberOfChannels = settings.value("channels/number", 0).toInt(&ok);
   if(!ok) AF4Log::logWarning(tr("Could not read parameter channels/number from iniFile. Value will be set to 1"));
   //qDebug() << numberOfChannels;
   uint numberOfCalibrations = 0;
   QString calibName;
   QString channelName;

   for(uint i=0; i < numberOfChannels; ++i){
      numberOfCalibrations = settings.value(tr("channels/%1/numberOfCalibrations").arg((i)), 1).toInt(&ok);

      if(!ok) AF4Log::logWarning(tr("Could not read parameter channels/%1/numberOfCalibrations from iniFile. Value will be set to 1.").arg(i));
      calibSelection =  new QComboBox(this);

      calibSelection->setToolTip("Choose a Calibration profile");
      calibSelection->hide();
      lay->addWidget(calibSelection, 0, 11);
      allCalibSelections.insert(channelSelection->itemText(i), calibSelection);

      channelName = channelSelection->itemText(i);
      channelCalibWidgets->insert(channelName, QMap<QString, AF4ChannelCalibWidget*>());
      for(uint j=0; j < numberOfCalibrations; j++){
         //qDebug() << "10" << i << j;
         qDebug() << "b1"<< numberOfCalibrations;
         calibName = settings.value(tr("channels/%1/calib/%2/name").arg(i).arg(j), "").toString();
         //currentCalibWidget = new AF4ChannelCalibWidget(i, j, channelName, calibName, settingsWriter, this);
         currentCalibWidget = new AF4ChannelCalibWidget(i, j, channelName, calibName, this);
         qDebug() << "b2"<< numberOfCalibrations;
         qDebug() << "11" << i << j;
         connect(currentCalibWidget, &AF4ChannelCalibWidget::calibrateChannelCalled,
                 this, &AF4CalibOrgFrame::calibrateChannelCalled);
         connect(this,  &AF4CalibOrgFrame::saveButtonClicked,
                 currentCalibWidget, &AF4ChannelCalibWidget::saveButtonClicked);

              //qDebug() << "12" << i << j;
         calibSelection->addItem(calibName);
         (*channelCalibWidgets.data())[channelName].insert(calibName, currentCalibWidget);
         lay->addWidget(currentCalibWidget, 2, 0, 7, 7);
         currentCalibWidget->hide();
      }
      connect(calibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4CalibOrgFrame::switchCalibWidget);
   }
   #undef CHECK_SETTINGS_CONVERSION

   plotWidget = new AF4CalibPlotWidget(this);
   lay->addWidget(plotWidget, 2, 7, 7, 7);

   renameCalibButton = new QToolButton(this);
   renameCalibButton->setText("R");
   renameCalibButton->setToolTip("Rename the current calibration profile");

   connect(renameCalibButton, &QPushButton::clicked,
           this, &AF4CalibOrgFrame::renameCalibration);
   lay->addWidget(renameCalibButton, 0, 12);

   addCalibButton = new QToolButton(this);
   addCalibButton->setText(tr("+"));
   addCalibButton->setToolTip("Add new calibration profile");
   connect(addCalibButton, &QPushButton::clicked,
           this, &AF4CalibOrgFrame::addCalibration);
   lay->addWidget(addCalibButton, 0, 13);

   deleteCalibButton = new QToolButton(this);
   deleteCalibButton->setText(tr("-"));
   deleteCalibButton->setToolTip("delete current calibration profile");
   connect(deleteCalibButton, &QPushButton::clicked,
           this, &AF4CalibOrgFrame::deleteCalibration);
   lay->addWidget(deleteCalibButton, 0, 14);
   qDebug() << "a6.5";
   calibSelection->show();
   currentCalibWidget->show();
   qDebug() << "a7";
   connect(this, &AF4CalibOrgFrame::saveButtonClicked, this, &AF4CalibOrgFrame::saveParameters);
}

AF4CalibOrgFrame::~AF4CalibOrgFrame()
{
   //delete channelCalibWidgets;
   //writeSettings();
   qDebug() << "called";
   saveParameters();
   //delete allCalibSelections;

}

ParametersForCalibration AF4CalibOrgFrame::getParamsForCalibration() const
{
   return currentCalibWidget->getParametersForCalibration();
}

CalibModeSettings AF4CalibOrgFrame::getCalibModes() const
{
   return currentCalibWidget->getCalibModes();
}

void AF4CalibOrgFrame::adaptAllCalibWidgetIds()
{
   int i = 0;
   for(const QString &key : channelCalibWidgets->keys()){
      //AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
      //configWidget->setConfigId(i);
      //adaptCalibWidgetIds(key, i);
      this->adaptCalibWidgetIds(key, i);
      ++i;
   }
}

void AF4CalibOrgFrame::adaptAllCalibWidgetNames()
{
   for(const QString &key : channelCalibWidgets->keys()){
    //  AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
    //  configWidget->setChannelName(key);
      adaptCalibWidgetNames(key);
   }
}

void AF4CalibOrgFrame::renameConnectedChannel(const QString &oldChName, const QString &newChName)
{
   QComboBox *renamedCalibSelection = allCalibSelections.value(oldChName);
   allCalibSelections.remove(oldChName);
   allCalibSelections.insert(newChName, renamedCalibSelection);
   //QMap<QString, AF4ChannelCalibWidget*> renamedCalibWidgets = channelCalibWidgets->value(oldChName);
   channelCalibWidgets->remove(oldChName);
   channelCalibWidgets->insert(newChName, channelCalibWidgets->value(oldChName));
}


void AF4CalibOrgFrame::adaptPlotData()
{
   QString fileName = currentCalibWidget->getInputFileName();
   if(!fileName.isEmpty())
      plotWidget->setPlotData(fileName);
}

void AF4CalibOrgFrame::switchToFirstCalibWidget(const QString &channelName)
{
   if(calibSelection && !(allCalibSelections.isEmpty())){
      calibSelection->hide();
      calibSelection = allCalibSelections.value(channelName);
      //calibSelection->addWidget(currentCalibSelection, 0, 4);
      //currentCalibSelection->setCurrentIndex(currentCalibS);
      calibSelection->show();
   }

   if(currentCalibWidget && (!channelCalibWidgets.isNull())){
      //currentCalibWidget->hide();
      QString calibWidgetKey = (channelCalibWidgets->value(channelName).keys()).at(0);
      switchCalibWidget(calibWidgetKey);
      //currentCalibWidget = channelCalibWidgets->value(newWidgetKey)->value(calibWidgetKey);
      //calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
      //currentCalibWidget->show();
   }
}

void AF4CalibOrgFrame::addConnectedChannel(const QString &newChName)
{
   if(calibSelection)
      calibSelection->hide();
   calibSelection = new QComboBox(this);
   //channelConfigFrameLayout->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   //currentChConfigWidget->show();

   lay->addWidget(calibSelection, 0, 4);
   allCalibSelections.insert(newChName, calibSelection);
   channelCalibWidgets->insert(newChName,  QMap<QString, AF4ChannelCalibWidget*>());

   //channelSelection->addItem(newChName);
   //channelSelection->setCurrentIndex(channelSelection->count() - 1);

   while(!addCalibration());
   calibSelection->setCurrentIndex(0);

   connect(calibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
           this, &AF4CalibOrgFrame::switchCalibWidget);
}

void AF4CalibOrgFrame::deleteConnectedChannel(const QString &rmChName, const QString newChToDisplayName)
{
   calibSelection = allCalibSelections.value(rmChName);
   //lay->addWidget(calibSelection, 0, 4);
   calibSelection->show();
   QComboBox *rmComboXox = allCalibSelections.value(rmChName);
   allCalibSelections.remove(rmChName);
   delete rmComboXox;
   currentCalibWidget = channelCalibWidgets->value(newChToDisplayName).value(calibSelection->currentText());
   currentCalibWidget->show();
   //QMap<QString, AF4ChannelCalibWidget*> rmCalWidgets = channelCalibWidgets->value(rmChName);
   for(AF4ChannelCalibWidget* rmWidget : channelCalibWidgets->value(rmChName)) delete rmWidget;
   //channelCalibWidgets->value(rmChName).clear();
   channelCalibWidgets->remove(rmChName);
   //delete rmCalWidgets;
}

bool AF4CalibOrgFrame::addCalibration()
{
   qDebug() << "1";
   QString newName;
   if(askCalibAdding(newName)){
      // add a new Calibration here:
      if(currentCalibWidget) currentCalibWidget->hide();
      // CalibrationParameters params;
      QString channelName = channelSelection->currentText();
      int channelId = channelSelection->currentIndex();
      int newCalibId = channelCalibWidgets->size();
      AF4ChannelCalibWidget* newCalibration = new AF4ChannelCalibWidget(channelId,
                                                                        newCalibId,
                                                                        //channelSelection->count(),
                                                                        //channelCalibWidgets->value(channelSelection->currentText())->size(),
                                                                        //currentChConfigWidget->getChannelName(),
                                                                        //channelConfigWidgets->currentText(),
                                                                        channelName,
                                                                        newName,
                                                                        //this->settingsWriter,
                                                                        this);
      if(currentCalibWidget) newCalibration->setAllCalibrationParameters(currentCalibWidget->getAllCalibrationParameters());
      //connect(newCalibration, &AF4ChannelCalibWidget::calibrateChannelCalled, this, &AF4ChannelCalibsOrgFrame::calibrateChannnel);
      connect(newCalibration, &AF4ChannelCalibWidget::calibrateChannelCalled, this, &AF4CalibOrgFrame::calibrateChannelCalled);

      (*channelCalibWidgets.data())[channelName].insert(newName, newCalibration);
      calibSelection->addItem(newName);
      calibSelection->setCurrentIndex(calibSelection->count()-1);
      currentCalibWidget = newCalibration;
      // int channelId = channelConfigWidgets->value(channelName)->getChannelId();
      adaptCalibWidgetIds(channelName, channelId);
      adaptCalibWidgetNames(channelName);
      AF4Log::logText(tr("New Calibration \"%1\" added.").arg(newName));
      lay->addWidget(currentCalibWidget, 2, 0, 7, 7);
      switchCalibWidget(newName);
      //currentCalibWidget->show();
      saveParameters();
      return true;
   }
   else return false;
}

void AF4CalibOrgFrame::renameCalibration()
{
   QString newName;
   QString oldCalibName = QString(calibSelection->currentText());

   if(askCalibRenaming(newName, oldCalibName)){
      qDebug() << "rename10";
      // remove the old calibration widget here and inset it under its new key:
      const QString channelName = channelSelection->currentText();
      const int channelId = channelConfigWidgets->value(channelName)->getChannelId();
      qDebug() << "rename20";
      AF4ChannelCalibWidget *oldCalibWidget = (*channelCalibWidgets)[channelName][oldCalibName];
      qDebug() << "rename25" << oldCalibWidget->getChannelId();
      (*channelCalibWidgets)[channelName].insert(newName, oldCalibWidget);
      (*channelCalibWidgets)[channelName].remove(oldCalibName);
      // rename the entry in the combo box
      calibSelection->setItemText(calibSelection->currentIndex(), newName);

      qDebug() << "rename40";
      //
      adaptCalibWidgetIds(channelName, channelId);
      qDebug() << "rename45";
      //adaptCalibWidgetNames(channelName);
      qDebug() << "rename50";
      saveParameters();
   }
}

void AF4CalibOrgFrame::deleteCalibration()
{
   if(calibSelection->count() > 1){
      AF4DeleteCalibDialog* deleteCalibDialog = new AF4DeleteCalibDialog();
      if(deleteCalibDialog->exec()){
         const QString calibrationToRemove = calibSelection->currentText();
         const QString channelName = channelSelection->currentText();
         int indexToRemove = calibSelection->currentIndex();
         calibSelection->removeItem(indexToRemove);
         //(channelCalibWidgets->(calibrationToRemove));
         (*channelCalibWidgets.data())[channelName].remove(calibrationToRemove);
         int channelId = channelConfigWidgets->value(channelName)->getChannelId();
         adaptCalibWidgetIds(channelName, channelId);
         adaptCalibWidgetNames(channelName);

         AF4Log::logText(tr("Calibration deleted."));
         saveParameters();
      }
   } else {
      AF4Log::logWarning(tr("You cannot remove the last Calibration!"));
   }
}

void AF4CalibOrgFrame::switchCalibWidget(const QString &calibName)
{
   currentCalibWidget->hide();
   plotWidget->disconnectCurrentMarkers();

   QString channelName = channelSelection->currentText();
   currentCalibWidget = channelCalibWidgets->value(channelName).value(calibName);
   lay->addWidget(currentCalibWidget, 2, 0, 7, 7);
   connectCtrlWithPlotWidget();
   adaptPlotData();
   currentCalibWidget->show();
}

void AF4CalibOrgFrame::saveParameters() const
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   qDebug() << 1;
   QList<QString> keys = allCalibSelections.keys();
   for(int i = 0; i < channelConfigWidgets->count(); ++i)
      settings.setValue(tr("channels/%1/numberOfCalibrations").arg(i), allCalibSelections.value(keys.at(i))->count());
   qDebug() << 2;
}

void AF4CalibOrgFrame::adaptCalibWidgetIds(const QString &channelName,int newChannelId)
{
   int i = 0;
   for(const QString &key : (*channelCalibWidgets)[channelName].keys()){
      AF4ChannelCalibWidget *calibWidget = channelCalibWidgets->value(channelName).value(key);
      calibWidget->setCalibId(i);
      calibWidget->setChannelId(newChannelId);
      ++i;
   }
}

void AF4CalibOrgFrame::adaptCalibWidgetNames(const QString &channelName)
{
   int i = 0;
   for(const QString &key: (*channelCalibWidgets)[channelName].keys()){
      AF4ChannelCalibWidget *calibWidget = channelCalibWidgets->value(channelName).value(key);
      calibWidget->setCalibName(key);
      calibWidget->setChannelName(channelName);
      ++i;
   }
}

bool AF4CalibOrgFrame::askCalibRenaming(QString &newName, const QString &oldName)
{
   bool nameIsOk;
   bool firstDialog = true;
   qDebug() << "rename1";
   qDebug() << calibSelection->currentText();
   qDebug() << "rename2";
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4CalibNameDialog calibNameDialog(&newName, firstDialog, oldName, true);
      firstDialog = false;
      if(calibNameDialog.exec()){
         // check if the entered name is already used
         nameIsOk = true;
         for(int i=0; i < calibSelection->count() ; ++i){
            QString compName = calibSelection->itemText(i);
            if(!(QString::compare(newName, compName))){
               nameIsOk = false;
               break;
            }
         }
      } else return false;
   } while(!nameIsOk);
   return true;
}

bool AF4CalibOrgFrame::askCalibAdding(QString &newName)
{
   bool nameIsOk;
   bool firstDialog = true;
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4CalibNameDialog calibNameDialog(&newName, firstDialog);
      firstDialog = false;
      if(calibNameDialog.exec()){
         // check if the entered name is already used
         nameIsOk = true;
         for(int i=0; i < calibSelection->count() ; i++){
            QString compName = calibSelection->itemText(i);
            if(!(QString::compare(newName, compName))){
               nameIsOk = false;
               break;
            }
         }
      } else return false;
   } while(!nameIsOk);
   return true;
}

void AF4CalibOrgFrame::connectCtrlWithPlotWidget()
{
   auto ctrl =  currentCalibWidget->getCtrlBoxRefs();
   plotWidget->connectMarkers(ctrl.leftOffset, ctrl.tVoid, ctrl.tElution);
}
