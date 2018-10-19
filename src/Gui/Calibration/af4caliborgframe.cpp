#include "af4caliborgframe.h"

AF4CalibOrgFrame::AF4CalibOrgFrame(QWeakPointer<QComboBox> channelSelection,
                                   QWeakPointer<QMap<QString, AF4ChannelDimsWidget *> > channelConfigWidgets,
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

   bool ok;
   uint numberOfChannels = settings.value("channels/number", 0).toInt(&ok);
   if(!ok) AF4Log::logWarning(tr("Could not read parameter channels/number from iniFile. Value will be set to 1"));

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
      allCalibSelections.insert(channelSelection.data()->itemText(i), calibSelection);

      channelName = channelSelection.data()->itemText(i);
      channelCalibWidgets->insert(channelName, QMap<QString, AF4ChannelCalibWidget*>());
      for(uint j=0; j < numberOfCalibrations; j++){
         calibName = settings.value(tr("channels/%1/calib/%2/name").arg(i).arg(j), "").toString();         
         currentCalibWidget = createNewCalilbWidget(i, j, channelName, calibName);
      }
      connect(calibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4CalibOrgFrame::switchCalibWidget);
   }

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
   calibSelection->show();
   currentCalibWidget->show();
   connect(this, &AF4CalibOrgFrame::saveButtonClicked, this, &AF4CalibOrgFrame::saveParameters);
}

AF4CalibOrgFrame::~AF4CalibOrgFrame()
{
   saveParameters();
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
      this->adaptCalibWidgetIds(key, i);
      ++i;
   }
}

void AF4CalibOrgFrame::adaptAllCalibWidgetNames()
{
   for(const QString &key : channelCalibWidgets->keys())
      adaptCalibWidgetNames(key);
}

void AF4CalibOrgFrame::renameConnectedChannel(const QString &oldChName, const QString &newChName)
{
   QComboBox *renamedCalibSelection = allCalibSelections.value(oldChName);
   allCalibSelections.remove(oldChName);
   allCalibSelections.insert(newChName, renamedCalibSelection);
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
      calibSelection->show();
   }
   if(currentCalibWidget && (!channelCalibWidgets.isNull())){
      QString calibWidgetKey = (channelCalibWidgets->value(channelName).keys()).at(0);
      switchCalibWidget(calibWidgetKey);
   }
}

void AF4CalibOrgFrame::addConnectedChannel(const QString &newChName)
{
   if(calibSelection)
      calibSelection->hide();
   calibSelection = new QComboBox(this);

   lay->addWidget(calibSelection, 0, 4);
   allCalibSelections.insert(newChName, calibSelection);
   channelCalibWidgets->insert(newChName,  QMap<QString, AF4ChannelCalibWidget*>());


   while(!addCalibration());
   calibSelection->setCurrentIndex(0);

   connect(calibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
           this, &AF4CalibOrgFrame::switchCalibWidget);
}

void AF4CalibOrgFrame::deleteConnectedChannel(const QString &rmChName, const QString newChToDisplayName)
{
   calibSelection = allCalibSelections.value(rmChName);
   calibSelection->show();
   QComboBox *rmComboXox = allCalibSelections.value(rmChName);
   allCalibSelections.remove(rmChName);
   delete rmComboXox;
   currentCalibWidget = channelCalibWidgets->value(newChToDisplayName).value(calibSelection->currentText());
   currentCalibWidget->show();
   for(AF4ChannelCalibWidget* rmWidget : channelCalibWidgets->value(rmChName)) delete rmWidget;
   channelCalibWidgets->remove(rmChName);
}

bool AF4CalibOrgFrame::addCalibration()
{
   QString newName;
   if(askCalibAdding(newName)){
      // add a new Calibration here:
      if(currentCalibWidget) currentCalibWidget->hide();
      // CalibrationParameters params;
      QString channelName = channelSelection.data()->currentText();
      int channelId = channelSelection.data()->currentIndex();
      int newCalibId = channelCalibWidgets->size();
      auto* newCalibration = createNewCalilbWidget(channelId, newCalibId, channelName, newName);
      if(currentCalibWidget) newCalibration->setAllCalibrationParameters(currentCalibWidget->getAllCalibrationParameters());
      calibSelection->setCurrentIndex(calibSelection->count()-1);
      adaptCalibWidgetIds(channelName, channelId);
      adaptCalibWidgetNames(channelName);
      AF4Log::logText(tr("New Calibration \"%1\" added.").arg(newName));
      switchCalibWidget(newName);
      currentCalibWidget->show();
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
      // remove the old calibration widget here and inset it under its new key:
      const QString channelName = channelSelection.data()->currentText();
      const int channelId = channelConfigWidgets.data()->value(channelName)->getChannelId();
      AF4ChannelCalibWidget *oldCalibWidget = (*channelCalibWidgets)[channelName][oldCalibName];
      (*channelCalibWidgets)[channelName].insert(newName, oldCalibWidget);
      (*channelCalibWidgets)[channelName].remove(oldCalibName);
      // rename the entry in the combo box
      calibSelection->setItemText(calibSelection->currentIndex(), newName);

      adaptCalibWidgetIds(channelName, channelId);
      adaptCalibWidgetNames(channelName);
      saveParameters();
   }
}

void AF4CalibOrgFrame::deleteCalibration()
{
   if(calibSelection->count() > 1){
      AF4DeleteCalibDialog* deleteCalibDialog = new AF4DeleteCalibDialog();
      if(deleteCalibDialog->exec()){
         const QString calibrationToRemove = calibSelection->currentText();
         const QString channelName = channelSelection.data()->currentText();
         int indexToRemove = calibSelection->currentIndex();
         calibSelection->removeItem(indexToRemove);
         (*channelCalibWidgets.data())[channelName].remove(calibrationToRemove);
         int channelId = channelConfigWidgets.data()->value(channelName)->getChannelId();
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

   QString channelName = channelSelection.data()->currentText();
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
   QList<QString> keys = allCalibSelections.keys();
   for(int i = 0; i < channelConfigWidgets->count(); ++i)
      settings.setValue(tr("channels/%1/numberOfCalibrations").arg(i), allCalibSelections.value(keys[i])->count());
}

AF4ChannelCalibWidget *AF4CalibOrgFrame::createNewCalilbWidget(const int channelId, const int calibId, const QString &channelName, const QString &calibName)
{
   auto* newWidget = new AF4ChannelCalibWidget(channelId, calibId, channelName, calibName, this);
   connect(newWidget, &AF4ChannelCalibWidget::calibrateChannelCalled,
           this, &AF4CalibOrgFrame::calibrateChannelCalled);
   connect(this,  &AF4CalibOrgFrame::saveButtonClicked,
           newWidget, &AF4ChannelCalibWidget::saveButtonClicked);
   calibSelection->addItem(calibName);
   (*channelCalibWidgets.data())[channelName].insert(calibName, newWidget);
   lay->addWidget(newWidget, 2, 0, 7, 7);
   newWidget->hide();
   return newWidget;
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
