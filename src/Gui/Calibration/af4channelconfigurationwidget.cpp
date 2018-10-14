#include "af4channelconfigurationwidget.h"

using std::string;

AF4ChannelConfigurationWidget::AF4ChannelConfigurationWidget(QWidget *parent) :
   QWidget(parent), settingsWriter (QSharedPointer<QPushButton>(new QPushButton("Save Parameters", this)))
{

   layout = new QGridLayout(this);
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok;
   //\/////////////////////////
   // Basical initialization //
   //\/////////////////////////

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
   connect(renameChButton, &QPushButton::clicked, this, &AF4ChannelConfigurationWidget::renameChannel);
   channelConfigFrameLayout->addWidget(renameChButton, 0, 5);

   addChButton = new QToolButton(channelConfigFrame);
   addChButton->setText(tr("+"));
   addChButton->setToolTip("Add new Channel");
   connect(addChButton, &QPushButton::clicked, this, &AF4ChannelConfigurationWidget::addChannel);
   channelConfigFrameLayout->addWidget(addChButton, 0, 6);
   deleteChButton = new QToolButton(channelConfigFrame);
   deleteChButton->setText(tr("-"));
   deleteChButton->setToolTip("delete current Channel");

   connect(deleteChButton, &QPushButton::clicked, this, &AF4ChannelConfigurationWidget::deleteChannel);
   channelConfigFrameLayout->addWidget(deleteChButton, 0, 7);

   ////////////////////////////////////////////////////
   // initialize channels with values from QSettings //
   ////////////////////////////////////////////////////
   //qDebug() << "a3";
   channelConfigWidgets = new QMap<QString, AF4ChannelDimsWidget*>();

   // make all channel widgets, add them to the QMap
   // of ChannelWidgets and insert them
   // into the ComboBox
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };


   uint numberOfChannels = settings.value("channels/number", 0).toInt(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "1");

   for(uint i = 0; i < numberOfChannels; i++){
      // double channelValue;
      QString newChannelName = settings.value(tr("channels/%1/name").arg(i), tr("New Channel %1").arg(i)).toString();
      currentChConfigWidget = new AF4ChannelDimsWidget(i, newChannelName, channelConfigFrame);
      channelConfigWidgets->insert(newChannelName, currentChConfigWidget);
      channelSelection->addItem(newChannelName);
      channelSelection->setCurrentIndex(channelSelection->count()-1);
      connect(channelSelection, qOverload<const QString&>(&QComboBox::currentIndexChanged),
              this, &AF4ChannelConfigurationWidget::switchChannelWidget );
      currentChConfigWidget->hide();
   }
   if(numberOfChannels == 0)
      while (!addChannel());

   channelConfigFrameLayout->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   currentChConfigWidget->show();

   ///////////////////////
   // Calibration Frame //
   ///////////////////////
   //qDebug() << "a4";
   uint numberOfCalibrations = 0;
   QString calibName;
   QString channelName;

   for(uint i=0; i < numberOfChannels; ++i){
      numberOfCalibrations = settings.value(tr("channels/%1/numberOfCalibrations").arg((i)), 0).toInt(&ok);
      CHECK_SETTINGS_CONVERSION(numberOfCalibrations, 0);
      currentCalibSelection =  new QComboBox(calibrationFrame);

      currentCalibSelection->setToolTip("Choose a Calibration profile");
      currentCalibSelection->hide();
      calibrationFrameLayout->addWidget(currentCalibSelection, 0, 11);
      allCalibSelections->insert(channelSelection->itemText(i), currentCalibSelection);

      channelName = channelSelection->itemText(i);
      channelCalibWidgets->insert(channelName, new QMap<QString, AF4ChannelCalibWidget*>);
    //  qDebug() << "a5"<< numberOfCalibrations;
      for(uint j=0; j < numberOfCalibrations; j++){
         //qDebug() << "b1"<< numberOfCalibrations;
         calibName = settings.value(tr("channels/%1/calib/%2/name").arg(i).arg(j), "").toString();
         currentCalibWidget = new AF4ChannelCalibWidget(i, j, channelName, calibName, settingsWriter, calibrationFrame);
         //qDebug() << "b2"<< numberOfCalibrations;
         connect(currentCalibWidget, &AF4ChannelCalibWidget::calibrateChannelCalled,
                 this, &AF4ChannelConfigurationWidget::calibrateChannnel);
         currentCalibSelection->addItem(calibName);
         channelCalibWidgets->value(channelName)->insert(calibName, currentCalibWidget);
         //qDebug() << "b3"<< numberOfCalibrations;
         calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
         currentCalibWidget->hide();
      }
      connect(currentCalibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4ChannelConfigurationWidget::switchCalibWidget);

   }
   #undef CHECK_SETTINGS_CONVERSION

   plotWidget = new AF4CalibPlotWidget(calibrationFrame);
   calibrationFrameLayout->addWidget(plotWidget, 2, 7, 7, 7);

   //qDebug() << "a5";
   renameCalibButton = new QToolButton(calibrationFrame);
   renameCalibButton->setText("R");
   renameCalibButton->setToolTip("Rename the current calibration profile");

   connect(renameCalibButton, &QPushButton::clicked,
           this, &AF4ChannelConfigurationWidget::renameCalibration);
   calibrationFrameLayout->addWidget(renameCalibButton, 0, 12);

   addCalibButton = new QToolButton(calibrationFrame);
   addCalibButton->setText(tr("+"));
   addCalibButton->setToolTip("Add new calibration profile");
   connect(addCalibButton, &QPushButton::clicked,
           this, &AF4ChannelConfigurationWidget::addCalibration);
   calibrationFrameLayout->addWidget(addCalibButton, 0, 13);

   deleteCalibButton = new QToolButton(calibrationFrame);
   deleteCalibButton->setText(tr("-"));
   deleteCalibButton->setToolTip("delete current calibration profile");
   connect(deleteCalibButton, &QPushButton::clicked,
           this, &AF4ChannelConfigurationWidget::deleteCalibration);
   calibrationFrameLayout->addWidget(deleteCalibButton, 0, 14);
   //-///////////////////////
   // set starting widgets //
   //-///////////////////////

   channelSelection->setCurrentIndex(0);
   channelName = channelSelection->currentText();
   currentChConfigWidget = channelConfigWidgets->value(channelName);
   currentChConfigWidget->show();
   currentCalibSelection->show();
   currentCalibSelection->setCurrentIndex(0);
   currentCalibWidget = channelCalibWidgets->value(channelName)->value(currentCalibSelection->currentText());
   // connect boxes to widget
   connectCtrlWithPlotWidget();
   adaptPlotData();
   currentCalibWidget->show();

   //\///////////////////////////
   // add Frames to the layout //
   //\///////////////////////////

   layout->addWidget(channelConfigFrame, 0, 0, 2, 10);
   layout->addWidget(calibrationFrame, 3, 0, 12, 10);

   connect(settingsWriter.data(), &QPushButton::clicked,
           this, &AF4ChannelConfigurationWidget::saveParameters);

   layout->addWidget(settingsWriter.data(), 15, 0);

}

AF4ChannelConfigurationWidget::~AF4ChannelConfigurationWidget()
{
   writeSettings();
   delete channelConfigWidgets;
   delete channelCalibWidgets;
   delete allCalibSelections;
}

void AF4ChannelConfigurationWidget::adaptConfigWidgetIds()
{
   int i = 0;
   for(const QString &key : channelCalibWidgets->keys()){
      AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
      configWidget->setConfigId(i);
      adaptCalibWidgetIds(key, i);
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

      channelSelection->blockSignals(true);
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

      connect(currentCalibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4ChannelConfigurationWidget::switchCalibWidget);
      channelSelection->blockSignals(false);

      AF4Log::logText(tr("New Channel \"%1\" added.").arg(newName));

      adaptConfigWidgetIds();
      // set final current widgets;
      switchCalibWidget(newName);
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
   currentChConfigWidget->show();
   if(currentCalibWidget && channelCalibWidgets){
      //currentCalibWidget->hide();
      QString calibWidgetKey = (channelCalibWidgets->value(newWidgetKey)->keys()).at(0);
      switchCalibWidget(calibWidgetKey);
      //currentCalibWidget = channelCalibWidgets->value(newWidgetKey)->value(calibWidgetKey);
      //calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
      //currentCalibWidget->show();
   }
}

void AF4ChannelConfigurationWidget::adaptCalibWidgetIds(const QString &channelName, int newChannelId)
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
      adaptCalibWidgetIds(channelName, channelId);
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
                                                                        newName,
                                                                        this->settingsWriter,
                                                                        calibrationFrame);
      if(currentCalibWidget) newCalibration->setAllCalibrationParameters(currentCalibWidget->getAllCalibrationParameters());
      connect(newCalibration, &AF4ChannelCalibWidget::calibrateChannelCalled, this, &AF4ChannelConfigurationWidget::calibrateChannnel);
      QString channelName = channelSelection->currentText();
      channelCalibWidgets->value(channelName)->insert(newName, newCalibration);
      currentCalibSelection->addItem(newName);
      currentCalibSelection->setCurrentIndex(currentCalibSelection->count()-1);
      currentCalibWidget = newCalibration;
      int channelId = channelConfigWidgets->value(channelName)->getChannelId();
      adaptCalibWidgetIds(channelName, channelId);
      adaptCalibWidgetNames(channelName);
      AF4Log::logText(tr("New Calibration \"%1\" added.").arg(newName));
      calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
      switchCalibWidget(newName);
      //currentCalibWidget->show();
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
         adaptCalibWidgetIds(channelName, channelId);
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
   plotWidget->disconnectCurrentMarkers();

   QString channelName = channelSelection->currentText();
   currentCalibWidget = channelCalibWidgets->value(channelName)->value(newWidgetKey);
   calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
   connectCtrlWithPlotWidget();
   //auto ctrl = currentCalibWidget->getCtrlBoxRefs();
   //plotWidget->connectMarkers(ctrl.leftOffset, ctrl.tVoid, ctrl.tElution);
   adaptPlotData();
   currentCalibWidget->show();
}

void AF4ChannelConfigurationWidget::calibrateChannnel()
{
   AF4Log::logText(tr("Start calibration..."));
   QGuiApplication::setOverrideCursor(Qt::WaitCursor);
   ChannelDims chDims  = currentChConfigWidget->getChannelDimensions();
   ParametersForCalibration params = currentCalibWidget->getParametersForCalibration();
   CalibModeSettings cModes = currentCalibWidget->getCalibModes();

   calibRealMeaurement(chDims, params, cModes);
   if(cModes.checkUncertainty){
      calibUncertaintyGrid(chDims, params, cModes);
      AF4Log::logText(tr(""));
   }
   QGuiApplication::restoreOverrideCursor();
}

void AF4ChannelConfigurationWidget::calibRealMeaurement(const ChannelDims &chDims, const ParametersForCalibration &params, const CalibModeSettings &cModes)
{

   AF4Log::logText(tr("Calibrate with measured values..."));
   CalibResult result;
   if(cModes.classical){
      AF4Log::logText(tr("Try \"Classical\" calibration..."));
      result = calibSingleParamSet(chDims, params, CalibMode::classical);
      if(result.errorCode == CalibErrorCode::noError){
         AF4Log::logText(tr("\"Classical\" calibration Finished. w_cla set to %1, V_cla set to %2.").arg(result.width).arg(result.volume));
         currentCalibWidget->setChannelWidthClassical(result.width);
         currentCalibWidget->setClassicalVolume(result.volume);
      }
      else      {
         AF4Log::logError(tr("\"Classical\" calibration could not be conducted."));
         logErrorMessage(result.errorCode);
      }
   }
   if(cModes.geometric){
      AF4Log::logText(tr("Try \"Geometric\" calibration..."));
      result = calibSingleParamSet(chDims, params, CalibMode::geometric);
      if(result.errorCode == CalibErrorCode::noError){
         AF4Log::logText(tr("\"Geometric\" calibration finished. w_geo set to %1, V_geo set to %2.").arg(result.width).arg(result.volume));
         currentCalibWidget->setChannelWidthGeo(result.width);
         currentCalibWidget->setGeometVolume(result.volume);
      }
      else {
         AF4Log::logError(tr("\"Geometric\" calibration could not be conducted."));
         logErrorMessage(result.errorCode);
      }
   }
   if(cModes.hydrodynamic){
      AF4Log::logText(tr("Try \"Hydrodynamic\" calibration..."));
      result = calibSingleParamSet(chDims, params, CalibMode::hydrodynamic);
      if(result.errorCode == CalibErrorCode::noError){
         AF4Log::logText(tr("\"Hydrodynamic\" calibration Finished. w_hyd set to %1, V_hyd set to %2.").arg(result.width).arg(result.volume));
         currentCalibWidget->setChannelWidthHydro(result.width);
         currentCalibWidget->setChannelWidthGeo(result.volume);
      }
      else {
         AF4Log::logError(tr("\"Hydrodynamic\" calibration could not be conducted."));
         logErrorMessage(result.errorCode);
      }
   }
}

CalibResult AF4ChannelConfigurationWidget::calibSingleParamSet(ChannelDims chDims, ParametersForCalibration params, CalibMode mode)
{
   AF4Calibrator calibrator(chDims, params);
   CalibResult result;
   result.errorCode = calibrator.checkParameters();
   if(result.errorCode != CalibErrorCode::noError) return result;

   switch (mode) {
   case CalibMode::classical:
      result = calibrator.calibrate_classic();
      break;
   case CalibMode::hydrodynamic:
      result = calibrator.calibrate_hydrodynamic();
      break;
   case CalibMode::geometric:
      result = calibrator.calibrate_geometric();
      break;
   }
  return result;
}



void AF4ChannelConfigurationWidget::calibUncertaintyGrid(const ChannelDims &chDims, const ParametersForCalibration &params, const CalibModeSettings &cModes)
{

   //-////////////////////////////////////////
   // non-const parameter structures for
   // Macro-based parameter modifications
   //-////////////////////////////////////////

   struct ChannelDims_nonConst {
      double length1;
      double length2;
      double length3;
      double chLength;
      double b0;
      double bL;
   };
   struct ParamsForCalibration_nonConst {
      double elutionFlow;
      double crossFlow;
      double relFocusPoint;
      //double leftOffsetTime;
      double voidPeakTime;
      double elutionTime;
      double diffCoeff;
   };

   //-/////////////////////////////////////////////////////////////////
   //
   // converters for const to non-const containers and vice versa
   //
   //-/////////////////////////////////////////////////////////////////

   auto dimsToNonConst = [](const ChannelDims &constDims)
         -> ChannelDims_nonConst {
      return ChannelDims_nonConst
      {        constDims.length1,
               constDims.length2,
               constDims.length3,
               constDims.chLength,
               constDims.b0,
               constDims.bL
      };
   };
   auto dimsToConst = [](const ChannelDims_nonConst &nonConstDims)
         -> ChannelDims {
      return ChannelDims
      {        nonConstDims.length1,
               nonConstDims.length2,
               nonConstDims.length3,
               nonConstDims.chLength,
               nonConstDims.b0,
               nonConstDims.bL
      };
   };
   auto paramsToNonConst = [](const ParametersForCalibration &nonConstParams)
         -> ParamsForCalibration_nonConst {
      return ParamsForCalibration_nonConst
      {        nonConstParams.elutionFlow,
               nonConstParams.crossFlow,
               nonConstParams.relFocusPoint,
               //nonConstParams.leftOffsetTime,
               nonConstParams.voidPeakTime,
               nonConstParams.elutionTime,
               nonConstParams.diffCoeff
      };
   };
   auto paramsToConst = [](const ParamsForCalibration_nonConst &constParams)
         ->ParametersForCalibration {
      return  ParametersForCalibration
      {        constParams.elutionFlow,
               constParams.crossFlow,
               constParams.relFocusPoint,
               //constParams.leftOffsetTime,
               constParams.voidPeakTime,
               constParams.elutionTime,
               constParams.diffCoeff
      };
   };

   //-/////////////////////////////////////////////////////////////////////////////
   //
   // Start of actual function
   // void .....::  calibUncertaintyGrid(const ChannelDims &chDims,
   //                                    const ParametersForCalibration &params,
   //                                    const CalibModeSettings &cModes)
   //
   //-//////////////////////////////////////////////////////////////////////////////

   const double deltaMax = cModes.uncertRange / 100.0; // from "%" to factor
   //const double deltaMin = - deltaMax;
   const uint   gridMidPos = cModes.uncertGridSize;
   const uint   gridSize(2 * gridMidPos + 1);
   vecD devXRel;                 // relative deviating sizes within the defined range [X - X⋅δmax, X + X⋅δmax]
   devXRel.resize(gridSize);
   vecD deltaWidth;              // relative deviations of channel volume from  ( (Y(X ± δX) / Y(X)) - 1 )
   deltaWidth.resize(gridSize);
   vecD deltaVolume;             // relative deviations of channel width from  ( (Y(X ± δX) / Y(X)) - 1 )
   deltaVolume.resize(gridSize);

/*
 * DELTAMAGN         = size in one of the containers
 * CONSTCONTAINER    = conatainer of DELTAMAGN (params or chDims)
 * NONCONSTCONTAINER = corresponding modifyable version ()
 * CALIBMODE         = one of the three CalibModes (classical, geometric or hydrodynamic)
 * FILENAMESUFF      = "_*.csv"
 *
*/
#define ITERATIVE_PARAMETER_DELTA_ANALYSIS(DELTAMAGN, CONSTCONTAINER, NONCONSTCONTAINER, CALIBMODE , FILENAMESUFFIX) \
   {                                                                           \
   const double X = CONSTCONTAINER . DELTAMAGN;                                \
   for(uint i = 0; i < gridSize; ++i){                                         \
   /* create modified parameter structures, for both containers,               \
    *  so no macro parameter required */                                       \
   ParamsForCalibration_nonConst paramsDeltaMod = paramsToNonConst(params);    \
   ChannelDims_nonConst chDimsDeltaMod  = dimsToNonConst(chDims);              \
   /* get the modifier between δmin to δmax */                                \
   devXRel[i] =  static_cast<double>(i) - static_cast<double>(gridMidPos);    \
   devXRel[i] *= deltaMax / static_cast<double>(gridMidPos);                  \
   /* modify δsize parameter to X ± δX, specific in NONCONSTCONTAINER REQUIRED */ \
   NONCONSTCONTAINER.DELTAMAGN = (1.0 + devXRel[i]) * X;                      \
   /* conduct calibration */                                                  \
   CalibResult deltaResult = calibSingleParamSet(dimsToConst(chDimsDeltaMod), \
   paramsToConst(paramsDeltaMod),                                             \
   CalibMode::CALIBMODE);                                                     \
   deltaWidth[i]  = (deltaResult.width  / widthRefY  ) - 1.0;                 \
   deltaVolume[i] = (deltaResult.volume / VolumeRefY ) - 1.0;                 \
   AF4Log::logText(tr("Tested for modified %1 = %2 , with δX = %3\%")         \
                   .arg(#DELTAMAGN)                                           \
                   .arg(paramsDeltaMod.DELTAMAGN)                             \
                   .arg(devXRel[i] * 100.0));                                 \
}                                                                             \
/* write results */                                                           \
string outFile = filePathPrefix + string(FILENAMESUFFIX);                     \
AF4Log::logText(tr("Write result to %1").arg(outFile.c_str()));               \
AF4CsvWriter csvWriter(outFile);                                              \
csvWriter.writeFile(matD{devXRel, deltaWidth, deltaVolume} , header);         \
};

/*----------------------------------------------------------------------------------------------------------------
//Expanded example for Macro with arguments (elutionFlow, params, paramsDeltaMod, classical,"_elutionFlow.csv")
{
   const double X = params.elutionFlow;
   for(uint i = 0; i < gridSize; ++i){
      // create modified parameter structures:
      ParamsForCalibration_nonConst paramsDeltaMod = paramsToNonConst(params);
      ChannelDims_nonConst chDimsDeltaMod  = dimsToNonConst(chDims);
      // get the modifier between δmin to δmax
      devXRel[i] =  static_cast<double>(i) - static_cast<double>(gridMidPos);
      devXRel[i] *= deltaMax / static_cast<double>(gridMidPos);
      modify δsize parameter to X ± δX
      paramsDeltaMod.elutionFlow = (1.0 + devXRel[i]) * X;
      // conduct calibration
      CalibResult deltaResult = calibSingleParamSet(dimsToConst(chDimsDeltaMod),
                                                    paramsToConst(paramsDeltaMod),
                                                    CalibMode::classical);
      deltaWidth[i]  = (deltaResult.width  / widthRefY  ) - 1.0;
      deltaVolume[i] = (deltaResult.volume / VolumeRefY ) - 1.0;
      AF4Log::logText(tr("Tested for modified %1 = %2 , with δX = %3\%")
                      .arg("elutionFlow")
                      .arg(paramsDeltaMod.elutionFlow)
                      .arg(devXRel[i] * 100.0));
   }
   // write results
   string outFile = filePathPrefix + string("_elutionFlow.csv");
   AF4Log::logText(tr("Write result to %1").arg(outFile.c_str()));
   AF4CsvWriter csvWriter(outFile);
   csvWriter.writeFile(matD{devXRel, deltaWidth, deltaVolume} , header);
};
-------------------------------------------------------------------------------------------------------------------*/

   CalibResult refResult;
   if(cModes.classical){
      // try a single test calibration to
      refResult = calibSingleParamSet(chDims, params, CalibMode::classical);
      if(refResult.errorCode != CalibErrorCode::noError){
         logErrorMessage(refResult.errorCode);
         return;
      }
      const double widthRefY = refResult.width;   // Y_width(X)
      const double VolumeRefY = refResult.volume;  // Y_vol(X)
      const string filePathPrefix("/home/bluemage/tests/deltaTests/delta");
      const std::vector<string> header = {string("delta X"), string("delta width"), string("delta volume") };

      ITERATIVE_PARAMETER_DELTA_ANALYSIS(elutionFlow,   params, paramsDeltaMod, classical, "_elutionFlow_cla.csv"   );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(crossFlow,     params, paramsDeltaMod, classical, "_crossFlow_cla.csv"     );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(relFocusPoint, params, paramsDeltaMod, classical, "_relFocusPoint_cla.csv" );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(voidPeakTime,  params, paramsDeltaMod, classical, "_voidPeakTime_cla.csv"  );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(elutionTime,   params, paramsDeltaMod, classical, "_elutionTime_cla.csv"   );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(diffCoeff,     params, paramsDeltaMod, classical, "_diffCoeff_cla.csv"     );

   }
   if(cModes.geometric){
      refResult = calibSingleParamSet(chDims, params, CalibMode::geometric);
      if(refResult.errorCode != CalibErrorCode::noError){
         logErrorMessage(refResult.errorCode);
         return;
      }
      else
         logErrorMessage(refResult.errorCode);
   }
   if(cModes.hydrodynamic){
      refResult = calibSingleParamSet(chDims, params, CalibMode::hydrodynamic);
      if(refResult.errorCode != CalibErrorCode::noError){
         logErrorMessage(refResult.errorCode);
         return;
      }
      else
         logErrorMessage(refResult.errorCode);
   }

#undef ITERATIVE_PARAMETER_DELTA_ANALYSIS

}

void AF4ChannelConfigurationWidget::logErrorMessage(CalibErrorCode errorCode)
{
   switch(errorCode){
   case CalibErrorCode::noError:
      break;
   case CalibErrorCode::voidTimeZero:
      AF4Log::logError(tr("tVoid = 0!"));
   case CalibErrorCode::eluFlowZero:
      AF4Log::logError(tr("Ve = 0!"));
   case CalibErrorCode::eluTimeZero:
      AF4Log::logError(tr("te = 0!"));
   case CalibErrorCode::crossFlowZero:
      AF4Log::logError(tr("Vc = 0!"));
   case CalibErrorCode::diffCoeffZero:
      AF4Log::logError(tr("D = 0!"));
   case CalibErrorCode::voidTimeTooSmall:
      AF4Log::logError(tr("tvoid must not be smaller than offset!"));
   case CalibErrorCode::eluTimeTooSmall:
      AF4Log::logError(tr("te must not be smaller than tvoid!"));
   case CalibErrorCode::ParamsNotChecked:
      AF4Log::logError(tr("Parameters were not checked! رجم مبرمج"));
      break;
   default:
      break;
   }
}

   void AF4ChannelConfigurationWidget::saveParameters() const
{
   AF4Log::logText(tr("Parameters saved of Channel Calibrations saved."));
   writeSettings();
   for(const QString &configWidgetKey : channelConfigWidgets->keys()){
      channelConfigWidgets->value(configWidgetKey)->writeSettings();
      //for(const QString &calibWidgetKey : channelCalibWidgets->value(configWidgetKey)->keys()){
      //channelCalibWidgets->value(configWidgetKey)->value(calibWidgetKey)->saveParameters();
      //}
   }
}



void AF4ChannelConfigurationWidget::adaptPlotData()
{
   QString fileName = currentCalibWidget->getInputFileName();
   if(!fileName.isEmpty())
      plotWidget->setPlotData(fileName);
}

void AF4ChannelConfigurationWidget::connectCtrlWithPlotWidget()
{
   auto ctrl =  currentCalibWidget->getCtrlBoxRefs();
   plotWidget->connectMarkers(ctrl.leftOffset, ctrl.tVoid, ctrl.tElution);
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




/**************************************
 *
 *
 *
 * AF4ChannelDimsOrgFrame
 *
 *
 *
 *************************************/



AF4ChannelDimsOrgFrame::AF4ChannelDimsOrgFrame(QWidget *parent) : QFrame(parent)
{





}















/**************************************
 *
 *
 *
 * AF4ChannelCalibsOrgFrame
 *
 *
 *
 *************************************/




AF4ChannelCalibsOrgFrame::AF4ChannelCalibsOrgFrame(QComboBox *channelSelection, QWidget *parent) : QFrame(parent)
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   bool ok;
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };

   uint numberOfChannels = settings.value("channels/number", 0).toInt(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "1");

   uint numberOfCalibrations = 0;
   QString calibName;
   QString channelName;

   for(uint i=0; i < numberOfChannels; ++i){
      numberOfCalibrations = settings.value(tr("channels/%1/numberOfCalibrations").arg((i)), 0).toInt(&ok);


      CHECK_SETTINGS_CONVERSION(numberOfCalibrations, 0);
      currentCalibSelection =  new QComboBox(calibrationFrame);

      currentCalibSelection->setToolTip("Choose a Calibration profile");
      currentCalibSelection->hide();
      calibrationFrameLayout->addWidget(currentCalibSelection, 0, 11);
      allCalibSelections->insert(channelSelection->itemText(i), currentCalibSelection);

      channelName = channelSelection->itemText(i);
      channelCalibWidgets->insert(channelName, new QMap<QString, AF4ChannelCalibWidget*>);
    //  qDebug() << "a5"<< numberOfCalibrations;
      for(uint j=0; j < numberOfCalibrations; j++){
         //qDebug() << "b1"<< numberOfCalibrations;
         calibName = settings.value(tr("channels/%1/calib/%2/name").arg(i).arg(j), "").toString();
         currentCalibWidget = new AF4ChannelCalibWidget(i, j, channelName, calibName, settingsWriter, calibrationFrame);
         //qDebug() << "b2"<< numberOfCalibrations;
         connect(currentCalibWidget, &AF4ChannelCalibWidget::calibrateChannelCalled,
                 this, &AF4ChannelCalibsOrgFrame::calibrateChannnel);
         currentCalibSelection->addItem(calibName);
         channelCalibWidgets->value(channelName)->insert(calibName, currentCalibWidget);
         //qDebug() << "b3"<< numberOfCalibrations;
         calibrationFrameLayout->addWidget(currentCalibWidget, 2, 0, 7, 7);
         currentCalibWidget->hide();
      }
      connect(currentCalibSelection, qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4ChannelCalibsOrgFrame::switchCalibWidget);

   }
   #undef CHECK_SETTINGS_CONVERSION

   plotWidget = new AF4CalibPlotWidget(calibrationFrame);
   calibrationFrameLayout->addWidget(plotWidget, 2, 7, 7, 7);

   //qDebug() << "a5";
   renameCalibButton = new QToolButton(calibrationFrame);
   renameCalibButton->setText("R");
   renameCalibButton->setToolTip("Rename the current calibration profile");

   connect(renameCalibButton, &QPushButton::clicked,
           this, &AF4ChannelCalibsOrgFrame::renameCalibration);
   calibrationFrameLayout->addWidget(renameCalibButton, 0, 12);

   addCalibButton = new QToolButton(calibrationFrame);
   addCalibButton->setText(tr("+"));
   addCalibButton->setToolTip("Add new calibration profile");
   connect(addCalibButton, &QPushButton::clicked,
           this, &AF4ChannelCalibsOrgFrame::addCalibration);
   calibrationFrameLayout->addWidget(addCalibButton, 0, 13);

   deleteCalibButton = new QToolButton(calibrationFrame);
   deleteCalibButton->setText(tr("-"));
   deleteCalibButton->setToolTip("delete current calibration profile");
   connect(deleteCalibButton, &QPushButton::clicked,
           this, &AF4ChannelCalibsOrgFrame::deleteCalibration);
   calibrationFrameLayout->addWidget(deleteCalibButton, 0, 14);


}

void AF4ChannelCalibsOrgFrame::adaptPlotData()
{

}

bool AF4ChannelCalibsOrgFrame::addCalibration()
{

}

void AF4ChannelCalibsOrgFrame::renameCalibration()
{

}

void AF4ChannelCalibsOrgFrame::deleteCalibration()
{

}

void AF4ChannelCalibsOrgFrame::switchCalibWidget(QString newWidgetKey)
{

}

void AF4ChannelCalibsOrgFrame::calibrateChannnel()
{

}

void AF4ChannelCalibsOrgFrame::saveParameters() const
{

}
