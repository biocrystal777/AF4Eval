#include "af4innercalibrationframe.h"

AF4InnerCalibrationFrame::AF4InnerCalibrationFrame(const int channelId,
                                                   const int calibId,
                                                   const QString channelName,
                                                   const QString calibName,                                                   
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

   classicCalibLine = new AF4InnerCalibResultLine("w_cla / µm", "Channel width", "V_cla / ml",   "Channel Volume", this);
   lay->addWidget(classicCalibLine, 2, 0, 1, 7, Qt::AlignLeft );

   approxGeoCalibLine = new AF4InnerCalibResultLine("w_app / µm", "Channel width", "V_app / ml",   "Channel Volume", this);
   lay->addWidget(approxGeoCalibLine, 3, 0, 1, 7, Qt::AlignLeft );

   geometCalibLine = new AF4InnerCalibResultLine("w_geo / µm", "Channel width", "V_geo / ml",   "Channel Volume", this);
   lay->addWidget(geometCalibLine, 4, 0, 1, 7, Qt::AlignLeft );

   hydrodynCalibLine = new AF4InnerCalibResultLine("w_hyd / µm", "Channel width", "V_hyd / ml",   "Channel Volume", this);
   lay->addWidget(hydrodynCalibLine, 5, 0, 1, 7, Qt::AlignLeft );

   auto adaptReadiness = [this](){
      if(classicCalibLine->isUsed()
            || approxGeoCalibLine->isUsed()
            || geometCalibLine->isUsed()
            || hydrodynCalibLine->isUsed())
         calibButton->setEnabled(true);
      else
         calibButton->setEnabled(false);
   };

   auto callCalibModeSettingsChanged = [this](){
      emit calibModeSettingsChanged( this->getCalibModes() );
   };

   connect(classicCalibLine, &AF4InnerCalibResultLine::useBoxToggled, adaptReadiness);
   connect(classicCalibLine, &AF4InnerCalibResultLine::useBoxToggled, callCalibModeSettingsChanged);

   connect(approxGeoCalibLine, &AF4InnerCalibResultLine::useBoxToggled, adaptReadiness);
   connect(approxGeoCalibLine, &AF4InnerCalibResultLine::useBoxToggled, callCalibModeSettingsChanged);

   connect(geometCalibLine, &AF4InnerCalibResultLine::useBoxToggled, adaptReadiness);
   connect(geometCalibLine, &AF4InnerCalibResultLine::useBoxToggled, callCalibModeSettingsChanged);

   connect(hydrodynCalibLine, &AF4InnerCalibResultLine::useBoxToggled, adaptReadiness);
   connect(hydrodynCalibLine, &AF4InnerCalibResultLine::useBoxToggled, callCalibModeSettingsChanged);

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
            classicCalibLine->isUsed(),
            approxGeoCalibLine->isUsed(),
            geometCalibLine->isUsed(),
            hydrodynCalibLine->isUsed()
   };
}

void AF4InnerCalibrationFrame::saveSettings()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("channels/%1/calib/%2/uncertRange").arg(channelId).arg(calibId),       QVariant(uncertRange->value()));
   settings.setValue(tr("channels/%1/calib/%2/uncertGridSize").arg(channelId).arg(calibId),    QVariant(uncertGrid->value()));

   settings.setValue(tr("channels/%1/calib/%2/channelWidth").arg(channelId).arg(calibId),          QVariant(getChannelWidth()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthGeo").arg(channelId).arg(calibId),       QVariant(getChannelWidthGeo()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthApproxGeo").arg(channelId).arg(calibId), QVariant(getChannelWidthApproxGeo()));
   settings.setValue(tr("channels/%1/calib/%2/channelWidthHydro").arg(channelId).arg(calibId),     QVariant(getChannelWidthHydro()));
   settings.setValue(tr("channels/%1/calib/%2/classicalVolume").arg(channelId).arg(calibId),       QVariant(getClassicalVolume()));
   settings.setValue(tr("channels/%1/calib/%2/approxGeometVolume").arg(channelId).arg(calibId),    QVariant(getApproxGeometVolume()));
   settings.setValue(tr("channels/%1/calib/%2/geometVolume").arg(channelId).arg(calibId),          QVariant(getGeometVolume()));
   settings.setValue(tr("channels/%1/calib/%2/hydrodynVolume").arg(channelId).arg(calibId),        QVariant(getHydrodynVolume()));
}

void AF4InnerCalibrationFrame::loadSettings()
{

   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");

   auto loadSetting = [this, &settings](const QString &paramKey, std::function< bool (double) > setVal, double defaultVal) {
      bool ok;
      QString settingsKey = tr("channels/%1/calib/%2/%3").arg(this->channelId).arg(this->calibId).arg(paramKey);
      double settingsVal = settings.value(settingsKey).toDouble(&ok);
      if(!ok)
         AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2")
                                 .arg(paramKey).arg(defaultVal)
                            );
      if(! setVal(settingsVal) )
         AF4Log::logWarning(tr("Error while setting calibration value in AF4InnerCalibrationFrame (%1, %2, %3).")
                            .arg(paramKey).arg(this->channelName).arg(this->calibName));
   };

   loadSetting("uncertRange",           [this](double v) -> bool { this->uncertRange->setValue(v);    return true;} , 1.0 );
   loadSetting("channelWidth",          [this](double v) -> bool { this->setChannelWidthClassical(v); return true;},  1.0 );
   loadSetting("channelWidthApproxGeo", [this](double v) -> bool { this->setChannelWidthApproxGeo(v); return true;},  1.0 );
   loadSetting("channelWidthGeo",       [this](double v) -> bool { this->setChannelWidthGeo(v);       return true;},  1.0 );
   loadSetting("channelWidthHydro",     [this](double v) -> bool { this->setChannelWidthHydro(v);     return true;},  1.0 );
   loadSetting("classicalVolume",       [this](double v) -> bool { return setClassicalVolume(v);                  },  1.0 );
   loadSetting("approxGeometVolume",    [this](double v) -> bool { return setApproxGeometVolume(v);               },  1.0 );
   loadSetting("geometVolume",          [this](double v) -> bool { return setGeometVolume(v);                     },  1.0 );
   loadSetting("hydrodynVolume",        [this](double v) -> bool { return setHydrodynVolume(v);                   },  1.0 );

   bool ok;
   int calibValInt = settings.value(tr("channels/%1/calib/%2/uncertGridSize").arg(channelId).arg(calibId), "").toInt(&ok);
   if(!ok)
      AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2")
                         .arg("channels/.../calib/.../uncertGridSize").arg("0.0e0"));
   this->uncertGrid->setValue(calibValInt);
}

AF4InnerCalibResultLine::AF4InnerCalibResultLine(
      const QString &wLabelStr,
      const QString &wtoolTipStr,
      const QString &vLabelStr,
      const QString &vtoolTipStr,
      QWidget *parent) : QWidget(parent)//, calibMode(calibMode)
{
   lay = new QHBoxLayout(this);
   useBox = new QCheckBox(this);
   useBox->setChecked(true);
   lay->addWidget(useBox);

   auto makeSpinBox = [this](QDoubleSpinBox *&spinBox, QwtTextLabel *&label, QString labelString, QString toolTip) {
      label = new QwtTextLabel(this);
      label->setText(labelString, QwtText::PlainText);
      label->setToolTip(toolTip);
      lay->addWidget(label, Qt::AlignLeft);
      spinBox = new QDoubleSpinBox(this);
      spinBox->setToolTip(toolTip);
      lay->addWidget(spinBox);
   };

   auto configSpinBox = [this](QDoubleSpinBox *spinBox, int decimals, double singleStep, double minimum, double maximum){
      spinBox->setDecimals(decimals);
      spinBox->setSingleStep(singleStep);
      spinBox->setMinimum(minimum);
      spinBox->setMaximum(maximum);
   };

   makeSpinBox( width, widthLabel, wLabelStr, wtoolTipStr);
   configSpinBox(width, 2, 1e-2, 1e-2, 9.9e3);

   makeSpinBox( volume, volumeLabel, vLabelStr, vtoolTipStr);
   configSpinBox( volume, 4, 1e-4, 1e-4, 9.9e2);

   connect(useBox, &QCheckBox::toggled, [this](bool enable){
      width->      setEnabled(enable);
      widthLabel-> setEnabled(enable);
      volume->     setEnabled(enable);
      volumeLabel->setEnabled(enable);
      emit useBoxToggled(enable);
   });
}

