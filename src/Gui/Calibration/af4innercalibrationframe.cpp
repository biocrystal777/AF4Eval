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

   auto makeSpinBox = [this](QDoubleSpinBox *&spinBox, QwtTextLabel *&label, QString labelString, QString toolTip, int row, int column) {
      label = new QwtTextLabel(this);
      label->setText(labelString, QwtText::PlainText);
      label->setToolTip(toolTip);
      lay->addWidget(label, row, column-1, Qt::AlignLeft);
      spinBox = new QDoubleSpinBox(this);
      spinBox->setToolTip(toolTip);
      lay->addWidget(spinBox, row, column);
   };

   auto configSpinBox = [this](QDoubleSpinBox *spinBox, int decimals, double singleStep, double minimum, double maximum){
      spinBox->setDecimals(decimals);
      spinBox->setSingleStep(singleStep);
      spinBox->setMinimum(minimum);
      spinBox->setMaximum(maximum);
   };

   makeSpinBox(  channelWidth,     channelWidthLabel,       "w_cla / µm",   "Channel width",  2, 2);
   configSpinBox(channelWidth,           2, 1e-2, 1e-2, 9.9e3);
   makeSpinBox(  channelWidthGeo,   channelWidthGeoLabel,   "w_geo / µm",   "Channel width",  3, 2);
   configSpinBox(channelWidthGeo,        2, 1e-2, 1e-2, 9.9e3);
   makeSpinBox(  channelWidthHydro, channelWidthHydroLabel, "w_hyd / µm",   "Channel width",  4, 2);
   configSpinBox(channelWidthHydro,      2, 1e-2, 1e-2, 9.9e3);

   makeSpinBox(  classicalVolume, classicalVolumeLabel,     "V_cla / ml",   "Channel Volume", 2, 4);
   configSpinBox(classicalVolume,        4, 1e-4, 1e-4, 9.9e2);
   makeSpinBox(  geometVolume, geometVolumeLabel,           "V_geo / ml",   "Channel Volume", 3, 4);
   configSpinBox(geometVolume,           4, 1e-4, 1e-4, 9.9e2);
   makeSpinBox(  hydrodynVolume, hydrodynVolumeLabel,       "V_hydro / ml", "Channel Volume", 4, 4);
   configSpinBox(hydrodynVolume,         4, 1e-4, 1e-4, 9.9e2);

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
   settings.setValue(tr("channels/%1/calib/%2/uncertRange").arg(channelId).arg(calibId),       QVariant(uncertRange->value()));
   settings.setValue(tr("channels/%1/calib/%2/uncertGridSize").arg(channelId).arg(calibId),    QVariant(uncertGrid->value()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidth").arg(channelId).arg(calibId),      QVariant(getChannelWidth()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthGeo").arg(channelId).arg(calibId),   QVariant(getChannelWidthGeo()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthHydro").arg(channelId).arg(calibId), QVariant(getChannelWidthHydro()));
   settings.setValue(tr("channels/%1/calib/%2/classicalVolume").arg(channelId).arg(calibId),   QVariant(getClassicalVolume()));
   settings.setValue(tr("channels/%1/calib/%2/geometVolume").arg(channelId).arg(calibId),      QVariant(getGeometVolume()));
   settings.setValue(tr("channels/%1/calib/%2/hydrodynVolume").arg(channelId).arg(calibId),    QVariant(getHydrodynVolume()));
   //settings.setValue(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), QVariant(this->getInputFilePath(true)));
}

void AF4InnerCalibrationFrame::loadSettings()
{

   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   //double calibValue;
   //QString calibStringValue;



   auto loadSetting = [this, &settings](QString paramKey, std::function< bool (double) > setVal, double defaultVal) {
      bool ok;
      QString settingsKey = tr("channels/%1/calib/%2/%3").arg(this->channelId).arg(this->calibId).arg(paramKey);
      double settingsVal = settings.value(settingsKey).toDouble(&ok);
      if(!ok)
         AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2")
                                 .arg(paramKey).arg(defaultVal)
                            );
      if(! setVal(settingsVal) )
         AF4Log::logWarning(tr("Error while setting calib Value (diffusion Coefficient, %1, %2).")
                            .arg(this->channelName).arg(this->calibName));
   };

   loadSetting("uncertRange",       [this](double v) -> bool { this->uncertRange->setValue(v);    return true;} , 1.0 );
   loadSetting("channelWidth",      [this](double v) -> bool { this->setChannelWidthClassical(v); return true;},  1.0 );
   loadSetting("channelWidthGeo",   [this](double v) -> bool { this->setChannelWidthGeo(v);       return true;},  1.0 );
   loadSetting("channelWidthHydro", [this](double v) -> bool { this->setChannelWidthHydro(v);     return true;},  1.0 );
   loadSetting("classicalVolume",   [this](double v) -> bool { return setClassicalVolume(v);                  },  1.0 );
   loadSetting("geometVolume",      [this](double v) -> bool { return setGeometVolume(v);                     },  1.0 );
   loadSetting("hydrodynVolume",    [this](double v) -> bool { return setHydrodynVolume(v);                   },  1.0 );

   bool ok;
   int calibValInt = settings.value(tr("channels/%1/calib/%2/uncertGridSize").arg(channelId).arg(calibId), "").toInt(&ok);
   if(!ok)
      AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2")
                         .arg("channels/.../calib/.../uncertGridSize").arg("0.0e0"));
   this->uncertGrid->setValue(calibValInt);
}
