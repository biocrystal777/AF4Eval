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

    labelPtr = new QLabel(this);
    pixMap = QPixmap(":/images/VE.png");
    pixMap = pixMap.scaledToHeight(16, Qt::SmoothTransformation);
    labelPtr = new QLabel(this);
    labelPtr->setPixmap(pixMap);
    lay->addWidget(labelPtr, 2, 0);
    elutionFlow = new QDoubleSpinBox(this);
    elutionFlow->setToolTip("Elution Flow");
    elutionFlow->setDecimals(2);
    elutionFlow->setMaximum(20);
    elutionFlow->setMinimum(0.01);
    lay->addWidget(elutionFlow, 2, 1, 1, 2);

    labelPtr = new QLabel(this);
    pixMap = QPixmap(":/images/Vc.png");
    pixMap = pixMap.scaledToHeight(16, Qt::SmoothTransformation);
    labelPtr = new QLabel(this);
    labelPtr->setPixmap(pixMap);
    lay->addWidget(labelPtr, 3, 0);
    crossFlow = new QDoubleSpinBox(this);
    crossFlow->setToolTip("CrossFlow");
    crossFlow->setDecimals(2);
    crossFlow->setMaximum(20.0);
    crossFlow->setMinimum(0.01);
    lay->addWidget(crossFlow, 3, 1, 1, 2);

    labelPtr = new QLabel(this);
    pixMap = QPixmap(":/images/t0.png");
    pixMap = pixMap.scaledToHeight(16, Qt::SmoothTransformation);
    labelPtr = new QLabel(this);
    labelPtr->setPixmap(pixMap);
    lay->addWidget(labelPtr, 4, 0);
    leftOffsetTime = new QDoubleSpinBox(this);
    leftOffsetTime->setToolTip("left Offset");
    leftOffsetTime->setDecimals(5);
    leftOffsetTime->setMaximum(100000.0);
    leftOffsetTime->setMinimum(0.00001);
    lay->addWidget(leftOffsetTime, 4, 1, 1, 2);


    labelPtr = new QLabel(this);
    pixMap = QPixmap(":/images/T.png");
    pixMap = pixMap.scaledToHeight(16, Qt::SmoothTransformation);
    labelPtr = new QLabel(this);
    labelPtr->setPixmap(pixMap);
    lay->addWidget(labelPtr, 2, 4);
    temperature = new QDoubleSpinBox(this);
    temperature->setToolTip("Temperature");
    temperature->setDecimals(2);
    temperature->setMaximum(1000);
    temperature->setMinimum(0.01);
    lay->addWidget(temperature, 2, 5, 1, 2);

    labelPtr = new QLabel("viscosity [mPa*s]", this);
    lay->addWidget(labelPtr, 3, 4);
    viscosity = new QDoubleSpinBox(this);
    viscosity->setDecimals(5);
    viscosity->setMaximum(100.0);
    viscosity->setMinimum(0.0001);
    viscosity->setSingleStep(0.01);
    lay->addWidget(viscosity, 3, 5, 1, 2);


    labelPtr = new QLabel(this);
    pixMap = QPixmap(":/images/zPerc.png");
    pixMap = pixMap.scaledToHeight(16, Qt::SmoothTransformation);
    labelPtr->setPixmap(pixMap);
    lay->addWidget(labelPtr, 4, 4);
    relFocusPoint = new QDoubleSpinBox(this);
    relFocusPoint->setMaximum(100.0);
    relFocusPoint->setMinimum(0.1);
    relFocusPoint->setDecimals(1);
    relFocusPoint->setSingleStep(0.1);
    lay->addWidget(relFocusPoint, 4, 5, 1, 2);

    voidPeakLabel = new QLabel(this);
    pixMap = QPixmap(":/images/tvoid.png");
    pixMap = pixMap.scaledToHeight(16, Qt::SmoothTransformation);
    voidPeakLabel = new QLabel(this);
    voidPeakLabel->setPixmap(pixMap);
    lay->addWidget(voidPeakLabel, 5, 3, 1, 2);

    voidPeakTime = new QDoubleSpinBox(this);
    voidPeakTime->setToolTip("void peak");
    voidPeakTime->setMaximum(100000.0);
    voidPeakTime->setMinimum(0.01);
    voidPeakTime->setSingleStep(0.01);
    lay->addWidget(voidPeakTime, 5, 5, 1, 2);

    useVoidPeakBox = new QCheckBox(this);
    lay->addWidget(useVoidPeakBox, 5, 0, 1, 1);
    QObject::connect(useVoidPeakBox, SIGNAL(toggled(bool)), this, SLOT(enableVoidPeakTime(bool)));
    labelPtr =  new QLabel("Recalculate <i>V</i><sup>0</sup>", this);
    lay->addWidget(labelPtr, 5, 1, 1, 2);

    loadSettings();
}


void AF4ExpSettingsFrame::loadSettings()
{
#ifndef CHECK_SETTINGS_CONVERSION
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
    if(!ok){ \
    AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
    .arg(keyName).arg(defaultValueName)); \
}\
};
#endif // CHECK_SETTINGS_CONVERSION
    QSettings settings("AgCoelfen", "FFFEval");
    bool ok{false};
    settings.setIniCodec("UTF-8");

    double initValue = settings.value(tr("%1/expSettings/%2/temperature").arg(prefix).arg(id), "300.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/temperature").arg(prefix).arg(id), "3.0e2");
    temperature->setValue(initValue);

    initValue = settings.value(tr("%1/expSettings/%2/elutionFlow").arg(prefix).arg(id), "1.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/elutionFlow").arg(prefix).arg(id), "1.0");
    elutionFlow->setValue(initValue);

    initValue = settings.value(tr("%1/expSettings/%2/crossFlow").arg(prefix).arg(id), "1.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/crossFlow").arg(prefix).arg(id), "1.0");
    crossFlow->setValue(initValue);

    initValue = settings.value(tr("%1/expSettings/%2/leftOffsetTime").arg(prefix).arg(id), "1.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/leftOffsetTime").arg(prefix).arg(id), "1.0");
    leftOffsetTime->setValue(initValue);

    initValue = settings.value(tr("%1/expSettings/%2/voidPeakTime").arg(prefix).arg(id), "1.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/voidPeakTime").arg(prefix).arg(id), "1.0");
    voidPeakTime->setValue(initValue);

    initValue = settings.value(tr("%1/expSettings/%2/relFocusPoint").arg(prefix).arg(id), "10.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/relFocusPoint").arg(prefix).arg(id), "10.0");
    relFocusPoint->setValue(initValue);

    initValue = settings.value(tr("%1/expSettings/%2/viscosity").arg(prefix).arg(id), "10.0").toDouble(&ok);
    CHECK_SETTINGS_CONVERSION(tr("%1/expSettings/%2/viscosity").arg(prefix).arg(id), "10.0");
    viscosity->setValue(initValue);

    bool checked = settings.value(tr("%1/expSettings/%2/useManualVoidPeak").arg(prefix).arg(id), "true").toBool();
    checked ? useVoidPeakBox->setCheckState(Qt::Checked) : useVoidPeakBox->setCheckState(Qt::Unchecked);
    this->enableVoidPeakTime(checked);


#undef CHECK_SETTINGS_CONVERSION
}

void AF4ExpSettingsFrame::writeSettings()
{
    QSettings settings("AgCoelfen", "FFFEval");
    settings.setIniCodec("UTF-8");

//    QString prefix = (id >= 0 ? QString("project") : QString("other"));

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
   ExpFFFParameters p;
   p.temperature = this->getTemperatureVal();
   p.viscosity = this->getViscosityVal();
   //p.voidPeakTime = this->getVoidPeakTimeVal();
   p.relFocusPoint = this->getRelFocusPointVal();
   p.elutionFlow = this->getElutionFlowVal();
   p.crossFlow = this->getCrossFlowVal();
   p.leftOffsetTime = this->getLeftOffsetTimeVal();
   return p;
}
