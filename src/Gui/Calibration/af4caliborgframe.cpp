#include "af4caliborgframe.h"

//-/////////////////
//  Public stuff //
//-////////////////

AF4CalibOrgFrame::AF4CalibOrgFrame(QWeakPointer<QComboBox> channelSelection,
                                   QWeakPointer<QMap<QString, AF4ChannelDimsWidget *> > channelConfigWidgets,
                                   QWidget *parent)
   : QFrame(parent),
     channelSelection(channelSelection),
     channelConfigWidgets(channelConfigWidgets),
     plotWidget(new AF4CalibPlotWidget(this))
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   this->setFrameStyle(0x1011);

   lay = new QGridLayout(this);
   lay->addWidget(new QLabel("<b>Calibration</b>", this), 0, 0, Qt::AlignLeft);
   allCalibSelections.clear();
   channelCalibWidgets = QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > >(new QMap<QString, QMap<QString, AF4ChannelCalibWidget*> >);

   uint numberOfChannels = channelConfigWidgets.data()->count();  //= settings.value("channels/number", 0).toInt(&ok);
   bool ok;
   if(!ok) AF4Log::logWarning(tr("Could not read parameter channels/number from iniFile. Default Value will be set to 0"));

   uint numberOfCalibrations = 0;
   QString calibName;
   QString channelName;

   for(uint i=0; i < numberOfChannels; ++i){
      numberOfCalibrations = settings.value(tr("channels/%1/numberOfCalibrations").arg((i)), 0).toInt(&ok);
      if(!ok) AF4Log::logWarning(tr("Could not read parameter channels/%1/numberOfCalibrations from iniFile. Generate new calibration").arg(i));
      calibSelection =  new QComboBox(this);
      calibSelection->clear();
      allCalibSelections.insert(channelSelection.data()->itemText(i), calibSelection);
      calibSelection->setInsertPolicy(QComboBox::NoInsert);
      calibSelection->setToolTip("Choose a Calibration profile");
      calibSelection->hide();
      channelName = channelSelection.data()->itemText(i);
      channelCalibWidgets->insert(channelName, QMap<QString, AF4ChannelCalibWidget*>());
      lay->addWidget(calibSelection, 0, 11);
      if(numberOfCalibrations == 0){
         if(addCalibration()) ++numberOfCalibrations;
         else                 AF4Log::logWarning(tr("No calibration widget was added. Program might now work!"), true);
      }
      else {
         for(uint j=0; j < numberOfCalibrations; j++){
            calibName = settings.value(tr("channels/%1/calib/%2/name").arg(i).arg(j), "").toString();
            curCalibWidget = createNewCalilbWidget(i, j, channelName, calibName);
         }
      }
      connect(calibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4CalibOrgFrame::switchCalibWidget);
   }

   lay->addWidget(plotWidget, 2, 7, 7, 7);

   auto makeButton = [this](QToolButton *&button, QString text, QString toolTip, int row, int column){
      button = new QToolButton(this);
      button->setText(text);
      button->setToolTip(toolTip);
      lay->addWidget(button, row, column);
   };

   makeButton(renameCalibButton, "R", "Rename the current calibration profile", 0, 12);
   connect(renameCalibButton, &QPushButton::clicked, this, &AF4CalibOrgFrame::renameCalibration);
   makeButton(addCalibButton,    "+", "Add new calibration profile",            0, 13);
   connect(addCalibButton,    &QPushButton::clicked, this, &AF4CalibOrgFrame::addCalibration);
   makeButton(deleteCalibButton, "-", "Delete current calibration profile",     0, 14);
   connect(deleteCalibButton, &QPushButton::clicked, this, &AF4CalibOrgFrame::deleteCalibration);

   // set respective combobox to
   QString currentChannel = channelSelection.data()->currentText();
   calibSelection = allCalibSelections.value(currentChannel);
   curCalibWidget = channelCalibWidgets.data()->value(currentChannel)[calibSelection->currentText()];
   calibSelection->show();
   curCalibWidget->show();

   connect(this, &AF4CalibOrgFrame::saveButtonClicked, this, &AF4CalibOrgFrame::saveParameters);

   connectCtrlWithPlotWidget();

}

AF4CalibOrgFrame::~AF4CalibOrgFrame()
{
   saveParameters();
}

ParametersForCalibration AF4CalibOrgFrame::getParamsForCalibration() const
{
   return curCalibWidget->getParametersForCalibration();
}

CalibModeSettings AF4CalibOrgFrame::getCalibModes() const
{
   return curCalibWidget->getCalibModes();
}


//-////////////////
//  Public slots //
//-////////////////



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

void AF4CalibOrgFrame::renameConnectedChannel(const QString oldChName, const QString newChName)
{
   // replace the entry in the map with comboboxes
   QComboBox *renamedCalibSelection = allCalibSelections.value(oldChName);
   allCalibSelections.remove(oldChName);
   allCalibSelections.insert(newChName, renamedCalibSelection);
   // replace the entry in the map with AF4ChannelCalibWidget maps
   QMap<QString, AF4ChannelCalibWidget*> map = channelCalibWidgets->value(oldChName);
   channelCalibWidgets->remove(oldChName);
   channelCalibWidgets->insert(newChName, map);
   //adapt information within the respective
   adaptAllCalibWidgetIds();
   adaptAllCalibWidgetNames();
}



void AF4CalibOrgFrame::adaptPlotData()
{
   QString fileName = curCalibWidget->getInputFileName();
   if(!fileName.isEmpty())
      plotWidget->setPlotData(fileName);
}

void AF4CalibOrgFrame::switchToFirstCalibWidget(const QString channelName)
{
   if(calibSelection && !(allCalibSelections.isEmpty())){
      calibSelection->hide();
      calibSelection = allCalibSelections.value(channelName);
      calibSelection->show();
   }
   if(curCalibWidget && (!channelCalibWidgets.isNull())){
      QString calibWidgetKey = (channelCalibWidgets->value(channelName).keys()).at(0);
      switchCalibWidget(calibWidgetKey);
   }
}


void AF4CalibOrgFrame::addConnectedChannel(const QString newChName)
{
   if(calibSelection) calibSelection->hide();
   calibSelection = new QComboBox(this);
   allCalibSelections.insert(newChName, calibSelection);
   lay->addWidget(calibSelection, 0, 11);
   channelCalibWidgets->insert(newChName,  QMap<QString, AF4ChannelCalibWidget*>());
   while(!addCalibration());
   calibSelection->setCurrentIndex(0);
   connect(calibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
           this, &AF4CalibOrgFrame::switchCalibWidget);
}

void AF4CalibOrgFrame::deleteConnectedChannel(const QString channelToRemove, const QString channelToDisplay)
{
   // show new ComboBox
   calibSelection = allCalibSelections.value(channelToDisplay);
   calibSelection->show();
   // show new widgets
   calibSelection->setCurrentIndex(0);
   curCalibWidget = channelCalibWidgets->value(channelToDisplay).value(calibSelection->currentText());
   curCalibWidget->show();
   //remove old combobox
   QComboBox *rmComboXox = allCalibSelections.value(channelToRemove);
   delete rmComboXox;
   rmComboXox = nullptr;
   allCalibSelections.remove(channelToRemove);
   // remove old calibWidgets
   for(AF4ChannelCalibWidget* rmWidget : channelCalibWidgets->value(channelToRemove)){
      delete rmWidget;
      rmWidget = nullptr;
   }
   channelCalibWidgets->remove(channelToRemove);
   adaptAllCalibWidgetIds();
   adaptAllCalibWidgetNames();
}

//-/////////////////
//  Private slots //
//-/////////////////


bool AF4CalibOrgFrame::addCalibration()
{
   QString newName;
   QString channelName = channelSelection.data()->currentText();
   if(askCalibAdding(channelName, newName)){
      // add a new Calibration here:
      if(curCalibWidget) curCalibWidget->hide();
      // CalibrationParameters params;
      int channelId = channelSelection.data()->currentIndex();
      int newCalibId = channelCalibWidgets->size();
      auto* newCalibration = createNewCalilbWidget(channelId, newCalibId, channelName, newName);
      if(curCalibWidget) newCalibration->setAllCalibrationParameters(curCalibWidget->getAllCalibrationParameters());
      curCalibWidget = newCalibration;
      calibSelection->setCurrentIndex(calibSelection->count()-1);
      adaptCalibWidgetIds(channelName, channelId);
      adaptCalibWidgetNames(channelName);
      AF4Log::logText(tr("New Calibration \"%1\" added.").arg(newName));
      switchCalibWidget(newName);
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

void AF4CalibOrgFrame::switchCalibWidget(const QString calibName)
{
   if(curCalibWidget){
      curCalibWidget->hide();
      if(plotWidget) plotWidget->disconnectCurrentMarkers();
      QString channelName = channelSelection.data()->currentText();
      curCalibWidget = channelCalibWidgets->value(channelName).value(calibName);
      lay->addWidget(curCalibWidget, 2, 0, 7, 7);
      connectCtrlWithPlotWidget();
      adaptPlotData();
      curCalibWidget->show();
   }
}

void AF4CalibOrgFrame::saveParameters() const
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   QList<QString> keys = allCalibSelections.keys();
   for(int i = 0; i < channelConfigWidgets->count(); ++i){
      int countSelections = allCalibSelections.value(keys[i])->count();
      settings.setValue(tr("channels/%1/numberOfCalibrations").arg(i), countSelections);
   }
}


//-/////////////////
//  Private stuff //
//-/////////////////



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
   bool firstDialog = true;
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4CalibNameDialog calibNameDialog(&newName, firstDialog, oldName, true);
      firstDialog = false;
      if(calibNameDialog.exec()){
         // check if the entered name is already used       
      } else return false;
   } while(!calibNameDuplicated(newName));
   return true;
}

bool AF4CalibOrgFrame::askCalibAdding(const QString &channelName, QString &newName)
{
   bool firstDialog = true;
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4CalibNameDialog calibNameDialog(&newName, firstDialog);
      firstDialog = false;
      if(calibNameDialog.exec()){
      } else return false;
   } while(!calibNameDuplicated(newName));
   return true;
}

bool AF4CalibOrgFrame::calibNameDuplicated(const QString &newName)
{
   bool nameIsOk = true;
   if(calibSelection)
      for(int i=0; i < calibSelection->count() ; i++){
         QString compName = calibSelection->itemText(i);
         if(!(QString::compare(newName, compName))){
            nameIsOk = false;
         break;
         }
      }
   else
      AF4Log::logWarning(tr("New name was not checked for duplication."),true);
   return nameIsOk;
}

void AF4CalibOrgFrame::connectCtrlWithPlotWidget()
{
   auto ctrl =  curCalibWidget->getCtrlBoxRefs();
   plotWidget->connectMarkers(ctrl.leftOffset, ctrl.tVoid, ctrl.tElution);
}
