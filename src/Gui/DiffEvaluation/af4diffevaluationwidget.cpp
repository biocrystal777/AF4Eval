#include "af4diffevaluationwidget.h"

using std::string;
using std::vector;

#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };


AF4DiffEvaluationWidget::AF4DiffEvaluationWidget(QMap<QString, AF4ChannelDimsWidget *> *channelConfigWidgets,
                                                 QMap<QString, QMap<QString, AF4ChannelCalibWidget *> *> *channelCalibWidgets,
                                                 QWidget *parent)
   : QFrame(parent), channelConfigWidgets(channelConfigWidgets), channelCalibWidgets(channelCalibWidgets)
{
   widgetLayout = new QGridLayout(this);

   fileWidget = new AF4FileInOutWidget(QString("diffEvaluation"), "_eval", this);
   widgetLayout->addWidget(fileWidget, 0, 0, 5, 15);

   calibSettingsFrame = new AF4CalibSettingsFrame(channelConfigWidgets, channelCalibWidgets, QString("diffEval"), this);
   widgetLayout->addWidget(calibSettingsFrame, 5, 0, 5, 5);

   settingsWriter = new QPushButton("Save Parameters", this);
   QObject::connect(settingsWriter, &QPushButton::clicked, this, &AF4DiffEvaluationWidget::saveParameters);
   widgetLayout->addWidget(settingsWriter, 15, 0, 1, 2);

   evalStarter = new QPushButton("Start Evaluation", this);
   QObject::connect(evalStarter, &QPushButton::clicked, this, &AF4DiffEvaluationWidget::startEvaluation);
   widgetLayout->addWidget(evalStarter, 15, 2, 1, 2);

   displayZeroMessages = new QCheckBox("Display Calculation Warnings", this);
   displayZeroMessages->setChecked(true);
   widgetLayout->addWidget(displayZeroMessages, 15, 4, 1, 4, Qt::AlignLeft);

   expSettingsFrame = new AF4ExpSettingsFrame(tr("diffEvaluation"), -1, this);
   widgetLayout->addWidget(expSettingsFrame, 5, 6, 10, 5);

   QObject::connect(expSettingsFrame, &AF4ExpSettingsFrame::callEnableVolume,
                    calibSettingsFrame, &AF4CalibSettingsFrame::enableVolume);
}


void AF4DiffEvaluationWidget::startEvaluation()
{

   // Check File Names:
   QString fileName = fileWidget->getInputFilePath(false);
   if(!QFile::exists(fileName)){
      AF4Log::logWarning(tr("File %1 does not exist!").arg(fileName));
      return;
   }
   QString outFileName = fileWidget->getOutputFilePath(false);

   QString testDirName = outFileName;
   while(!(testDirName.endsWith('\\') || testDirName.endsWith('/')))
      testDirName.chop(1);
   testDirName.chop(1);
   QDir testDir(testDirName);
   if(! testDir.exists()){
      AF4Log::logWarning(tr("Chosen Directory does not exist!"));
      AF4Log::logWarning(tr("Evaluation Aborted!"));
      evalStarter->setEnabled(true);
      return;
   }
   // Parse File
   AF4CsvParser parser(fileName.toStdString(), ',', '.');

   bool ok;
   int errorCode;
   uint errorInLine;
   errorCode = parser.parseFile(&errorInLine);
   if(errorCode){
      AF4Log::logError(tr("Error %1").arg(errorCode), true);
      AF4Log::logError(tr("Diffusion Evaluation Aborted while reading the input file."), true);
      return;
   } else AF4Log::logText(tr("File %1 read").arg(fileName));
   parser.getData();
   std::vector<std::string> headLines = parser.getHeadLines();
   matD data = parser.getData();

   QList<bool> activated;
   activated.append(false);
   activated.append(true);
   for(uint i = 2; i < headLines.size(); i++)
      activated.append(false);
   uint timeIndex = 0; // time per definition always in column 0

   QApplication::setOverrideCursor(Qt::WaitCursor);
   double progress = 0;
   uint currentLine = 0;   

   AF4DiffEvaluator evaluator;

   errorCode = evaluator.evalDiffCoeffs(data.at(timeIndex),                                        
                                        expSettingsFrame->getLeftOffsetTimeVal(),
                                        expSettingsFrame->getVoidPeakTimeVal(),
                                        expSettingsFrame->getElutionFlowVal(),
                                        expSettingsFrame->getCrossFlowVal(),
                                        calibSettingsFrame->getChWidth(),
                                        calibSettingsFrame->getHydrodynVolume(),
                                        expSettingsFrame->getRelFocusPointVal(),
                                        calibSettingsFrame->getChLength(),
                                        expSettingsFrame->getRecalcVolume(),
                                        displayZeroMessages->isChecked(),
                                        &progress,
                                        &currentLine);

   if(errorCode){
      QApplication::restoreOverrideCursor();
      parentWidget()->parentWidget()->setEnabled(true);
      AF4Log::logError(tr("Error while evaulating data: Code %1").arg(errorCode));
      switch(errorCode){
      case 1: AF4Log::logError(tr("Error code 1: void peakt time must not be 0.")); break;
      case 2: AF4Log::logError(tr("Error code 2: elution flow must not be 0.")); break;
      case 3: AF4Log::logError(tr("Error code 3: cross flow must not be 0.")); break;
      case 4: AF4Log::logError(tr("Error code 4: channel Width must not be 0.")); break;
      case 5: AF4Log::logError(tr("Error code 5: leftoffsetTime must not be bigger than voidPeak")); break;
      }
      return;
   }

   vecD diffCoeffs = evaluator.getResDiffCoeff();
   evaluator.calcStokesRadii(diffCoeffs, expSettingsFrame->getViscosityVal(), expSettingsFrame->getTemperatureVal());
   vecD stokesRadii = evaluator.getResStokesRadii();

   ///////////////
   // write data
   ///////////////
   AF4CsvWriter writer(outFileName.toStdString());
   matD writeResults;
   // copy "used" times from data to result matrix
   uint startTimeIndex = evaluator.getEvaluationStart();
   // chop beginning of data matrix to evaluation begin;
   uint oldLength = data.at(0).size();
   uint newLength = oldLength - startTimeIndex;


   for (uint i = 0; i < data.size() ; ++i){
      for (uint j = 0; j < newLength; ++j){
         data.at(i).at(j) = data.at(i).at(j + startTimeIndex);
      }
      data.at(i).resize(newLength);
   }
   writeResults.push_back(data.at(timeIndex)); // time
   writeResults.push_back(diffCoeffs); // diffcoeffs

   writeResults.push_back(stokesRadii);
   for (uint i = 1; i < data.size(); i++) // data
      if(i != timeIndex) writeResults.push_back(data.at(i));
   std::vector<std::string> resultHeadLines;
   resultHeadLines.push_back(std::string("time"));
   resultHeadLines.push_back(std::string("Diffusion Coefficients [cm^2/min]"));
   resultHeadLines.push_back(std::string("Stokes Radius [nm]"));
   for(uint i=1; i < headLines.size(); ++i)
      if(i != timeIndex)
         resultHeadLines.push_back(headLines.at(i));
   ok = writer.writeFile(writeResults, resultHeadLines);

   if(ok) AF4Log::logText(tr("Evaluation results written to %1.").arg(outFileName));
   else AF4Log::logError(tr("output file could not be written."));

   parentWidget()->parentWidget()->setEnabled(true);

   QApplication::restoreOverrideCursor();
}

void AF4DiffEvaluationWidget::saveParameters() const
{
   AF4Log::logText(tr("Parameters for the Calculation of Diffusion Coefficients saved."));   
   expSettingsFrame->writeSettings();
   fileWidget->writeSettings();
   calibSettingsFrame->saveParameters();
}

void AF4DiffEvaluationWidget::adaptChannelParameters()
{
    if(calibSettingsFrame) calibSettingsFrame->adaptChannelParameters();
}

void AF4DiffEvaluationWidget::adaptExpParameters(double tempVal,
                                                 double eluFlowVal,
                                                 double crossFlowVal,
                                                 double voidPeakVal)
{
   if(expSettingsFrame){
      expSettingsFrame->setTemperatureVal(tempVal);
      expSettingsFrame->setElutionFlowVal(eluFlowVal);
      expSettingsFrame->setCrossFlowVal(crossFlowVal);
      expSettingsFrame->setVoidPeakTimeVal(voidPeakVal);
   };
}

