#include "af4innercalibrationframe.h"

AF4InnerCalibrationFrame::AF4InnerCalibrationFrame(const int channelId,
                                                   const int calibId,
                                                   const QString channelName,
                                                   const QString calibName,
                                                   // QWeakPointer<QPushButton> saveButton,
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
   channelWidthLabel->setText(QString("w_cla / µm"), QwtText::PlainText);
   channelWidthLabel->setToolTip("Channel Width");
   lay->addWidget(channelWidthLabel, 2, 1, Qt::AlignRight);
   channelWidth = new QDoubleSpinBox( this);
   channelWidth->setDecimals(4);
   channelWidth->setMinimum(1.0e-4);
   channelWidth->setMaximum(9.9e3);
   lay->addWidget(channelWidth, 2, 2, Qt::AlignLeft);

   channelWidthGeoLabel = new QwtTextLabel(this);
   //channelWidthGeoLabel->setText(QString("<math><msub><mi>&omega;</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   channelWidthGeoLabel->setText(QString("w_geo / µm"), QwtText::PlainText);
   channelWidthGeoLabel->setToolTip("Channel Width");
   lay->addWidget(channelWidthGeoLabel, 3, 1, Qt::AlignRight);
   channelWidthGeo = new QDoubleSpinBox(this);
   channelWidthGeo->setDecimals(4);
   channelWidthGeo->setMinimum(1.0e-3);
   channelWidthGeo->setMaximum(9.9999e3);
   lay->addWidget(channelWidthGeo, 3, 2, Qt::AlignLeft);

   channelWidthHydroLabel = new QwtTextLabel(this);
   //channelWidthHydroLabel->setText(QString("<math><msub><mi>&omega;</mi><mtext>e</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   channelWidthHydroLabel->setText(QString("w_hyd / µm"), QwtText::PlainText);
   channelWidthHydroLabel->setToolTip("Channel Width");
   lay->addWidget(channelWidthHydroLabel, 4, 1, Qt::AlignRight);
   channelWidthHydro = new QDoubleSpinBox(this);
   channelWidthHydro->setDecimals(4);
   channelWidthHydro->setMinimum(1.0e-4);
   channelWidthHydro->setMaximum(9.9999e3);
   lay->addWidget(channelWidthHydro, 4, 2, Qt::AlignLeft);


   classicalVolumeLabel = new QwtTextLabel(this);
   //classicalVolumeLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   classicalVolumeLabel->setText(QString("V_cla / ml "), QwtText::PlainText);
   classicalVolumeLabel->setToolTip("Channel Volume");
   lay->addWidget(classicalVolumeLabel, 2, 3, Qt::AlignLeft);
   classicalVolume = new QDoubleSpinBox(this);
   classicalVolume->setDecimals(4);
   classicalVolume->setMinimum(1.0e-2);
   classicalVolume->setMaximum(9.9999e0);
   lay->addWidget(classicalVolume, 2, 4, Qt::AlignLeft);

   geometVolumeLabel = new QwtTextLabel(this);
   //geometVolumeLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   geometVolumeLabel->setText(QString("V_geo / ml "), QwtText::PlainText);
   geometVolumeLabel->setToolTip("Channel Volume");
   lay->addWidget(geometVolumeLabel, 3, 3, Qt::AlignLeft);
   geometVolume = new QDoubleSpinBox(this);
   geometVolume->setDecimals(4);
   geometVolume->setMinimum(1.0e-2);
   geometVolume->setMaximum(9.9999e0);
   lay->addWidget(geometVolume, 3, 4, Qt::AlignLeft);

   hydrodynVolumeLabel = new QwtTextLabel(this);
   //hydrodynVolumeLabel->setText(QString("<math><msup><mi>V</mi><mtext>0</mtext></msup><mtext>&nbsp;/&nbsp;min</mtext></math>"), QwtText::MathMLText);
   hydrodynVolumeLabel->setText(QString("V_hyd / ml "), QwtText::PlainText);
   hydrodynVolumeLabel->setToolTip("Channel Volume");
   lay->addWidget(hydrodynVolumeLabel, 4, 3, Qt::AlignLeft);
   hydrodynVolume = new QDoubleSpinBox(this);
   hydrodynVolume->setDecimals(4);
   hydrodynVolume->setMinimum(1.0e-2);
   hydrodynVolume->setMaximum(9.9999e0);
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
   connect(this, &AF4InnerCalibrationFrame::saveButtonClicked, this, &AF4InnerCalibrationFrame::saveSettings);   
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
   QSettings settings("AgCoelfen", "AF4Eval");
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

   QSettings settings("AgCoelfen", "AF4Eval");
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
