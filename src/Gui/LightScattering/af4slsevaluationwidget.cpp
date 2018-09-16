#include "af4slsevaluationwidget.h"

using std::vector;
using std::string;
using std::map;

AF4SLSEvaluationWidget::AF4SLSEvaluationWidget(QWidget *parent) :
   QWidget(parent)
{   

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   bool ok;
   int numberOfPeaks = settings.value(tr("/slsEvaluation/peakParameters/numberOfPeaks"), 1).toInt(&ok);
   if(numberOfPeaks < 1) numberOfPeaks = 1;
   if(!ok) AF4Log::logWarning(tr("Could not read number Of Peaks."));

   widgetLayout = new QGridLayout(this);

   fileWidget = new AF4FileInOutWidget("slsEvaluation", "_sls", this);

   evaluationFrame = new QFrame(this);
   evaluationFrame->setFrameStyle(0x1011);
   evaluationLayout = new QGridLayout(evaluationFrame);

   evalGroup = new QGroupBox(tr("Choose evaluation method"), evaluationFrame);
   evalGroup->setFlat(false);
   evalLayout = new QHBoxLayout(evalGroup);
   zimmPlot = new QRadioButton("Zimm", evalGroup);
   zimmPlot->setChecked(true);
   evalLayout->addWidget(zimmPlot);
   kratkyPlot = new QRadioButton("Kratky", evalGroup);
   evalLayout->addWidget(kratkyPlot);
   kratkyPlot->setEnabled(false);
   berryPlot = new QRadioButton("Berry", evalGroup);
   evalLayout->addWidget(berryPlot);
   berryPlot->setEnabled(false);
   guinierPlot = new QRadioButton("Guinier", evalGroup);
   evalLayout->addWidget(guinierPlot);
   guinierPlot->setEnabled(false);
   evaluationLayout->addWidget(evalGroup, 0, 0, 2, 15);

   concGroup = new QGroupBox("Calculate c(t) from", evaluationFrame);
   concGroup->setFlat(false);
   concLayout = new QHBoxLayout(concGroup);
   refIndexConc = new QRadioButton("Refractive Index", concGroup);
   refIndexConc->setChecked(true);
   concLayout->addWidget(refIndexConc);
   absorbanceConc = new QRadioButton("Absorbance",concGroup);

   concLayout->addWidget(absorbanceConc);
   evaluationLayout->addWidget(concGroup, 1, 0, 2, 10, Qt::AlignBottom);

   QObject::connect(refIndexConc, SIGNAL(toggled(bool)), this,SLOT(emitConcModeChanged()));
   QObject::connect(absorbanceConc, SIGNAL(toggled(bool)), this,SLOT(emitConcModeChanged()));

   evaluationLayout->addWidget(new QLabel(tr("c<sub>min</sub> [mg/ml]"), evaluationFrame), 4, 0, 1, 1, Qt::AlignRight);
   concentrationCut = new FFFTwoBoxWidget(QString("Minimum concentration to be evaluated"),evaluationFrame);
   concentrationCut->setMinimum(1.0e-15);
   evaluationLayout->addWidget(concentrationCut, 4, 1, 1, 4);

   evaluationLayout->addWidget(new QLabel(tr("lambda<sub>laser</sub>"), evaluationFrame), 5, 0, 1, 1, Qt::AlignRight);
   laserWaveLength = new QDoubleSpinBox(evaluationFrame);
   laserWaveLength->setDecimals(1);
   laserWaveLength->setMinimum(100.0);
   laserWaveLength->setMaximum(1000.0);
   evaluationLayout->addWidget(laserWaveLength, 5, 1, 1, 4);

   evaluationLayout->addWidget(new QLabel(tr("n0:"), evaluationFrame), 6, 0, 1, 1, Qt::AlignLeft);
   refIndexSolvent = new QDoubleSpinBox(evaluationFrame);
   refIndexSolvent->setDecimals(5);
   refIndexSolvent->setToolTip("Refractive Index of the solvent");
   evaluationLayout->addWidget(refIndexSolvent, 6, 1, 1, 4, Qt::AlignLeft);

   /*
   evalPeaksSeparately = new QCheckBox(tr("Evaluate Peaks separately"), evaluationFrame);
   evalPeaksSeparately->setChecked(true);
   QObject::connect(evalPeaksSeparately, SIGNAL(toggled(bool)), this, SLOT(enablePeakRanges(bool)));
   evaluationLayout->addWidget(evalPeaksSeparately, 7, 0, 1, 5, Qt::AlignLeft);
   */
   calculateShapeFactor = new QCheckBox(tr("Calculate shape factor rho"), evaluationFrame);
   calculateShapeFactor->setToolTip("Calculate the ratio between hydodynamic radius and radius of gyaration");
   calculateShapeFactor->setChecked(true);
   calculateShapeFactor->setEnabled(false);
   evaluationLayout->addWidget(calculateShapeFactor, 8, 0, 1, 5, Qt::AlignLeft);

   settingsWriter = new QPushButton("Save Parameters", evaluationFrame);
   QObject::connect(settingsWriter, SIGNAL(clicked()), this, SLOT(saveParameters()));
   evaluationLayout->addWidget(settingsWriter, 10, 0, 1, 2);

   evalStarter = new QPushButton("Start Evaluation", evaluationFrame);
   QObject::connect(evalStarter, SIGNAL(clicked()), this, SLOT(startEvaluation()));
   evaluationLayout->addWidget(evalStarter, 10, 2, 1, 4);

   calibrationFrame = new AF4SLSCalibrationFrame("slsEvaluation", -1, this);
   //peakParameterFrame = new FFFPeakParameterFrame("slsEvaluation", -1, this);
   peakParameterFrame = new AF4PeakParameterFrame("slsEvaluation", -1, this);
   QObject::connect(this, SIGNAL(concModeChanged(SLSConcMode)), peakParameterFrame, SLOT(adoptConcentrationMode(SLSConcMode)));

   widgetLayout->addWidget(fileWidget      , 0, 0 );
   widgetLayout->addWidget(peakParameterFrame  , 0, 1 );
   widgetLayout->addWidget(evaluationFrame , 1, 0 );
   //widgetLayout->addWidget(gridsearchFrame , 1, 1 );
   widgetLayout->addWidget(calibrationFrame, 1, 1 );
   loadSettings();
}

AF4SLSEvaluationWidget::~AF4SLSEvaluationWidget()
{
   writeSettings();
}

void AF4SLSEvaluationWidget::writeSettings() const
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("slsEvaluation/evaluation/n0"), refIndexSolvent->value());
   settings.setValue(tr("slsEvaluation/evaluation/laserWaveLength"), laserWaveLength->value());
   settings.setValue(tr("slsEvaluation/evaluation/concentrationCut"), concentrationCut->value());
}

#ifndef CHECK_SETTINGS_CONVERSION
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };
#endif // CHECK_SETTINGS_CONVERSION

void AF4SLSEvaluationWidget::loadSettings() const
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   //QString stringValue;
   double value;
   //double signValue;
   //int expValue;
   //bool boolValue;
   bool ok;

   value = settings.value(tr("slsEvaluation/evaluation/n0"), "1.3332").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("slsEvaluation/evaluation/n0"), "1.3332");
   refIndexSolvent->setValue(value);

   value = settings.value(tr("slsEvaluation/evaluation/laserWaveLength"), "632.8").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("slsEvaluation/evaluation/laserWaveLength"), "632.8");
   laserWaveLength->setValue(value);

   value = settings.value(tr("slsEvaluation/evaluation/concentrationCut"), "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("slsEvaluation/evaluation/concentrationCut"), "1.0");
   //signValue = FFFTwoBoxWidget::calcSignificand(value, &expValue);
   //concentrationCut->setValue(signValue, expValue, 520);
   concentrationCut->setValueM(value, 520);
}
#undef CHECK_SETTINGS_CONVERSION


void AF4SLSEvaluationWidget::startEvaluation()
{
   // Check File Names:
   QString fileName = fileWidget->getInputFilePath(false);
   if(!QFile::exists(fileName)){
      AF4Log::logWarning(tr("Input File %1 does not exist!").arg(fileName));
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
   AF4Log::logText(tr("Data found."));
   // Parse File
   AF4CsvParser parser(fileName.toStdString(), ',', '.');
   bool ok;
   int errorCode;
   uint errorInLine;
   errorCode = parser.parseFile(&errorInLine);
   if(errorCode){
      AF4Log::logError(tr("Error %1").arg(errorCode));
      AF4Log::logError(tr("Diffusion Evaluation Aborted while reading the input file."));
      QApplication::restoreOverrideCursor();
      return;
   } else AF4Log::logText(tr("File %1 read").arg(fileName));

   vector<string> headLines = std::move(parser.getHeadLines());
   matD data = std::move(parser.getData());

   AF4Log::logText(tr("File parsed."));

   uint timeIndex(0);
   uint diffCoeffIndex(1);
   uint rSIndex(2);
   vector<uint> mallsIndices = {3, 4, 5, 6, 7, 8, 9, 10};
   uint uvVisIndex = 11;
   uint riIndex = 12;

   vecD times = data.at(timeIndex);
   vecD diffCoeffs = data.at(diffCoeffIndex);
   vecD stokesRadii = data.at(rSIndex);
   vecD uvVisData = data.at(uvVisIndex);
   vecD riData = data.at(riIndex);


   matD mallsData(NUMBER_OF_DETECTORS);
   for(uint i = 0; i < NUMBER_OF_DETECTORS; ++i) mallsData.at(i) = vecD(data.at(i+3));
   vector<bool> useDataPoint = vector<bool>(riData.size(), true);
   vecD dnDcs = vecD(riData.size(), peakParameterFrame->getRefIndexIncs().at(0));
   vecD mallsCalibConstants = calibrationFrame->getMallsAngleConstants().toVector().toStdVector();
   vector<bool> useAngleFlags = calibrationFrame->getUsedAngles().toVector().toStdVector();
   bool stokesRadiusChosen(true);


   matD rayleighRatios(mallsData);


   //bool raylefighRatiosinFile = true;
   if(calibrationFrame->getVoltSignalChecked()){
      //qDebug() << "recalcRayleigh";
      for(uint i = 0; i < rayleighRatios.size(); ++i){
         //qDebug() << "recalcRayleigh" << calibrationFrame->getMallsConstant() << mallsCalibConstants[i] ;
         double constant = 2 * calibrationFrame->getMallsConstant() * mallsCalibConstants[i];
         for(double &R : rayleighRatios[i]){
            R *= constant;
            //if(i==0) qDebug() << R;
         }
      }

   }

   //for(int i = 0; i < 1500; ++i) qDebug() << rayleighRatios[0][i] << mallsData[0][i];
   //qDebug() << " ";
   //for(int i = 0; i < 10; ++i) qDebug() << rayleighRatios[1][i] << mallsData[1][i];

   /////////////////
   // calculation //
   /////////////////

   AF4SLSEvaluator slsEvaluator;

   if(refIndexConc->isChecked())
      slsEvaluator.evalRiMALLS_partZimmplot(
               riData,
               rayleighRatios,
               useDataPoint,
               dnDcs,
               laserWaveLength->value() / 1e7,     // from nm to cm
               refIndexSolvent->value(),
               calibrationFrame->getRIConstant(), //  riConstant->value(),
               concentrationCut->value(),
               calibrationFrame->getMallsConstant(),
               mallsCalibConstants,
               useAngleFlags
               );
   else if(absorbanceConc->isChecked()){

      vecD epsilon = vecD(riData.size(), peakParameterFrame->getEpsilons().at(0));
            slsEvaluator.evalUVVis_MALLS_partZimmplot(
               uvVisData,
               rayleighRatios,
               useDataPoint,
               epsilon,
               dnDcs,
               laserWaveLength->value() / 1e7,     // from nm to cm
               refIndexSolvent->value(),
               calibrationFrame->getRIConstant(), //  riConstant->value(),
               concentrationCut->value(),
               calibrationFrame->getMallsConstant(),
               mallsCalibConstants,
               useAngleFlags
               );
}

   vecD gyrRadii = slsEvaluator.getRadGyrs(&ok);
   if(!ok){
      AF4Log::logError(tr("Rad of gyr vector does not exist."), true);

      return;
   }
   vecD MWs = slsEvaluator.getMolWeights(&ok);
   if(!ok){
      AF4Log::logError(tr("Mol weights vector does not exist."), true);


      return;
   }
   vecD rhos(gyrRadii.size());
   if(calculateShapeFactor){
      if(stokesRadiusChosen){
         slsEvaluator.calcRho(stokesRadii, gyrRadii, rhos);
      } else {
         AF4Log::logWarning(tr("No column with Stokes radii specified. Shape factor rho cannot be calculated."));
      }
   }

   vector<string> resultHeadLines;
   matD resultData;
   resultHeadLines.push_back(headLines.at(timeIndex));
   resultData.push_back(times);
   resultHeadLines.push_back(headLines.at(diffCoeffIndex));
   resultData.push_back(diffCoeffs);

   resultHeadLines.push_back(string("Molecular Weight [g/mol]"));
   resultData.push_back(MWs);

   resultHeadLines.push_back(string("Radius of Gyration [nm]"));
   resultData.push_back(gyrRadii);
   if(stokesRadiusChosen){
      resultHeadLines.push_back(headLines.at(rSIndex));
      resultData.push_back(data.at(rSIndex));
      if(calculateShapeFactor->isChecked()){
         resultHeadLines.push_back(string("rho"));
         resultData.push_back(rhos);
      }
   }
   //for(int i = 0; i < riIndices.size(); ++i){
   resultHeadLines.push_back(headLines.at(riIndex));
   resultData.push_back(riData);

   vecD concentrations = slsEvaluator.getConcentrations();
   resultHeadLines.push_back(string("Concentrations"));
   resultData.push_back(concentrations);

//}

   //for(int k = 0; k < mallsIndices.size(); ++k){
   for(uint i = 0; i < mallsIndices.size(); ++i){
      resultHeadLines.push_back(headLines.at(mallsIndices.at(i)));
      resultData.push_back(data.at(mallsIndices.at(i)));
   }

   AF4CsvWriter fileWriter(outFileName.toStdString());


   ///// INTERMEDIATE SOLUTION

   matD write_data(resultData.size());
   for(uint i = 0; i < resultData.size(); ++i){
      write_data[i] = vecD(resultData.at(i));
   }

   errorCode = fileWriter.writeFile(write_data, resultHeadLines);

   QApplication::restoreOverrideCursor();
}

void AF4SLSEvaluationWidget::emitConcModeChanged()
{
   if(absorbanceConc->isChecked())    emit concModeChanged(SLSConcMode::FromUVVis);
   else if(refIndexConc->isChecked()) emit concModeChanged(SLSConcMode::FromRI);
}


void AF4SLSEvaluationWidget::logChosenIndices(uint timeIndex,
                                              const QList<posPeakPair> &riIndices,
                                              const QList<QList<posPeakPair> > &mallsIndices,
                                              uint diffCoeffIndex,
                                              uint rSIndex,
                                              bool diffCoeffIndexChosen,
                                              bool rSIndexChosen)
{
   AF4Log::logText(tr("time index: %1").arg(timeIndex));
   if(diffCoeffIndexChosen) AF4Log::logText(tr("diffCoeffIndex: %1").arg(diffCoeffIndex));
   if(rSIndexChosen) AF4Log::logText(tr("diffCoeffIndex: %1").arg(rSIndex));
   for(int i = 0 ; i < riIndices.size(); ++i)
      AF4Log::logText(tr("riIndices: index %1, peak %2, ").arg(riIndices.at(i).position).arg(riIndices.at(i).position));
   for(int k = 0 ; k < mallsIndices.size(); ++k)
      for(int i = 0 ; i < mallsIndices.at(k).size(); ++i)
         AF4Log::logText(tr("mallsIndices[%1]: index %2, peak %3").arg(k).
                         arg(mallsIndices.at(k).at(i).position).arg(mallsIndices.at(k).at(i).peak));
}

void AF4SLSEvaluationWidget::saveParameters() const
{
   writeSettings();
   AF4Log::logText(tr("Parameters of SLS Evaluation saved."));

}
