#include "af4channelcalibwidget.h"

AF4ChannelCalibWidget::AF4ChannelCalibWidget(const int channelId,
                                             const int calibId,
                                             const QString channelName,
                                             const QString calibName,
                                             const bool loadParameters,
                                             QWidget *parent) :
   QWidget(parent), channelId(channelId), calibId(calibId),
   channelName(channelName), calibName(calibName)
{

   widgetLayout = new QGridLayout(this);
   widgetFrame = new QFrame(this);
   widgetFrame->setFrameStyle(0x0023);
   widgetLayout->addWidget(widgetFrame, 0, 0, 1, 1);
   frameLayout = new QGridLayout(widgetFrame);

   frameLayout->addWidget(new QLabel("<b>Calibration File</b>", this), 0, 5, 1, 3, Qt::AlignBottom);
   inputFileChooser = new QToolButton(this);
   inputFileChooser->setText("..");
   inputFileChooser->setToolTip("Browse Files");
   QObject::connect(inputFileChooser, SIGNAL(clicked()), this, SLOT(chooseInputFile()));
   frameLayout->addWidget(inputFileChooser, 1, 4, 1, 1);
   inputFileName = new QLineEdit(this);

   frameLayout->addWidget(inputFileName, 1, 5, 1, 9);

   QLabel *labelPtr = new QLabel(tr("<b>Notes</b>"), this);
   labelPtr->setMaximumHeight(50);
   frameLayout->addWidget(labelPtr, 0, 1, Qt::AlignCenter);

   frameLayout->addWidget(new QLabel("Date:", this), 2, 0);
   dateDescr = new QLineEdit(this);
   frameLayout->addWidget(dateDescr, 2, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Sample:", this), 3, 0);
   sampleDescr = new QLineEdit(this);
   frameLayout->addWidget(sampleDescr, 3, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Buffer:", this), 4, 0);
   bufferDescr = new QLineEdit(this);
   frameLayout->addWidget(bufferDescr, 4, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Additional Notes:", this), 7, 0, Qt::AlignTop);
   notesDescr = new QTextEdit(this);
   frameLayout->addWidget(notesDescr, 8, 0, 1, 14);


   QwtTextLabel* qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString("<math><mi>D</mi><mtext>&nbsp; /&nbsp; cm²/s </mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("D / cm²/s"), QwtText::PlainText);
   qwtLabel->setToolTip("Diffusion coefficient");
   frameLayout->addWidget(qwtLabel, 2, 5,Qt::AlignLeft);
   diffCoefficient = new FFFTwoBoxWidget("Diffusion Coefficient", widgetFrame);
   diffCoefficient->setMinimum(1.0000e-15);
   diffCoefficient->setMaximum(9.99999e2);
   frameLayout->addWidget(diffCoefficient, 2, 6, 1, 2);


   diffCoeffCalculator = new QToolButton(this);
   diffCoeffCalculator->setText("D from R");
   QObject::connect(diffCoeffCalculator, SIGNAL(clicked()), this, SLOT(callDiffCoeffDialog()));
   frameLayout->addWidget(diffCoeffCalculator, 2, 8, 1, 1);


   calibrationFrame = new QFrame(this);
   calibrationFrame->setFrameStyle(0x1011);
   calibrationFrameLayout = new QGridLayout(calibrationFrame);

   calibButton = new QPushButton("Calibrate", widgetFrame);
   QObject::connect(calibButton, SIGNAL(clicked()), this, SLOT(callCalibrateChannel()));
   calibrationFrameLayout->addWidget(calibButton, 0, 0, 1, 3);

   qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("<math><msub><mi>&omega;</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("ω / cm"), QwtText::PlainText);
   qwtLabel->setToolTip("Channel Width");
   calibrationFrameLayout->addWidget(qwtLabel, 1, 0, Qt::AlignLeft);
   channelWidth = new FFFTwoBoxWidget("Channel Width", widgetFrame);
   //channelWidth->setMinimum(1.0, -4);
   //channelWidth->setMaximum(9.9999, -1);
   channelWidth->setMinimum(1.0e-4);
   channelWidth->setMaximum(9.9999e-1);
   calibrationFrameLayout->addWidget(channelWidth, 1, 1);



   qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("V0 / ml "), QwtText::PlainText);
   qwtLabel->setToolTip("Channel Volume");
   calibrationFrameLayout->addWidget(qwtLabel, 2, 0, Qt::AlignLeft);
   hydrodynVolume = new QDoubleSpinBox(widgetFrame);
   hydrodynVolume->setMinimum(1.0e-2);
   hydrodynVolume->setMaximum(9.9999e2);
   calibrationFrameLayout->addWidget(hydrodynVolume, 2, 1);

   qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("Vg / ml "), QwtText::PlainText);
   qwtLabel->setToolTip("Channel Volume");
   calibrationFrameLayout->addWidget(qwtLabel, 3, 0, Qt::AlignLeft);
   geometVolume = new QDoubleSpinBox(widgetFrame);
   geometVolume->setMinimum(1.0e-2);
   geometVolume->setMaximum(9.9999e2);
   calibrationFrameLayout->addWidget(geometVolume, 3, 1);

   frameLayout->addWidget(calibrationFrame, 3, 5, 3, 4);


   /**************************************
    *
    * third column
    *
    *************************************/

   qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.0);


   qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString("<math><mi>T</mi><mtext>&nbsp;/&nbsp;K</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("T / K"), QwtText::PlainText);
   qwtLabel->setToolTip("Temperature");
   frameLayout->addWidget(qwtLabel, 2, 9, Qt::AlignLeft);
   temperature = new QDoubleSpinBox(widgetFrame);
   temperature->setToolTip("Temperature");
   temperature->setDecimals(1);
   temperature->setSingleStep(0.1);
   temperature->setMaximum(1000.0);
   temperature->setMinimum(0.001);
   temperature->setMaximumWidth(100);
   frameLayout->addWidget(temperature, 2, 10, 1, 2);


   qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString("<math><msub><mi>V</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;ml/min</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("Ve / (ml/min)"), QwtText::PlainText);
   qwtLabel->setToolTip("Elution Flow");
   frameLayout->addWidget(qwtLabel, 3, 9,Qt::AlignLeft);
   elutionFlow = new QDoubleSpinBox(widgetFrame);
   elutionFlow->setToolTip("Elution Flow");
   elutionFlow->setDecimals(3);
   elutionFlow->setSingleStep(0.1);
   elutionFlow->setMaximum(100.0);
   elutionFlow->setMinimum(0.001);
   elutionFlow->setMaximumWidth(100);
   frameLayout->addWidget(elutionFlow, 3, 10, 1, 2);


   qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("<math><msub><mi>V</mi><mtext>c</mtext></msub><mtext>&nbsp;/&nbsp;ml/min</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("Vc / (ml/min)"), QwtText::PlainText);
   qwtLabel->setToolTip("Cross Flow");
   frameLayout->addWidget(qwtLabel, 4, 9, Qt::AlignLeft);
   crossFlow = new QDoubleSpinBox(widgetFrame);
   crossFlow->setToolTip("Cross Flow");
   crossFlow->setDecimals(3);
   crossFlow->setSingleStep(0.1);
   crossFlow->setMaximum(100.0);
   crossFlow->setMinimum(0.001);
   crossFlow->setMaximumWidth(100);
   frameLayout->addWidget(crossFlow, 4, 10, 1, 2);

   qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("<math><msub><mi>z</mi><mtext>%</mtext></msub><mtext>&nbsp;/&nbsp;%</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("z% / %"), QwtText::PlainText);
   qwtLabel->setToolTip("Relative position of the focussing point\ncompared to the length of the channel");
   frameLayout->addWidget(qwtLabel, 5, 9, Qt::AlignLeft);
   relFocusPoint = new QDoubleSpinBox(widgetFrame);
   relFocusPoint->setToolTip("Relative position of the focussing point\ncompared to the length of the channel");
   relFocusPoint->setDecimals(1);
   relFocusPoint->setSingleStep(0.1);
   relFocusPoint->setMaximum(100.0);
   relFocusPoint->setMinimum(0.1);
   relFocusPoint->setMaximumWidth(100);
   frameLayout->addWidget(relFocusPoint, 5, 10, 1, 2);

    /**************************************
    *
    * Fourth column
    *
    *************************************/

   qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString("<math><msub><mi>t</mi><mtext>off</mtext></msub><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("t_off / min"), QwtText::PlainText);
   qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.0);
   qwtLabel->setToolTip("Left offset time");
   frameLayout->addWidget(qwtLabel, 2, 11, Qt::AlignLeft);
   leftOffsetTime = new QDoubleSpinBox(widgetFrame);
   leftOffsetTime->setToolTip("Left offset time");
   leftOffsetTime->setDecimals(3);
   leftOffsetTime->setSingleStep(0.005);
   leftOffsetTime->setMaximum(30.0);
   leftOffsetTime->setMinimum(0.000);
   leftOffsetTime->setMaximumWidth(100);
   frameLayout->addWidget(leftOffsetTime, 2, 12, 1, 2);


   qwtLabel = new QwtTextLabel(widgetFrame);
   qwtLabel->setText(QString("t_void / min"), QwtText::PlainText);
   qwtLabel->setToolTip("Void Peak Time");
   frameLayout->addWidget(qwtLabel, 3, 11, Qt::AlignLeft);
   voidPeakTime = new QDoubleSpinBox(widgetFrame);
   voidPeakTime->setToolTip("Void Peak Time");
   voidPeakTime->setDecimals(3);
   voidPeakTime->setSingleStep(0.005);
   voidPeakTime->setMaximum(30.0);
   voidPeakTime->setMinimum(0.00001);
   voidPeakTime->setMaximumWidth(100);
   frameLayout->addWidget(voidPeakTime, 3, 12, 1, 2);

   qwtLabel = new QwtTextLabel(widgetFrame);
   qwtLabel->setText(QString("t_e / min"), QwtText::PlainText);
   qwtLabel->setToolTip("Elution Time");
   frameLayout->addWidget(qwtLabel, 4, 11, Qt::AlignLeft);
   elutionTime = new QDoubleSpinBox(widgetFrame);
   elutionTime->setToolTip("Elution Time");
   elutionTime->setDecimals(3);
   elutionTime->setSingleStep(0.005);
   elutionTime->setMaximum(300.0);
   elutionTime->setMinimum(0.00001);
   elutionTime->setMaximumWidth(100);
   frameLayout->addWidget(elutionTime, 4, 12, 1, 2);



   if(loadParameters) loadSettings();
   //else defaultInit();

   /**************************************
   *
   * Fifth column / plot
   *
   *************************************/


   const QRect rec = QApplication::desktop()->availableGeometry();
   const uint screenWidth  = rec.width();
   const uint screenHeight = rec.height();



   plotWidget = new AF4CalibPlotWidget(QString("Calibration signal (t_void region)"), this);
   plotWidget->setMinimumWidth(screenWidth/10*4);
   plotWidget->setMaximumWidth(screenWidth/10*4);
   plotWidget->setMinimumHeight(screenHeight/20*5);
   plotWidget->setMaximumHeight(screenHeight/10*5);
   frameLayout->addWidget(plotWidget,0,15,8,4);


   plotWidget2 = new AF4CalibPlotWidget(QString("Calibration signal (t_e region)"), this);
   plotWidget2->setMinimumWidth(screenWidth/10*4);
   plotWidget2->setMaximumWidth(screenWidth/10*4);
   plotWidget2->setMinimumHeight(screenHeight/20*5);
   plotWidget2->setMaximumHeight(screenHeight/10*5);
   frameLayout->addWidget(plotWidget2,8,15,8,4);

   QString calibFileName = getInputFilePath(false);

   setPlotDataFromFile();

   plotWidget->initPlot();
   plotWidget->addPlotVLine(leftOffsetTime, QColor(0x00,0x00,0x00));
   plotWidget->addPlotVLine(voidPeakTime, QColor(0x00,0x66,0xFF));

   plotWidget2->initPlot();
   plotWidget2->addPlotVLine(elutionTime, QColor(0xFF, 0x55, 0x00));





}

AF4ChannelCalibWidget::~AF4ChannelCalibWidget()
{
   saveParameters();
}


bool AF4ChannelCalibWidget::setDiffCoefficient(double value)
{
   int valExponent;
   double valSignificand = FFFTwoBoxWidget::calcSignificand(value, &valExponent);
   if ((1.0  <= valSignificand) && (valSignificand < 10.0)){
      diffCoefficient->setValueS(valSignificand, valExponent, 1);
      return true;
   } else {
      return false;
   }
}

void AF4ChannelCalibWidget::callCalibrateChannel()
{
   calibrateChannelCalled();
}

void AF4ChannelCalibWidget::setPlotDataFromFile()
{

   QString calibFileName = this->getInputFilePath(false);

   if(!QFile::exists(calibFileName)){
      return;
   }

   // Parse File
   AF4CsvParser parser(calibFileName.toStdString(), ',', '.');
   int errorCode;
   uint errorInLine;
   errorCode = parser.parseFile(&errorInLine);
   if(errorCode){
      FFFLog::logError(tr("Error %1").arg(errorCode), true);
         FFFLog::logError(tr("Diffusion Evaluation Aborted while reading the input file."), true);
         return;
   } else FFFLog::logText(tr("File %1 read").arg(calibFileName));
   parser.getData();
   std::vector<std::string> headLines = parser.getHeadLines();
   matD data = parser.getData();

   QVector<double> plotX = QVector<double>::fromStdVector( data[0] );


   QVecMatrix<double> plotY;
   plotY.resize(data.size() -1);
   for(int i = 0; i < plotY.size(); ++i){
      plotY[i] = QVector<double>::fromStdVector( data[i+1] );
      // adjust plot Signals to non-negative values:
      double minY = *(std::min_element(plotY[i].begin(), plotY[i].end()));
      double maxY = *(std::max_element(plotY[i].begin(), plotY[i].end()));
      for(double& v : plotY[i]){
         v -= minY;
         v += 0.01 * fabs(maxY-minY);
      }

   }


   QStringList signalSwitchEntries;
   for(std::string &entry : headLines)
      signalSwitchEntries.append(QString::fromStdString(entry));
   signalSwitchEntries.removeFirst(); // remove entry for time column

   plotWidget->setPlotXData( plotX );
   plotWidget->setPlotYData(plotY);   
   plotWidget->setXScale(5,10);
   plotWidget->autoScaleY1Axis();
   plotWidget->autoScaleY2Axis();
   plotWidget->setSignal1Channels(signalSwitchEntries);
   plotWidget->setSignal2Channels(signalSwitchEntries);

   plotWidget2->setPlotXData( plotX );
   plotWidget2->setPlotYData(plotY);
   plotWidget2->setXScale(8,60);
   plotWidget2->autoScaleY1Axis();
   plotWidget2->autoScaleY2Axis();
   plotWidget2->setSignal1Channels(signalSwitchEntries);
   plotWidget2->setSignal2Channels(signalSwitchEntries);

}

bool AF4ChannelCalibWidget::setChannelWidth(double value)
{
   int valExponent;
   double valSignificand = FFFTwoBoxWidget::calcSignificand(value, &valExponent);
   if ((1.0  <= valSignificand) && (valSignificand < 10.0)){
      channelWidth->setValueS(valSignificand, valExponent, 7);
      return true;
   } else {
      return false;
   }
}


bool AF4ChannelCalibWidget::setInputFileName(QString path, bool quoted)
{
   bool ok(true);
   QString testPath = path;
   chopStringsQuotMarksEntirely(testPath);
   if(!QFile::exists(testPath)){
      path = QDir::homePath();
      path.prepend('"');
      path.append('"');
      ok = false;
   }
   else {
      if(quoted) chopStringsQuotMarksToOne(path);
      else chopStringsQuotMarksEntirely(path);
      inputFileName->setText(path);
   }
   return ok;
}


void AF4ChannelCalibWidget::loadSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   double calibValue;
   QString calibStringValue;
   bool ok;

#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   FFFLog::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
}\
};

   calibValue = settings.value(tr("channels/%1/calib/%2/diffCoefficient").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../diffCoefficient", "0.0e0");
   if(!(this->setDiffCoefficient(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (diffusion Coefficient, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/crossFlow").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../crossFlow", "0.0e0");
   if(!(this->setCrossFlow(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (crossFlow, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/relFocusPoint").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../relFocusPoint", "0.0e0");
   if(!(this->setRelFocusPoint(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (relFocusPoint, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/temperature").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../temperature", "0.0e0");
   if(!(this->setTemperature(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (temperature, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/elutionFlow").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../elutionFlow", "0.0e0");
   if(!(this->setElutionFlow(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (elutionFlow, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/leftOffsetTime").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../leftOffsetTime", "1.0e0");
   if(!(this->setLeftOffsetTime(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (leftOffsetTime t0, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/voidPeakTime").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../voidPeakTime", "1.0e0");
   if(!(this->setVoidPeakTime(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (voidPeakTime, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/elutionTime").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../elutionTime", "1.0e0");
   if(!(this->setElutionTime(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (elutionTime, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibStringValue = settings.value(tr("channels/%1/calib/%2/dateDescr").arg(channelId).arg(calibId), "").toString();


   calibStringValue = settings.value(tr("channels/%1/calib/%2/bufferDescr").arg(channelId).arg(calibId), "").toString();


   calibStringValue = settings.value(tr("channels/%1/calib/%2/sampleDescr").arg(channelId).arg(calibId), "").toString();


   calibStringValue = settings.value(tr("channels/%1/calib/%2/notesDescr").arg(channelId).arg(calibId), "").toString();


   calibValue = settings.value(tr("channels/%1/calib/%2/channelWidth").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../channelWidth", "0.0e0");
   if(!(this->setChannelWidth(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (channelWidth, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/hydrodynVolume").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../hydrodynVolume", "0.0e0");
   if(!(this->setHydrodynVolume(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (hydrodynVolume, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/geometVolume").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../hydrodynVolume", "0.0e0");
   if(!(this->setGeometVolume(calibValue)))
      FFFLog::logWarning(tr("Error while setting calib Value (geometVolume, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibStringValue = settings.value(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), "").toString();
   if(!(this->setInputFileName(calibStringValue)))
      FFFLog::logWarning(tr("Error while setting calib Value \"calib FileName\" in Constructor."));

#undef CHECK_SETTINGS_CONVERSION //
}

void AF4ChannelCalibWidget::saveParameters()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("channels/%1/calib/%2/name").arg(channelId).arg(calibId), calibName);
   settings.setValue(tr("channels/%1/calib/%2/diffCoefficient").arg(channelId).arg(calibId), QVariant(getDiffCoefficient()));
   settings.setValue(tr("channels/%1/calib/%2/crossFlow").arg(channelId).arg(calibId), QVariant(getCrossFlow()));
   settings.setValue(tr("channels/%1/calib/%2/temperature").arg(channelId).arg(calibId), QVariant(getTemperature()));
   settings.setValue(tr("channels/%1/calib/%2/elutionFlow").arg(channelId).arg(calibId), QVariant(getElutionFlow()));
   settings.setValue(tr("channels/%1/calib/%2/leftOffsetTime").arg(channelId).arg(calibId), QVariant(getLeftOffsetTime()));
   settings.setValue(tr("channels/%1/calib/%2/voidPeakTime").arg(channelId).arg(calibId), QVariant(getVoidPeakTime()));
   settings.setValue(tr("channels/%1/calib/%2/relFocusPoint").arg(channelId).arg(calibId), QVariant(getRelFocusPoint()));
   settings.setValue(tr("channels/%1/calib/%2/elutionTime").arg(channelId).arg(calibId), QVariant(getElutionTime()));
   settings.setValue(tr("channels/%1/calib/%2/sampleDescr").arg(channelId).arg(calibId), QVariant(getSampleDescr()));
   settings.setValue(tr("channels/%1/calib/%2/bufferDescr").arg(channelId).arg(calibId), QVariant(getBufferDescr()));
   settings.setValue(tr("channels/%1/calib/%2/dateDescr").arg(channelId).arg(calibId), QVariant(getDateDescr()));
   settings.setValue(tr("channels/%1/calib/%2/notesDescr").arg(channelId).arg(calibId), QVariant(getNotesDescr()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidth").arg(channelId).arg(calibId), QVariant(getChannelWidth()));
   settings.setValue(tr("channels/%1/calib/%2/hydrodynVolume").arg(channelId).arg(calibId), QVariant(getHydrodynVolume()));
   settings.setValue(tr("channels/%1/calib/%2/geometVolume").arg(channelId).arg(calibId), QVariant(getGeometVolume()));
   settings.setValue(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), QVariant(this->getInputFilePath(true)));
}

QString AF4ChannelCalibWidget::getInputFilePath(bool quoted)
{
   QString path = inputFileName->text();
   if(quoted)
      chopStringsQuotMarksToOne(path);
   else
      chopStringsQuotMarksEntirely(path);
   return path;
}

void AF4ChannelCalibWidget::callDiffCoeffDialog()
{
   if(diffCoeffCalcWidget){
      delete diffCoeffCalcWidget; diffCoeffCalcWidget = nullptr;
      return;
   }
   else {
      diffCoeffCalculator->setDown(true);
      diffCoeffCalcWidget = new AF4StokesEinsteinCalculatorWidget(this);
      diffCoeffCalcWidget->show();
      QObject::connect(diffCoeffCalcWidget, SIGNAL(destroyed()), this, SLOT(finishDiffCoeffDialog()));
   }
}

void AF4ChannelCalibWidget::finishDiffCoeffDialog()
{
   diffCoeffCalculator->setDown(false);
   diffCoeffCalcWidget = nullptr;
}

void AF4ChannelCalibWidget::chooseInputFile()
{
   QString s;
   QString oldInputFile = inputFileName->text();
   chopStringsQuotMarksEntirely(oldInputFile);
   if(!QFile::exists(oldInputFile)){
      oldInputFile = QDir::homePath();
   }
   s = QFileDialog::getOpenFileName(this, tr("Choose a File to evaluate"), oldInputFile,
                                    QString(), nullptr,
                                    ( QFileDialog::DontConfirmOverwrite |
                                      QFileDialog::ReadOnly |
                                      QFileDialog::HideNameFilterDetails )
                                    );
   if(QFile::exists(s)) inputFileName->setText(chopStringsQuotMarksToOne(s));
   else FFFLog::logWarning(tr("Chosen input file does not exist."));
}


QString AF4ChannelCalibWidget::chopStringsQuotMarksToOne(QString & string) const
{
   chopStringsQuotMarksEntirely(string);
   string.prepend('\"');
   string.append('\"');
   return string;
}

QString AF4ChannelCalibWidget::chopStringsQuotMarksEntirely(QString & string) const
{
   const QChar quotMark('\"');
   while(!string.isEmpty() && string.at(0) == quotMark)
      string.remove(0, 1);
   while(!string.isEmpty() && string.at(string.length()-1) == quotMark)
      string.remove(string.length()-1, 1);

   return string;
}
