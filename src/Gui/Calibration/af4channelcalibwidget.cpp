#include "af4channelcalibwidget.h"

AF4ChannelCalibWidget::AF4ChannelCalibWidget(const int channelId,
                                             const int calibId,
                                             const QString channelName,
                                             const QString calibName,
                                             QWeakPointer<QPushButton> saveButton,
                                             QWidget *parent) :
   QWidget(parent), channelId(channelId), calibId(calibId),
   channelName(channelName), calibName(calibName)//, saveButton(saveButton)
{
   widgetLayout = new QGridLayout(this);
   widgetFrame = new QFrame(this);
   widgetFrame->setFrameStyle(0x0023);
   widgetLayout->addWidget(widgetFrame, 0, 0, 1, 1);
   frameLayout = new QGridLayout(widgetFrame);

   frameLayout->addWidget(new QLabel("<b>Calibration File</b>", this), 0, 1, 1, 3, Qt::AlignBottom);
   inputFileChooser = new QToolButton(this);
   inputFileChooser->setText("..");
   inputFileChooser->setToolTip("Browse Files");
   connect(inputFileChooser, &QToolButton::clicked, this, &AF4ChannelCalibWidget::chooseInputFile);
   frameLayout->addWidget(inputFileChooser, 1, 0, 1, 1, Qt::AlignRight);
   inputFileName = new QLineEdit(this);

   frameLayout->addWidget(inputFileName, 1, 1, 1, 14);

   QLabel *labelPtr = new QLabel(tr("<b>Notes</b>"), this);
   //labelPtr->setMaximumHeight(50);
   frameLayout->addWidget(labelPtr, 2, 1, Qt::AlignCenter);

   frameLayout->addWidget(new QLabel("Date:", this),   3, 0);
   dateDescr = new QLineEdit(this);
   frameLayout->addWidget(dateDescr,                   3, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Sample:", this), 4, 0);
   sampleDescr = new QLineEdit(this);
   frameLayout->addWidget(sampleDescr,                 4, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Buffer:", this), 5, 0);
   bufferDescr = new QLineEdit(this);
   frameLayout->addWidget(bufferDescr,                 5, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Additional Notes:", this), 7, 0, Qt::AlignTop);
   notesDescr = new QTextEdit(this);
   frameLayout->addWidget(notesDescr, 8, 0, 1, 14);

   loadSettings();
   //-
   // Second column
   //--
   innerCalibFrame = new AF4InnerCalibrationFrame(channelId, calibId, channelName, calibName, saveButton, this);
   connect(innerCalibFrame, &AF4InnerCalibrationFrame::calibrateChannelCalled, this, &AF4ChannelCalibWidget::calibrateChannelCalled);
   frameLayout->addWidget(innerCalibFrame, 3, 5, 3, 4);

   /**************************************
    *
    * third column
    *
    *************************************/
   calibParFrame = new AF4CalibParametersFrame(channelId, calibId, channelName, calibName, saveButton, this);
   frameLayout->addWidget(calibParFrame, 3, 9, 3, 4);
   // connect saveButton
   connect(saveButton.data(), &QPushButton::clicked, this, &AF4ChannelCalibWidget::saveParameters);
}

AF4ChannelCalibWidget::~AF4ChannelCalibWidget()
{
   saveParameters();
}


ChannelDimsFromCalib AF4ChannelCalibWidget::getChannelDimsFromCalib() const
{      
   return ChannelDimsFromCalib {
      getClassicalChannelWidth(),
            getHydrodynVolume(),
            getGeometVolume()
   };
}



bool AF4ChannelCalibWidget::setInputFileName(QString path, bool quoted)
{
   QString testPath = path;
   if(testPath.isEmpty()) return true;
   bool ok(true);
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

ParametersForCalibration AF4ChannelCalibWidget::getParametersForCalibration()
{
   return ParametersForCalibration {
      calibParFrame->getElutionFlow(),
            calibParFrame->getCrossFlow(),
            calibParFrame->getRelFocusPoint(),
            calibParFrame->getLeftOffsetTime(),
            calibParFrame->getVoidPeakTime(),
            calibParFrame->getElutionFlow(),
            calibParFrame->getDiffCoefficient()
   };
}

AllCalibrationParameters AF4ChannelCalibWidget::getAllCalibrationParameters()
{   
   return AllCalibrationParameters {
      calibParFrame->getTemperature(),
            calibParFrame->getElutionFlow(),
            calibParFrame->getCrossFlow(),
            calibParFrame->getRelFocusPoint(),
            calibParFrame->getLeftOffsetTime(),
            calibParFrame->getVoidPeakTime(),
            calibParFrame->getElutionTime(),
            calibParFrame->getDiffCoefficient(),
            innerCalibFrame->getChannelWidth(),
            innerCalibFrame->getHydrodynVolume(),
            innerCalibFrame->getGeometVolume(),
            getDateDescr(),
            getSampleDescr(),
            getBufferDescr(),
            getNotesDescr()
   };
}

void AF4ChannelCalibWidget::setAllCalibrationParameters(const AllCalibrationParameters &p)
{
   calibParFrame->setTemperature(p.temperature);
   calibParFrame->setElutionFlow(p.elutionFlow);
   calibParFrame->setCrossFlow(p.crossFlow);
   calibParFrame->setRelFocusPoint(p.relFocusPoint);
   calibParFrame->setLeftOffsetTime(p.leftOffsetTime);
   calibParFrame->setVoidPeakTime(p.voidPeakTime);
   calibParFrame->setElutionTime(p.elutionTime);
   calibParFrame->setDiffCoefficient(p.diffCoeff);
   innerCalibFrame->setChannelWidthClassical(p.chWidth);
   innerCalibFrame->setHydrodynVolume(p.hydrodynVolume);
   innerCalibFrame->setGeometVolume(p.geometVolume);
   this->setDateDescr(p.date);
   this->setSampleDescr(p.sample);
   this->setBufferDescr(p.buffer);
   this->setNotesDescr(p.additionalNotes);
}

void AF4ChannelCalibWidget::loadSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   //double calibValue;
   QString calibString;
   //bool ok;
   calibString = settings.value(tr("channels/%1/calib/%2/dateDescr").arg(channelId).arg(calibId), "").toString();
   dateDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/bufferDescr").arg(channelId).arg(calibId), "").toString();
   bufferDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/sampleDescr").arg(channelId).arg(calibId), "").toString();
   sampleDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/notesDescr").arg(channelId).arg(calibId), "").toString();
   notesDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), "").toString();
   if(!(this->setInputFileName(calibString))) AF4Log::logWarning(tr("Error while setting calib Value \"calib FileName\" in Constructor."));
}

void AF4ChannelCalibWidget::saveParameters()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("channels/%1/calib/%2/name").arg(channelId).arg(calibId), calibName);
   settings.setValue(tr("channels/%1/calib/%2/sampleDescr").arg(channelId).arg(calibId), QVariant(getSampleDescr()));
   settings.setValue(tr("channels/%1/calib/%2/bufferDescr").arg(channelId).arg(calibId), QVariant(getBufferDescr()));
   settings.setValue(tr("channels/%1/calib/%2/dateDescr").arg(channelId).arg(calibId), QVariant(getDateDescr()));
   settings.setValue(tr("channels/%1/calib/%2/notesDescr").arg(channelId).arg(calibId), QVariant(getNotesDescr()));
   settings.setValue(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), QVariant(this->getInputFilePath(false)));
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
   else AF4Log::logWarning(tr("Chosen input file does not exist."));
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























/**************************************
 *
 *
 *
 * AF4InnerCalibrationFrame
 *
 *
 *
 *************************************/

AF4InnerCalibrationFrame::AF4InnerCalibrationFrame(const int channelId,
                                                   const int calibId,
                                                   const QString channelName,
                                                   const QString calibName,
                                                   QWeakPointer<QPushButton> saveButton,
                                                   QWidget *parent)  :
   QFrame(parent), channelId(channelId), calibId(calibId),
   channelName(channelName), calibName(calibName)
{
   this->setFrameStyle(0x1011);
   lay = new QGridLayout(this);

   QString tip("Relative uncertainty consideration within grid\n\
               (from -u to +u) around value with \"|grid|\"\n\
               points on each side. Warning: may slow down computation\n\
               significantly!"
               );
   checkUncertainties = new QCheckBox(this);
   checkUncertainties->setChecked(false);
   checkUncertainties->setToolTip(tip);

   uncertRangeLabel = new QwtTextLabel(this);
   uncertRangeLabel->setText(QString("u(X) / %"), QwtText::PlainText);
   uncertRangeLabel->setToolTip(tip);

   uncertRange = new QDoubleSpinBox(this);
   uncertRange->setMinimum(0.1);
   uncertRange->setMaximum(99.9);
   uncertRange->setSingleStep(0.1);
   uncertRange->setToolTip(tip);

   uncertGridLabel = new QwtTextLabel(this);
   uncertGridLabel->setText(QString("|grid|"), QwtText::PlainText);
   uncertGridLabel->setToolTip(tip);

   uncertGrid = new QSpinBox(this);
   uncertGrid->setMinimum(1);
   uncertGrid->setMaximum(500);

   lay->addWidget(checkUncertainties, 0, 0, 1, 1, Qt::AlignRight);
   lay->addWidget(uncertRangeLabel,   0, 1, 1, 1, Qt::AlignRight);
   lay->addWidget(uncertRange,        0, 2, 1, 1, Qt::AlignLeft);
   lay->addWidget(uncertGridLabel,    0, 3, 1, 1, Qt::AlignRight);
   lay->addWidget(uncertGrid,         0, 4, 1, 1, Qt::AlignLeft);

   auto enableUncertBlock = [this](bool b){
      uncertRangeLabel->setEnabled(b);
      uncertRange     ->setEnabled(b);
      uncertGridLabel ->setEnabled(b);
      uncertGrid      ->setEnabled(b);
   };
   enableUncertBlock(false);
   connect(checkUncertainties, &QCheckBox::toggled, enableUncertBlock);

   calibButton = new QPushButton("Calibrate", this);
   connect(calibButton, &QToolButton::clicked, this, &AF4InnerCalibrationFrame::calibrateChannelCalled);
   lay->addWidget(calibButton, 1, 0, 1, 5);

   classicMode = new QCheckBox(this);
   classicMode->setChecked(true);
   lay->addWidget(classicMode, 2, 0);
   geoMode = new QCheckBox(this);
   geoMode->setChecked(true);
   lay->addWidget(geoMode, 3, 0);
   hydMode = new QCheckBox(this);
   hydMode->setChecked(true);
   lay->addWidget(hydMode, 4, 0);

   channelWidthLabel = new QwtTextLabel(this);
   //channelWidthLabel->setText(QString("<math><msub><mi>&omega;</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   channelWidthLabel->setText(QString("w_cla / cm"), QwtText::PlainText);
   channelWidthLabel->setToolTip("Channel Width");
   lay->addWidget(channelWidthLabel, 2, 1, Qt::AlignRight);
   channelWidth = new QDoubleSpinBox( this);
   //channelWidth->setMinimum(1.0, -4);
   //channelWidth->setMaximum(9.9999, -1);
   channelWidth->setMinimum(1.0e-4);
   channelWidth->setMaximum(9.9999e-1);
   lay->addWidget(channelWidth, 2, 2, Qt::AlignLeft);

   channelWidthGeoLabel = new QwtTextLabel(this);
   //channelWidthGeoLabel->setText(QString("<math><msub><mi>&omega;</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   channelWidthGeoLabel->setText(QString("w_Hyd / cm"), QwtText::PlainText);
   channelWidthGeoLabel->setToolTip("Channel Width");
   lay->addWidget(channelWidthGeoLabel, 3, 1, Qt::AlignRight);
   channelWidthGeo = new QDoubleSpinBox(this);
   //channelWidth->setMinimum(1.0, -4);
   //channelWidth->setMaximum(9.9999, -1);
   channelWidthGeo->setMinimum(1.0e-4);
   channelWidthGeo->setMaximum(9.9999e-1);
   lay->addWidget(channelWidthGeo, 3, 2, Qt::AlignLeft);

   channelWidthHydroLabel = new QwtTextLabel(this);
   //channelWidthHydroLabel->setText(QString("<math><msub><mi>&omega;</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   channelWidthHydroLabel->setText(QString("w_Geo / cm"), QwtText::PlainText);
   channelWidthHydroLabel->setToolTip("Channel Width");
   lay->addWidget(channelWidthHydroLabel, 4, 1, Qt::AlignRight);
   channelWidthHydro = new QDoubleSpinBox(this);
   //channelWidth->setMinimum(1.0, -4);
   //channelWidth->setMaximum(9.9999, -1);
   channelWidthHydro->setMinimum(1.0e-4);
   channelWidthHydro->setMaximum(9.9999e-1);
   lay->addWidget(channelWidthHydro, 4, 2, Qt::AlignLeft);

   classicalVolumeLabel = new QwtTextLabel(this);
   //classicalVolumeLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   classicalVolumeLabel->setText(QString("V_cla / ml "), QwtText::PlainText);
   classicalVolumeLabel->setToolTip("Channel Volume");
   lay->addWidget(classicalVolumeLabel, 2, 3, Qt::AlignLeft);
   classicalVolume = new QDoubleSpinBox(this);
   classicalVolume->setMinimum(1.0e-2);
   classicalVolume->setMaximum(9.9999e2);
   lay->addWidget(classicalVolume, 2, 4, Qt::AlignLeft);

   geometVolumeLabel = new QwtTextLabel(this);
   //geometVolumeLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   geometVolumeLabel->setText(QString("V_Geo / ml "), QwtText::PlainText);
   geometVolumeLabel->setToolTip("Channel Volume");
   lay->addWidget(geometVolumeLabel, 3, 3, Qt::AlignLeft);
   geometVolume = new QDoubleSpinBox(this);
   geometVolume->setMinimum(1.0e-2);
   geometVolume->setMaximum(9.9999e2);
   lay->addWidget(geometVolume, 3, 4, Qt::AlignLeft);

   hydrodynVolumeLabel = new QwtTextLabel(this);
   //hydrodynVolumeLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   hydrodynVolumeLabel->setText(QString("V_hyd / ml "), QwtText::PlainText);
   hydrodynVolumeLabel->setToolTip("Channel Volume");
   lay->addWidget(hydrodynVolumeLabel, 4, 3, Qt::AlignLeft);
   hydrodynVolume = new QDoubleSpinBox(this);
   hydrodynVolume->setMinimum(1.0e-2);
   hydrodynVolume->setMaximum(9.9999e2);
   lay->addWidget(hydrodynVolume, 4, 4, Qt::AlignLeft);

   auto adaptReadiness = [this](){
      if(classicMode->isChecked() || geoMode->isChecked() || hydMode->isChecked()) calibButton->setEnabled(true);
      else                                                                         calibButton->setEnabled(false);
   };

   auto callCalibModeSettingsChanged = [this](){
      emit calibModeSettingsChanged( this->getCalibModes() );
   };

   connect(classicMode, &QCheckBox::toggled, [this](bool enable){
      channelWidth->        setEnabled(enable);
      channelWidthLabel->   setEnabled(enable);
      classicalVolume->     setEnabled(enable);
      classicalVolumeLabel->setEnabled(enable);
   });
   connect(classicMode, &QCheckBox::toggled, adaptReadiness);
   connect(classicMode, &QCheckBox::toggled, callCalibModeSettingsChanged);

   connect(geoMode, &QCheckBox::toggled, [this](bool enable){
      channelWidthGeo->     setEnabled(enable);
      channelWidthGeoLabel->setEnabled(enable);
      geometVolume->        setEnabled(enable);
      geometVolumeLabel->   setEnabled(enable);
   });
   connect(geoMode, &QCheckBox::toggled, adaptReadiness);
   connect(geoMode, &QCheckBox::toggled, callCalibModeSettingsChanged);

   connect(hydMode, &QCheckBox::toggled, [this](bool enable){
      channelWidthHydro->     setEnabled(enable);
      channelWidthHydroLabel->setEnabled(enable);
      hydrodynVolume->        setEnabled(enable);
      hydrodynVolumeLabel->   setEnabled(enable);
   });
   connect(hydMode, &QCheckBox::toggled, adaptReadiness);
   connect(hydMode, &QCheckBox::toggled, callCalibModeSettingsChanged);

   loadSettings();
   connect(saveButton.data(), &QPushButton::clicked, this, &AF4InnerCalibrationFrame::saveSettings);
}

AF4InnerCalibrationFrame::~AF4InnerCalibrationFrame()
{
   saveSettings();
}

CalibModeSettings AF4InnerCalibrationFrame::getCalibModes() const
{
   return  CalibModeSettings {
      checkUncertainties->isChecked(),
            uncertRange->value(),
            static_cast<uint>(uncertGrid->value()),
            classicMode->isChecked(),
            geoMode->isChecked(),
            hydMode->isChecked()
   };
}

void AF4InnerCalibrationFrame::saveSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");   
   settings.setValue(tr("channels/%1/calib/%2/uncertRange").arg(channelId).arg(calibId),      QVariant(uncertRange->value()));
   settings.setValue(tr("channels/%1/calib/%2/uncertGridSize").arg(channelId).arg(calibId),      QVariant(uncertGrid->value()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidth").arg(channelId).arg(calibId),      QVariant(getChannelWidth()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthGeo").arg(channelId).arg(calibId),   QVariant(getChannelWidthGeo()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthHydro").arg(channelId).arg(calibId), QVariant(getChannelWidthHydro()));
   settings.setValue(tr("channels/%1/calib/%2/classicalVolume").arg(channelId).arg(calibId), QVariant(getClassicalVolume()));
   settings.setValue(tr("channels/%1/calib/%2/hydrodynVolume").arg(channelId).arg(calibId),  QVariant(getHydrodynVolume()));
   settings.setValue(tr("channels/%1/calib/%2/geometVolume").arg(channelId).arg(calibId),    QVariant(getGeometVolume()));
   //settings.setValue(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), QVariant(this->getInputFilePath(true)));
}

void AF4InnerCalibrationFrame::loadSettings()
{

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   double calibValue;
   //QString calibStringValue;
   bool ok;
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
}\
};
   calibValue = settings.value(tr("channels/%1/calib/%2/uncertRange").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../uncertRange", "0.0e0");
   this->uncertRange->setValue(calibValue);

   int calibValInt = settings.value(tr("channels/%1/calib/%2/uncertGridSize").arg(channelId).arg(calibId), "").toInt(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../uncertGridSize", "0.0e0");
   this->uncertGrid->setValue(calibValInt);

   calibValue = settings.value(tr("channels/%1/calib/%2/channelWidth").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../channelWidth", "0.0e0");
   this->setChannelWidthClassical(calibValue);

   calibValue = settings.value(tr("channels/%1/calib/%2/channelWidthGeo").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../channelWidthGeo", "0.0e0");
   this->setChannelWidthGeo(calibValue);

   calibValue = settings.value(tr("channels/%1/calib/%2/channelWidthHydro").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../channelWidthHydro", "0.0e0");
   this->setChannelWidthHydro(calibValue);
      //AF4Log::logWarning(tr("Error while setting calib Value (channelWidth, %1, %2).")
      //                   .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/classicalVolume").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../classicalVolume", "0.0e0");
   if(!(this->setClassicalVolume(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (classicalVolume, %1, %2).")
                         .arg(channelName).arg(calibName));


   calibValue = settings.value(tr("channels/%1/calib/%2/hydrodynVolume").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../hydrodynVolume", "0.0e0");
   if(!(this->setHydrodynVolume(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (hydrodynVolume, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/geometVolume").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../hydrodynVolume", "0.0e0");
   if(!(this->setGeometVolume(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (geometVolume, %1, %2).")
                         .arg(channelName).arg(calibName));

#undef CHECK_SETTINGS_CONVERSION //

}

















/**************************************
 *
 *
 *
 * AF4CalibParametersFrame
 *
 *
 *
 *************************************/


AF4CalibParametersFrame::AF4CalibParametersFrame(int channelId, int calibId,
                                                 const QString &channelName,
                                                 const QString &calibName,
                                                 QWeakPointer<QPushButton> saveButton,
                                                 QWidget *parent) :
   QFrame(parent), channelId(channelId), calibId(calibId),
   channelName(channelName), calibName(calibName)
{

   lay = new QGridLayout(this);
   this->setFrameStyle(0x1011);
   /**************************************
    *
    * first column
    *
    *************************************/

   QwtTextLabel *label = new QwtTextLabel(this);
   // qwtLabel->setText(QString("<math><mi>T</mi><mtext>&nbsp;/&nbsp;K</mtext></math>"), QwtText::MathMLText);
   label->setText(QString("T / K"), QwtText::PlainText);
   label->setToolTip("Temperature");
   lay->addWidget(label, 0, 0, Qt::AlignLeft);
   temperature = new QDoubleSpinBox(this);
   temperature->setToolTip("Temperature");
   temperature->setDecimals(1);
   temperature->setSingleStep(0.1);
   temperature->setMaximum(1000.0);
   temperature->setMinimum(0.001);
   temperature->setMaximumWidth(100);
   lay->addWidget(temperature, 0, 1, 1, 2);


   label = new QwtTextLabel(this);
   // qwtLabel->setText(QString("<math><msub><mi>V</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;ml/min</mtext></math>"), QwtText::MathMLText);
   label->setText(QString("Ve / (ml/min)"), QwtText::PlainText);
   label->setToolTip("Elution Flow");
   lay->addWidget(label, 1, 0,Qt::AlignLeft);
   elutionFlow = new QDoubleSpinBox(this);
   elutionFlow->setToolTip("Elution Flow");
   elutionFlow->setDecimals(3);
   elutionFlow->setSingleStep(0.1);
   elutionFlow->setMaximum(100.0);
   elutionFlow->setMinimum(0.001);
   elutionFlow->setMaximumWidth(100);
   lay->addWidget(elutionFlow, 1, 1, 1, 2);

   label = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><msub><mi>V</mi><mtext>c</mtext></msub><mtext>&nbsp;/&nbsp;ml/min</mtext></math>"), QwtText::MathMLText);
   label->setText(QString("Vc / (ml/min)"), QwtText::PlainText);
   label->setToolTip("Cross Flow");
   lay->addWidget(label, 2, 0, Qt::AlignLeft);
   crossFlow = new QDoubleSpinBox(this);
   crossFlow->setToolTip("Cross Flow");
   crossFlow->setDecimals(3);
   crossFlow->setSingleStep(0.1);
   crossFlow->setMaximum(100.0);
   crossFlow->setMinimum(0.001);
   crossFlow->setMaximumWidth(100);
   lay->addWidget(crossFlow, 2, 1, 1, 2);

   label = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><msub><mi>z</mi><mtext>%</mtext></msub><mtext>&nbsp;/&nbsp;%</mtext></math>"), QwtText::MathMLText);
   label->setText(QString("z% / %"), QwtText::PlainText);
   label->setToolTip("Relative position of the focussing point\ncompared to the length of the channel");
   lay->addWidget(label, 3, 0, Qt::AlignLeft);
   relFocusPoint = new QDoubleSpinBox(this);
   relFocusPoint->setToolTip("Relative position of the focussing point\ncompared to the length of the channel");
   relFocusPoint->setDecimals(1);
   relFocusPoint->setSingleStep(0.1);
   relFocusPoint->setMaximum(100.0);
   relFocusPoint->setMinimum(0.1);
   relFocusPoint->setMaximumWidth(100);
   lay->addWidget(relFocusPoint, 3, 1, 1, 2);
   //qDebug() << "2";

    /**************************************
    *
    * second column
    *
    *************************************/

   label = new QwtTextLabel(this);
   // qwtLabel->setText(QString("<math><msub><mi>t</mi><mtext>off</mtext></msub><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   label->setText(QString("t_off / min"), QwtText::PlainText);
   //label->setMinimumWidth(label->size().width() * 1.0);
   label->setToolTip("Left offset time");
   lay->addWidget(label, 0, 3, Qt::AlignLeft);
   leftOffsetTime = QSharedPointer<QDoubleSpinBox>(new QDoubleSpinBox(this));
   leftOffsetTime->setToolTip("Left offset time");
   leftOffsetTime->setDecimals(3);
   leftOffsetTime->setSingleStep(0.005);
   leftOffsetTime->setMaximum(30.0);
   leftOffsetTime->setMinimum(0.000);
   leftOffsetTime->setMaximumWidth(100);
   lay->addWidget(leftOffsetTime.data(), 0, 4, 1, 2);

   label = new QwtTextLabel(this);
   label->setText(QString("t_void / min"), QwtText::PlainText);
   label->setToolTip("Void Peak Time");
   lay->addWidget(label, 1, 3, Qt::AlignLeft);
   voidPeakTime = QSharedPointer<QDoubleSpinBox>(new QDoubleSpinBox(this));
   voidPeakTime->setToolTip("Void Peak Time");
   voidPeakTime->setDecimals(3);
   voidPeakTime->setSingleStep(0.005);
   voidPeakTime->setMaximum(30.0);
   voidPeakTime->setMinimum(0.00001);
   voidPeakTime->setMaximumWidth(100);
   lay->addWidget(voidPeakTime.data(), 1, 4, 1, 2);

   label = new QwtTextLabel(this);
   label->setText(QString("t_e / min"), QwtText::PlainText);
   label->setToolTip("Elution Time");
   lay->addWidget(label, 2, 3, Qt::AlignLeft);
   elutionTime = QSharedPointer<QDoubleSpinBox>(new QDoubleSpinBox(this));
   elutionTime->setToolTip("Elution Time");
   elutionTime->setDecimals(3);
   elutionTime->setSingleStep(0.05);
   elutionTime->setMaximum(300.0);
   elutionTime->setMinimum(0.00001);
   elutionTime->setMaximumWidth(100);
   lay->addWidget(elutionTime.data(), 2, 4, 1, 2);

   /**************************************
   *
   * last line with DiffCoeffBox
   *
   *************************************/

   diffCoeffCalcButton = new QToolButton(this);
   diffCoeffCalcButton->setText("D / (cm²/s)");
   connect(diffCoeffCalcButton, &QToolButton::clicked, this, &AF4CalibParametersFrame::callDiffCoeffDialog);
   lay->addWidget(diffCoeffCalcButton, 5, 0, 1, 1);

   diffCoefficient = new AF4SciNotSpinBox("Diffusion Coefficient", this);
   diffCoefficient->setMinimum(1.0000e-15);
   diffCoefficient->setMaximum(9.99999e2);
   lay->addWidget(diffCoefficient, 5, 1, 1, 3);

   loadSettings();
   connect(saveButton.data(), &QPushButton::clicked, this, &AF4CalibParametersFrame::saveSettings);
}

AF4CalibParametersFrame::~AF4CalibParametersFrame()
{
   saveSettings();
}

void AF4CalibParametersFrame::callDiffCoeffDialog()
{
   if(!diffCoeffCalcWidget){
      diffCoeffCalcButton->setDown(true);
      diffCoeffCalcButton->clearFocus();
      diffCoeffCalcWidget = new AF4StokesEinsteinCalculatorWidget(this);
      connect(diffCoeffCalcWidget, &AF4StokesEinsteinCalculatorWidget::newDiffCoeff, diffCoefficient, &AF4SciNotSpinBox::setValue);
      diffCoeffCalcWidget->show();

      diffCoeffCalcWidget->setFocus();
      connect(diffCoeffCalcWidget, &AF4StokesEinsteinCalculatorWidget::finished, this, [this]() {
         diffCoeffCalcButton->setDown(false);
         diffCoeffCalcWidget =  nullptr;
      } );
   }
}

void AF4CalibParametersFrame::saveSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("channels/%1/calib/%2/diffCoefficient").arg(channelId).arg(calibId), QVariant(getDiffCoefficient()));
   settings.setValue(tr("channels/%1/calib/%2/crossFlow").arg(channelId).arg(calibId),       QVariant(getCrossFlow()));
   settings.setValue(tr("channels/%1/calib/%2/temperature").arg(channelId).arg(calibId),     QVariant(getTemperature()));
   settings.setValue(tr("channels/%1/calib/%2/elutionFlow").arg(channelId).arg(calibId),     QVariant(getElutionFlow()));
   settings.setValue(tr("channels/%1/calib/%2/leftOffsetTime").arg(channelId).arg(calibId),  QVariant(getLeftOffsetTime()));
   settings.setValue(tr("channels/%1/calib/%2/voidPeakTime").arg(channelId).arg(calibId),    QVariant(getVoidPeakTime()));
   settings.setValue(tr("channels/%1/calib/%2/relFocusPoint").arg(channelId).arg(calibId),   QVariant(getRelFocusPoint()));
   settings.setValue(tr("channels/%1/calib/%2/elutionTime").arg(channelId).arg(calibId),     QVariant(getElutionTime()));
}

void AF4CalibParametersFrame::adaptEnablingStatus(CalibModeSettings m)
{
   uint mode{0x00000000};
   if (m.classical)    mode += 0x000000001;
   if (m.geometric)    mode += 0x000000010;
   if (m.hydrodynamic) mode += 0x000000100;

   switch (mode) {
   case 0x00000000:

      break;
   case 0x00000001:

      break;
   case 0x00000010:

      break;
   case 0x00000011:

      break;
   case 0x00000100:

      break;
   case 0x00000101:

      break;
   case 0x00000110:

      break;
   case 0x00000111:

      break;
   //default: break;
   }
}

void AF4CalibParametersFrame::loadSettings()
{

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   double calibValue;
   //QString calibStringValue;
   bool ok;

#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
}\
};

   calibValue = settings.value(tr("channels/%1/calib/%2/diffCoefficient").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../diffCoefficient", "0.0e0");
   if(!(this->setDiffCoefficient(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (diffusion Coefficient, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/crossFlow").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../crossFlow", "0.0e0");
   if(!(this->setCrossFlow(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (crossFlow, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/relFocusPoint").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../relFocusPoint", "0.0e0");
   if(!(this->setRelFocusPoint(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (relFocusPoint, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/temperature").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../temperature", "0.0e0");
   if(!(this->setTemperature(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (temperature, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/elutionFlow").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../elutionFlow", "0.0e0");
   if(!(this->setElutionFlow(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (elutionFlow, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/leftOffsetTime").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../leftOffsetTime", "1.0e0");
   if(!(this->setLeftOffsetTime(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (leftOffsetTime t0, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/voidPeakTime").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../voidPeakTime", "1.0e0");
   if(!(this->setVoidPeakTime(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (voidPeakTime, %1, %2).")
                         .arg(channelName).arg(calibName));

   calibValue = settings.value(tr("channels/%1/calib/%2/elutionTime").arg(channelId).arg(calibId), "").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/.../calib/.../elutionTime", "1.0e0");
   if(!(this->setElutionTime(calibValue)))
      AF4Log::logWarning(tr("Error while setting calib Value (elutionTime, %1, %2).")
                         .arg(channelName).arg(calibName));

#undef CHECK_SETTINGS_CONVERSION //
}
