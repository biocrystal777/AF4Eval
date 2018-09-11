#include "af4channelconfigurationwidget.h"

#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };

AF4ChannelConfigurationWidget::AF4ChannelConfigurationWidget(QWidget *parent) :
   QWidget(parent)
{

   layout = new QGridLayout(this);
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok;

   ////////////////////////////
   // Basical initialization //
   ////////////////////////////

   channelConfigFrame = new QFrame(this);
   channelConfigFrame->setFrameStyle(0x1011);
   channelConfigFrameLayout = new QGridLayout(channelConfigFrame);
   channelConfigFrameLayout->addWidget(new QLabel("<b>Channel Configurations</b>", channelConfigFrame), 0, 0, Qt::AlignLeft);

   calibrationFrame = new QFrame(this);
   calibrationFrame->setFrameStyle(0x1011);
   calibrationFrameLayout = new QGridLayout(calibrationFrame);
   calibrationFrameLayout->addWidget(new QLabel("<b>Calibration</b>", calibrationFrame), 0, 0, Qt::AlignLeft);
   allCalibSelections = new QMap<QString, QComboBox*>();
   channelCalibWidgets = new QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>();

   ///////////////////////////////////
   //Constant channel configuration //
   ///////////////////////////////////

   channelSelection = new QComboBox(channelConfigFrame);
   channelSelection->setToolTip("");
   channelConfigFrameLayout->addWidget(channelSelection, 0, 4);
   renameChButton = new QToolButton(channelConfigFrame);
   renameChButton->setText("R");
   renameChButton->setToolTip("Rename the current channel");
   QObject::connect(renameChButton, SIGNAL(clicked()), this, SLOT(renameChannel()));
   channelConfigFrameLayout->addWidget(renameChButton, 0, 5);
   addChButton = new QToolButton(channelConfigFrame);
   addChButton->setText(tr("+"));
   addChButton->setToolTip("Add new Channel");
   QObject::connect(addChButton, SIGNAL(clicked()), this, SLOT(addChannel()));
   channelConfigFrameLayout->addWidget(addChButton, 0, 6);
   deleteChButton = new QToolButton(channelConfigFrame);
   deleteChButton->setText(tr("-"));
   deleteChButton->setToolTip("delete current Channel");
   QObject::connect(deleteChButton, SIGNAL(clicked()), this, SLOT(deleteChannel()));
   channelConfigFrameLayout->addWidget(deleteChButton, 0, 7);

   ////////////////////////////////////////////////////
   // initialize channels with values from QSettings //
   ////////////////////////////////////////////////////

   channelConfigWidgets = new QMap<QString, AF4ChannelDimsWidget*>();

   // make all channel widgets, add them to the QMap
   // of ChannelWidgets and insert them
   // into the ComboBox


   uint numberOfChannels = settings.value("channels/number", 0).toInt(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "1");

   for(uint i = 0; i < numberOfChannels; i++){
      // double channelValue;
      QString newChannelName = settings.value(tr("channels/%1/name").arg(i), tr("New Channel %1").arg(i)).toString();
      currentChConfigWidget = new AF4ChannelDimsWidget(i, newChannelName, channelConfigFrame);
      channelConfigWidgets->insert(newChannelName, currentChConfigWidget);
      channelSelection->addItem(newChannelName);
      channelSelection->setCurrentIndex(channelSelection->count()-1);
      QObject::connect(channelSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchChannelWidget(QString)));
      currentChConfigWidget->hide();
   }
   if(numberOfChannels == 0)
      while (!addChannel());

   channelConfigFrameLayout->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   currentChConfigWidget->show();

   ///////////////////////
   // Calibration Frame //
   ///////////////////////

   uint numberOfCalibrations = 0;
   QString calibName;
   QString channelName;
   for(uint i=0; i < numberOfChannels; i++){
      numberOfCalibrations = settings.value(tr("channels/%1/numberOfCalibrations").arg((i)), 0).toInt(&ok);
      CHECK_SETTINGS_CONVERSION(numberOfCalibrations, 0);
      currentCalibSelection =  new QComboBox(calibrationFrame);

      currentCalibSelection->setToolTip("Choose a Calibration profile");
      currentCalibSelection->hide();
      calibrationFrameLayout->addWidget(currentCalibSelection, 0, 4);
      allCalibSelections->insert(channelSelection->itemText(i), currentCalibSelection);

      channelName = channelSelection->itemText(i);
      channelCalibWidgets->insert(channelName, new QMap<QString, AF4ChannelCalibWidget*>);

      for(uint j=0; j < numberOfCalibrations; j++){
         calibName = settings.value(tr("channels/%1/calib/%2/name").arg(i).arg(j), "").toString();
         currentCalibWidget = new AF4ChannelCalibWidget(i, j, channelName, calibName, calibrationFrame);
         QObject::connect(currentCalibWidget, SIGNAL(calibrateChannelCalled()), this, SLOT(calibrateChannnel()));
         currentCalibSelection->addItem(calibName);
         channelCalibWidgets->value(channelName)->insert(calibName, currentCalibWidget);
         calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
         currentCalibWidget->hide();
      }
      QObject::connect(currentCalibSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchCalibWidget(QString)));
   }   
   renameCalibButton = new QToolButton(calibrationFrame);
   renameCalibButton->setText("R");
   renameCalibButton->setToolTip("Rename the current calibration profile");
   QObject::connect(renameCalibButton, SIGNAL(clicked()), this, SLOT(renameCalibration()));
   calibrationFrameLayout->addWidget(renameCalibButton, 0, 5);

   addCalibButton = new QToolButton(calibrationFrame);
   addCalibButton->setText(tr("+"));
   addCalibButton->setToolTip("Add new calibration profile");
   QObject::connect(addCalibButton, SIGNAL(clicked()), this, SLOT(addCalibration()));
   calibrationFrameLayout->addWidget(addCalibButton, 0, 6);

   deleteCalibButton = new QToolButton(calibrationFrame);
   deleteCalibButton->setText(tr("-"));
   deleteCalibButton->setToolTip("delete current calibration profile");
   QObject::connect(deleteCalibButton, SIGNAL(clicked()), this, SLOT(deleteCalibration()));
   calibrationFrameLayout->addWidget(deleteCalibButton, 0, 7);
   //////////////////////////
   // set starting widgets //
   //////////////////////////

   channelSelection->setCurrentIndex(0);
   channelName = channelSelection->currentText();
   currentChConfigWidget = channelConfigWidgets->value(channelName);
   currentChConfigWidget->show();
   currentCalibSelection->show();
   currentCalibSelection->setCurrentIndex(0);
   currentCalibWidget = channelCalibWidgets->value(channelName)->value(currentCalibSelection->currentText());
   currentCalibWidget->show();

   //////////////////////////////
   // add Frames to the layout //
   //////////////////////////////

   layout->addWidget(channelConfigFrame, 0, 0, 2, 10);
   layout->addWidget(calibrationFrame, 3, 0, 12, 10);

   settingsWriter = new QPushButton("Save Parameters", this);
   QObject::connect(settingsWriter, SIGNAL(clicked()), this, SLOT(saveParameters()));
   layout->addWidget(settingsWriter, 15, 0);

}

AF4ChannelConfigurationWidget::~AF4ChannelConfigurationWidget()
{
   writeSettings();
   delete channelConfigWidgets;
   delete channelCalibWidgets;
   delete allCalibSelections;
}

QMap<QString, AF4ChannelDimsWidget *> *AF4ChannelConfigurationWidget::getChannelConfigWidgets() const
{
   return channelConfigWidgets;
}

QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* AF4ChannelConfigurationWidget::getChannelCalibWidgets() const
{
   return channelCalibWidgets;
}

void AF4ChannelConfigurationWidget::adaptConfigWidgetIds()
{
   int i = 0;
   for(const QString &key : channelCalibWidgets->keys()){
      AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
      configWidget->setConfigId(i);
      adaptCalibWidgetIds(key, i, "adaptConfigWidgetIds");
      ++i;
   }
}

void AF4ChannelConfigurationWidget::adaptConfigWidgetNames()
{
   for(const QString &key : channelCalibWidgets->keys()){
      AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
      configWidget->setChannelName(key);
      adaptCalibWidgetNames(key);
   }
}

bool AF4ChannelConfigurationWidget::askChannelRenaming(QString &newName, const QString &oldName)
{
   bool nameIsOk;
   bool firstDialog = true;
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4ChannelNameDialog chNameDialog(&newName, firstDialog, oldName, true);
      firstDialog = false;
      if(chNameDialog.exec()){
         // check if the entered name is already used
         nameIsOk = true;
         for (int i=0; i < channelSelection->count() ; i++){
            QString compName = channelSelection->itemText(i);
            if(!(QString::compare(newName, compName))){
               nameIsOk = false;
               break;
            }
         }
      } else return false;
   } while(!nameIsOk);
   return true;
}

void AF4ChannelConfigurationWidget::renameChannel()
{
   QString newName;
   QString oldName = QString(channelSelection->currentText());
   if(askChannelRenaming(newName, oldName)){
      // add a new Channel here:
      AF4ChannelDimsWidget *oldConfWidget = channelConfigWidgets->value(oldName, nullptr);
      channelConfigWidgets->remove(oldName);
      channelConfigWidgets->insert(newName, oldConfWidget);
      channelSelection->setItemText(channelSelection->currentIndex(), newName);
      oldConfWidget->setChannelName(newName);
      // remove and reinsert the calibSelection and the FFFChannelselection
      // under the new key;
      QComboBox *renamedCalibSelection = allCalibSelections->value(oldName);
      allCalibSelections->remove(oldName);
      allCalibSelections->insert(newName, renamedCalibSelection);
      QMap<QString, AF4ChannelCalibWidget*>* renamedCalibWidgets = channelCalibWidgets->value(oldName);
      channelCalibWidgets->remove(oldName);
      channelCalibWidgets->insert(newName, renamedCalibWidgets);
      saveParameters();
   }
}

bool AF4ChannelConfigurationWidget::askChannelAdding(QString &newName){
   bool nameIsOk;
   bool firstDialog = true;
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4ChannelNameDialog chNameDialog(&newName, firstDialog);
      firstDialog = false;
      if(chNameDialog.exec()){
         // check if the entered name is already used
         nameIsOk = true;
         for (int i=0; i < channelSelection->count(); ++i){
            QString compName = channelSelection->itemText(i);
            if(!(QString::compare(newName, compName))){
               nameIsOk = false;
               break;
            }
         }
      } else return false;
   } while(!nameIsOk);
   return true;
}

bool AF4ChannelConfigurationWidget::addChannel()
{
   QString newName;
   if(askChannelAdding(newName)){
      // add a new Channel here:
      AF4ChannelDimsWidget* newChannel = new AF4ChannelDimsWidget(channelConfigWidgets->size(), newName, channelConfigFrame);
      channelConfigWidgets->insert(newName, newChannel);
      if(currentChConfigWidget) currentChConfigWidget->hide();
      currentChConfigWidget = newChannel;

      // add new assigned channelCalibWidget, Comboboxes etc.

      QObject::disconnect(channelSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchChannelWidget(QString)));
      if(currentCalibSelection)
         currentCalibSelection->hide();
      currentCalibSelection = new QComboBox(calibrationFrame);
      channelConfigFrameLayout->addWidget(currentChConfigWidget, 2, 0, 7, 7);
      currentChConfigWidget->show();

      calibrationFrameLayout->addWidget(currentCalibSelection, 0, 4);
      allCalibSelections->insert(newName, currentCalibSelection);

      channelCalibWidgets->insert(newName, new  QMap<QString, AF4ChannelCalibWidget*>());

      channelSelection->addItem(newName);
      channelSelection->setCurrentIndex(channelSelection->count() - 1);

      while(!addCalibration());
      currentCalibSelection->setCurrentIndex(0);

      QObject::connect(currentCalibSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchCalibWidget(QString)));
      QObject::connect(channelSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchChannelWidget(QString)));
      AF4Log::logText(tr("New Channel \"%1\" added.").arg(newName));



      adaptConfigWidgetIds();
      // set final current widgets;

      currentCalibSelection->show();

      currentCalibWidget = channelCalibWidgets->value(newName)->value(currentCalibSelection->currentText());
      currentCalibWidget->show();
      saveParameters();

      return true;
   }
   else return false;
}


void AF4ChannelConfigurationWidget::deleteChannel()
{
   if(channelSelection->count() > 1){
      AF4DeleteChannelDialog deleteChDialog;
      if(deleteChDialog.exec()){
         const QString channelToRemove = channelSelection->currentText();
         int indexToRemove = channelSelection->currentIndex();
         channelSelection->removeItem(indexToRemove);
         delete (channelConfigWidgets->value(channelToRemove));
         channelConfigWidgets->remove(channelToRemove);
         AF4Log::logText(tr("Channel deleted."));

         QString newChannelName = channelSelection->currentText();
         // Delete now all assigned Calibration things:
         QComboBox *selectionToRemove = allCalibSelections->value(channelToRemove);
         currentCalibSelection = allCalibSelections->value(newChannelName);
         calibrationFrameLayout->addWidget(currentCalibSelection, 0, 4);
         currentCalibSelection->show();
         allCalibSelections->remove(channelToRemove);
         delete selectionToRemove;
         currentCalibWidget = channelCalibWidgets->value(newChannelName)->value(currentCalibSelection->currentText());
         currentCalibWidget->show();
         QMap<QString, AF4ChannelCalibWidget*>* calWidgetsToRemove = channelCalibWidgets->value(channelToRemove);
         calWidgetsToRemove->clear();
         channelCalibWidgets->remove(channelToRemove);
         delete calWidgetsToRemove;
         saveParameters();
         adaptConfigWidgetIds();
      }
   }
   else AF4Log::logWarning(tr("You cannot remove the last Channel!"));

}

void AF4ChannelConfigurationWidget::switchChannelWidget(QString newWidgetKey)
{
   currentChConfigWidget->hide();
   currentChConfigWidget = channelConfigWidgets->value(newWidgetKey);
   channelConfigFrameLayout->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   currentChConfigWidget->show();

   if(currentCalibSelection && allCalibSelections){
      currentCalibSelection->hide();
      currentCalibSelection = allCalibSelections->value(newWidgetKey);
      calibrationFrameLayout->addWidget(currentCalibSelection, 0, 4);
      //currentCalibSelection->setCurrentIndex(currentCalibS);
      currentCalibSelection->show();
   }

   if(currentCalibWidget && channelCalibWidgets){
      currentCalibWidget->hide();
      QString calibWidgetKey = (channelCalibWidgets->value(newWidgetKey)->keys()).at(0);

      currentCalibWidget = channelCalibWidgets->value(newWidgetKey)->value(calibWidgetKey);

      calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
      currentCalibWidget->show();

   }
   currentChConfigWidget->show();
}

void AF4ChannelConfigurationWidget::adaptCalibWidgetIds(const QString &channelName, int newChannelId, const QString caller)
{
   int i = 0;
   for(const QString &key: channelCalibWidgets->value(channelName)->keys()){
      AF4ChannelCalibWidget *calibWidget = channelCalibWidgets->value(channelName)->value(key);
      calibWidget->setCalibId(i);
      calibWidget->setChannelId(newChannelId);
      ++i;
   }
}

void AF4ChannelConfigurationWidget::adaptCalibWidgetNames(const QString &channelName)
{
   int i = 0;
   for(const QString &key: channelCalibWidgets->value(channelName)->keys()){
      AF4ChannelCalibWidget *calibWidget = channelCalibWidgets->value(channelName)->value(key);
      calibWidget->setCalibName(key);
      calibWidget->setChannelName(channelName);
      ++i;
   }
}

bool AF4ChannelConfigurationWidget::askCalibRenaming(QString &newName, const QString &oldName)
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
         for(int i=0; i < currentCalibSelection->count() ; i++){
            QString compName = currentCalibSelection->itemText(i);
            if(!(QString::compare(newName, compName))){
               nameIsOk = false;
               break;
            }
         }
      } else return false;
   } while(!nameIsOk);
   return true;
}

void AF4ChannelConfigurationWidget::renameCalibration()
{
   QString newName;
   QString oldCalibName = QString(currentCalibSelection->currentText());

   if(askCalibRenaming(newName, oldCalibName)){

      // remove the old calibration here and inset it under its new key:
      QString channelName = channelSelection->currentText();

      AF4ChannelCalibWidget *oldCalibWidget = channelCalibWidgets->value(channelName)->value(oldCalibName);
      channelCalibWidgets->value(channelName)->remove(oldCalibName);
      channelCalibWidgets->value(channelName)->insert(newName, oldCalibWidget);

      currentCalibSelection->setItemText(currentCalibSelection->currentIndex(), newName);
      int channelId = channelConfigWidgets->value(channelName)->getChannelId();
      adaptCalibWidgetIds(channelName, channelId, "renameCalibration");
      adaptCalibWidgetNames(channelName);
      saveParameters();
   }
}

bool AF4ChannelConfigurationWidget::askCalibAdding(QString &newName)
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
         for(int i=0; i < currentCalibSelection->count() ; i++){
            QString compName = currentCalibSelection->itemText(i);
            if(!(QString::compare(newName, compName))){
               nameIsOk = false;
               break;
            }
         }
      } else return false;
   } while(!nameIsOk);
   return true;
}

bool AF4ChannelConfigurationWidget::addCalibration()
{
   QString newName;
   if(askCalibAdding(newName)){
      // add a new Calibration here:
      if(currentCalibWidget) currentCalibWidget->hide();
     // CalibrationParameters params;

      AF4ChannelCalibWidget* newCalibration = new AF4ChannelCalibWidget(channelConfigWidgets->size(),
                                                                        channelCalibWidgets->value(currentChConfigWidget->getChannelName())->size(),
                                                                        currentChConfigWidget->getChannelName(),
                                                                        newName, calibrationFrame);
      if(currentCalibWidget) newCalibration->setAllCalibrationParameters(currentCalibWidget->getAllCalibrationParameters());
      QObject::connect(newCalibration, SIGNAL(calibrateChannelCalled()), this, SLOT(calibrateChannnel()));
      QString channelName = channelSelection->currentText();
      channelCalibWidgets->value(channelName)->insert(newName, newCalibration);
      currentCalibSelection->addItem(newName);
      currentCalibSelection->setCurrentIndex(currentCalibSelection->count()-1);
      currentCalibWidget = newCalibration;
      int channelId = channelConfigWidgets->value(channelName)->getChannelId();
      adaptCalibWidgetIds(channelName, channelId, "addCalibration");
      adaptCalibWidgetNames(channelName);
      AF4Log::logText(tr("New Calibration \"%1\" added.").arg(newName));
      calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
      currentCalibWidget->show();
      saveParameters();
      return true;
   }
   else return false;
}

void AF4ChannelConfigurationWidget::deleteCalibration()
{
   if(currentCalibSelection->count() > 1){
      AF4DeleteCalibDialog* deleteCalibDialog = new AF4DeleteCalibDialog();
      if(deleteCalibDialog->exec()){
         const QString calibrationToRemove = currentCalibSelection->currentText();
         const QString channelName = channelSelection->currentText();
         int indexToRemove = currentCalibSelection->currentIndex();
         currentCalibSelection->removeItem(indexToRemove);
         delete (channelCalibWidgets->value(channelName)->value(calibrationToRemove));
         channelCalibWidgets->value(channelName)->remove(calibrationToRemove);
         int channelId = channelConfigWidgets->value(channelName)->getChannelId();
         adaptCalibWidgetIds(channelName, channelId, "deleteCalibration");
         adaptCalibWidgetNames(channelName);
         AF4Log::logText(tr("Calibration deleted."));
         saveParameters();
      }
   } else {
      AF4Log::logWarning(tr("You cannot remove the last Calibration!"));
   }

}

void AF4ChannelConfigurationWidget::switchCalibWidget(QString newWidgetKey)
{
   currentCalibWidget->hide();
   QString channelName = channelSelection->currentText();
   currentCalibWidget = channelCalibWidgets->value(channelName)->value(newWidgetKey);
   calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
   currentCalibWidget->show();
}

void AF4ChannelConfigurationWidget::calibrateChannnel()
{
   // calculate omega (channel Width) by calibrator
   AF4Calibrator calibrator;
   //bool calibSuccess = calibrator.calibrate(currentCalibWidget->getAllCalibrationParameters()
   bool calibSuccess = calibrator.calibrate(currentChConfigWidget->getChannelDimensions(),
                                            currentCalibWidget->getParametersForCalibration());

   double newChWidth = calibrator.getChWidth();
   double newChVolume = calibrator.getHydrodynVolume();
   double newGeometVolume = calibrator.getGeometVolume();
   // adjust omega
   if(calibSuccess){
      currentCalibWidget->setChannelWidth(newChWidth);
      currentCalibWidget->setHydrodynVolume(newChVolume);
      AF4Log::logText(tr("Calibration Finished. Channel Width set to %1, Channel Volume set to %2.").arg(newChWidth).arg(newChVolume));
   } else {
      AF4Log::logError(tr("Calibration could not be finished."));
   }
}

void AF4ChannelConfigurationWidget::saveParameters() const
{
   AF4Log::logText(tr("Parameters saved of Channel Calibrations saved."));
   writeSettings();
   for(const QString &configWidgetKey : channelConfigWidgets->keys()){
      channelConfigWidgets->value(configWidgetKey)->writeSettings();
      for(const QString &calibWidgetKey : channelCalibWidgets->value(configWidgetKey)->keys()){
         channelCalibWidgets->value(configWidgetKey)->value(calibWidgetKey)->saveParameters();
      }
   }
}

void AF4ChannelConfigurationWidget::writeSettings() const
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.remove("channels");
   int numberOfChannels = channelSelection->count();
   settings.setValue("channels/number", numberOfChannels);
   for(int i = 0 ; i < numberOfChannels; i++){
      QString channelName = channelSelection->itemText(i);
      int numberOfCalibrations = channelCalibWidgets->value(channelName)->count();
      settings.setValue(tr("channels/%1/numberOfCalibrations").arg(i), numberOfCalibrations);
   }
}

QString AF4ChannelConfigurationWidget::chopStringsQuotMarksToOne(QString string)
{
   QChar compChar = QChar('\"');
   uint stringLength = string.length();
   if(stringLength < 5)
      return QString("");
   while(string.at(1) == compChar){
      string.remove(0, 1);
   }
   stringLength = string.length();
   while(string.at(stringLength - 2) == compChar){
      string.remove(stringLength - 1, 1);
      stringLength = string.length();
   }
   stringLength = string.length();
   if(string.at(0) != compChar) string.prepend(compChar);
   if(string.at(stringLength - 1) != compChar) string.append((compChar));
   return string;
}

QString AF4ChannelConfigurationWidget::chopStringsQuotMarksEntirely(QString string)
{
   QChar compChar = QChar('\"');
   uint stringLength = string.length();
   if(stringLength < 5)
      return QString("");
   while(string.at(0) == compChar){
      string.remove(0, 1);
   }
   stringLength = string.length();
   while(string.at(stringLength - 1) == compChar){
      string.remove(stringLength - 1, 1);
      stringLength = string.length();
   }
   stringLength = string.length();
   return string;
}


//\/////////////////////////// Dialogs /////////////////////////////////////////


void AF4CalibNameDialog::acceptName()
{
   *calibName = QString(calibNameInput->text());
   accept();
}


AF4CalibNameDialog::AF4CalibNameDialog(QString *name, bool first, const QString nameSuggestion, bool rename)
{
   calibName = name;
   setFixedSize(250, 120);
   layout = new QGridLayout(this);
   if(first)
      layout->addWidget(new QLabel("Specify Calibration Name:"), 0, 0, 1, 2, Qt::AlignLeft);
   else{
      layout->addWidget(new QLabel(tr("<font color=\"#FF4400\"><i>\"%1\" exists already.</i></font>").arg(*calibName)),
                        0, 0, 1, 2, Qt::AlignLeft);
      layout->addWidget(new QLabel(tr("Please specify another name:")), 1, 0, 1, 2, Qt::AlignLeft);
   }
   QString defText;
   if(nameSuggestion == 0) defText = QString("New Calibration");
   else defText = nameSuggestion;
   calibNameInput = new QLineEdit(defText, this);
   layout->addWidget(calibNameInput, 2, 0, 1, 2);

   if(rename) accepter = new QPushButton("Rename Calibration", this);
   else accepter = new QPushButton("Add Calibration", this);
   QObject::connect(accepter, SIGNAL(clicked()), this, SLOT(acceptName()));
   layout->addWidget(accepter, 3, 0);
   decliner = new QPushButton("Close");
   QObject::connect(decliner, SIGNAL(clicked()), this, SLOT(reject()));
   layout->addWidget(decliner, 3, 1);

   calibNameInput->setFocus();
}

void AF4ChannelNameDialog::acceptName()
{
   *channelName = QString(channelNameInput->text());
   accept();
}

AF4DeleteCalibDialog::AF4DeleteCalibDialog()
{
   layout = new QGridLayout(this);
   layout->addWidget(new QLabel("Do you really want to delete the current Calibration?"), 0, 0, 1, 2, Qt::AlignLeft);
   accepter = new QPushButton("Yes", this);
   QObject::connect(accepter, SIGNAL(clicked()), this, SLOT(accept()));
   layout->addWidget(accepter, 1, 0);
   decliner = new QPushButton("Close");
   QObject::connect(decliner, SIGNAL(clicked()), this, SLOT(reject()));
   layout->addWidget(decliner, 1, 1);
}

AF4DeleteChannelDialog::AF4DeleteChannelDialog()
{
   layout = new QGridLayout(this);
   layout->addWidget(new QLabel("Do you really want to delete the displayed channel\n and its assigned calibrations?"), 0, 0, 1, 2, Qt::AlignLeft);
   accepter = new QPushButton(" I know, what I am doing!", this);
   QObject::connect(accepter, SIGNAL(clicked()), this, SLOT(accept()));
   layout->addWidget(accepter, 1, 0);
   decliner = new QPushButton("Close");
   QObject::connect(decliner, SIGNAL(clicked()), this, SLOT(reject()));
   layout->addWidget(decliner, 1, 1);
}
#undef CHECK_SETTINGS_CONVERSION
