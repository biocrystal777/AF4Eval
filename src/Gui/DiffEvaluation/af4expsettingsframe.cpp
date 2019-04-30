#include "af4expsettingsframe.h"

AF4ExpSettingsFrame::AF4ExpSettingsFrame(const QString &prefix, int id, QWidget *parent) :QFrame(parent), prefix(prefix), id(id)
{
    QPixmap pixMap;
    //QString toolTipDescription;
    QLabel* labelPtr;

    this->setFrameStyle(0x1011);
    lay = new QGridLayout(this);

    labelPtr = new QLabel("<b>Experimental FFF settings</b>", this);
    lay->addWidget(labelPtr, 0, 0, 1, 3);

    auto makeSpinBox = [this](QDoubleSpinBox *&spinBox, QString labelString, QString toolTip, int row, int column, int rowSpan, int columnSpan)  {
       QwtTextLabel *label = new QwtTextLabel(this);
       label->setText(labelString, QwtText::PlainText);
       label->setToolTip(toolTip);
       lay->addWidget(label, row, column-1, Qt::AlignRight);
       spinBox = new QDoubleSpinBox(this);
       spinBox->setToolTip(toolTip);
       lay->addWidget(spinBox, row, column, rowSpan, columnSpan);
    };

    auto configSpinBox = [this](QDoubleSpinBox *spinBox, int decimals, double singleStep, double minimum, double maximum){
       spinBox->setDecimals(decimals);
       spinBox->setSingleStep(singleStep);
       spinBox->setMinimum(minimum);
       spinBox->setMaximum(maximum);
    };

    makeSpinBox  (elutionFlow,   "Ve / (ml/min)", "Elution Flow",   2, 1, 1, 2);
    configSpinBox(elutionFlow,    2, 0.1, 0.0, 10);
    makeSpinBox  (crossFlow,     "Vc / (ml/min)", "Cross Flow",     3, 1, 1, 2);
    configSpinBox(crossFlow,      2, 0.1, 0.0, 10);
    makeSpinBox  (leftOffsetTime, "t0 / min",     "left Offset",    4, 1, 1, 2);
    configSpinBox(leftOffsetTime, 3, 0.1, 1e-3, 1e2);
    makeSpinBox  (temperature,    "T / K",        "temperature",    2, 5, 1, 2);
    configSpinBox(temperature,    1, 0.1, 1e-3, 1e2);
    makeSpinBox  (viscosity,      "η / cP",       "viscosity",      3, 5, 1, 2);
    configSpinBox(viscosity,      4, 1e-3, 1e-3, 1e1);
    makeSpinBox  (relFocusPoint,  "z% / %",       "Relative focus position",  4, 5, 1, 2);
    configSpinBox(relFocusPoint,  1, 0.1, 0.0, 1e2);


    useVoidPeakBox = new QCheckBox(this);
    lay->addWidget(useVoidPeakBox, 5, 0, 1, 1);
    voidPeakLabel = new QwtTextLabel(this);
    voidPeakLabel->setText("tvoid / min", QwtText::PlainText);
    voidPeakLabel->setToolTip("void peak time");
    lay->addWidget(voidPeakLabel, 5, 4, 1, 2);

    voidPeakTime = new QDoubleSpinBox(this);    
    voidPeakTime->setToolTip("void peak");
    lay->addWidget(voidPeakTime, 5, 5, 1, 2);
    configSpinBox(voidPeakTime, 1, 0.1, 0, 1e2);

    connect(useVoidPeakBox, &QCheckBox::toggled,
            this, &AF4ExpSettingsFrame::enableVoidPeakTime);
    labelPtr =  new QLabel("Recalculate <i>V</i><sup>0</sup>", this);
    lay->addWidget(labelPtr, 5, 1, 1, 2);

    loadSettings();
}


void AF4ExpSettingsFrame::loadSettings()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");

   auto loadSetting = [this, &settings](QString paramKey, std::function< void (double) > setVal, double defaultVal){
      bool ok;
      double val = settings.value(tr("%1/expSettings/%2/%3").arg(prefix).arg(id).arg(paramKey), defaultVal).toDouble(&ok);
      setVal(val);
      if(!ok) AF4Log::logWarning(tr("%1 could not be found, set to default = %2").arg(paramKey).arg(defaultVal));
   };

   loadSetting("temperature",    [this](double v){ temperature->setValue(v); },  333.3);
   loadSetting("elutionFlow",    [this](double v){ elutionFlow->setValue(v); },  333.3);
   loadSetting("crossFlow",      [this](double v){ crossFlow->setValue(v); },  333.3);
   loadSetting("leftOffsetTime", [this](double v){ leftOffsetTime->setValue(v); },  333.3);
   loadSetting("voidPeakTime",   [this](double v){ voidPeakTime->setValue(v); },  333.3);
   loadSetting("relFocusPoint",   [this](double v){ relFocusPoint->setValue(v); },  333.3);
   loadSetting("viscosity",   [this](double v){ viscosity->setValue(v); },  333.3);

   bool checked = settings.value(tr("%1/expSettings/%2/useManualVoidPeak").arg(prefix).arg(id), "true").toBool();
   checked ? useVoidPeakBox->setCheckState(Qt::Checked) : useVoidPeakBox->setCheckState(Qt::Unchecked);
   this->enableVoidPeakTime(checked);
}

void AF4ExpSettingsFrame::writeSettings()
{
    QSettings settings("AgCoelfen", "AF4Eval");
    settings.setIniCodec("UTF-8");

    settings.setValue(tr("%1/expSettings/%2/temperature").arg(prefix).arg(id), temperature->value());
    settings.setValue(tr("%1/expSettings/%2/elutionFlow").arg(prefix).arg(id), elutionFlow->value());
    settings.setValue(tr("%1/expSettings/%2/crossFlow").arg(prefix).arg(id), crossFlow->value());
    settings.setValue(tr("%1/expSettings/%2/leftOffsetTime").arg(prefix).arg(id), leftOffsetTime->value());
    settings.setValue(tr("%1/expSettings/%2/voidPeakTime").arg(prefix).arg(id), voidPeakTime->value());
    settings.setValue(tr("%1/expSettings/%2/relFocusPoint").arg(prefix).arg(id), relFocusPoint->value());
    settings.setValue(tr("%1/expSettings/%2/viscosity").arg(prefix).arg(id), viscosity->value());
    settings.setValue(tr("%1/expSettings/%2/useManualVoidPeak").arg(prefix).arg(id), useVoidPeakBox->isChecked());
}

void AF4ExpSettingsFrame::enableVoidPeakTime(bool checked)
{
   if(voidPeakTime){      
      voidPeakTime->setEnabled(checked);
      voidPeakLabel->setEnabled(checked);
      emit callEnableVolume(!checked);
   }
}

bool AF4ExpSettingsFrame::setTemperatureVal(double val) const
{
   bool ok(false);
   if(0.0<val && val<400.0){
      temperature->setValue(val);
      ok = true;
   }
   return ok;
}

bool AF4ExpSettingsFrame::setViscosity(double val) const
{
    bool ok(false);
    if(0.0<val && val<400.0){
       viscosity->setValue(val);
       ok = true;
    }
    return ok;
}

bool AF4ExpSettingsFrame::setVoidPeakTimeVal(double val) const
{
    bool ok(false);
    if(0.0<val){
        voidPeakTime->setValue(val);
        ok = true;
    }
    return ok;
}

bool AF4ExpSettingsFrame::setRelFocusPointVal(double val) const
{
    bool ok(false);
    if(0.0<val&& val<100.0 ){
        relFocusPoint->setValue(val);
        ok = true;
    }
    return ok;
}

bool AF4ExpSettingsFrame::setElutionFlowVal(double val) const
{
    bool ok(false);
    if(0.0<val&& val<100.0 ){
       elutionFlow->setValue(val);
       ok = true;
    }
    return ok;
}

bool AF4ExpSettingsFrame::setCrossFlowVal(double val) const
{
    bool ok(false);
    if(0.0<val&& val<100.0 ){
        crossFlow->setValue(val);
        ok = true;
    }
    return ok;
}

bool AF4ExpSettingsFrame::setLeftOffsetTimeVal(double val) const
{
    bool ok(false);
    if(0.0<val&& val<100.0 ){
        leftOffsetTime->setValue(val);
        ok = true;
    }
    return ok;
}

void AF4ExpSettingsFrame::setRecalcVolume(bool setValue)
{
   this->enableVoidPeakTime(setValue);
}

ExpFFFParameters AF4ExpSettingsFrame::getExpFFFParameters()
{
   return ExpFFFParameters{
      getTemperatureVal(),
            getViscosityVal(),
            getVoidPeakTimeVal(),
            getRelFocusPointVal(),
            getElutionFlowVal(),
            getCrossFlowVal(),
            getLeftOffsetTimeVal()
   };
}
