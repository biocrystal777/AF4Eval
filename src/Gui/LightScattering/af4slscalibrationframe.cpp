#include "af4slscalibrationframe.h"

QList<bool> AF4SLSCalibrationFrame::getUsedAngles()
{
   QList<bool> usedAngles;
   for (AF4NumberedCheckBox *box : useAngles) usedAngles.append( box->isChecked() );
   return usedAngles;
}

QList<double> AF4SLSCalibrationFrame::getMallsAngleConstants()
{
   QList<double> mallsAngles;
   for (QDoubleSpinBox *box : mallsAngleConstants) mallsAngles.append( box->value() );
   return mallsAngles;
}

AF4SLSCalibrationFrame::AF4SLSCalibrationFrame(const QString &prefix, int id, QWidget *parent)
   : QFrame(parent), prefix(prefix), id(id)
{

   this->setFrameStyle(0x1011);
   lay = new QGridLayout(this);
   lay->addWidget(new QLabel(tr("<b>Optical Calibration Constants</b>"), this),0, 0, 1, 5, Qt::AlignLeft);
   lay->addWidget(new QLabel(tr("RI:"), this), 1, 1, 1, 2, Qt::AlignRight);
   riConstant = new AF4SciNotSpinBox(false, this);
   riConstant->setToolTip("Calibration Constant of the RI Detector");
   riConstant->setMaximumHeight(35);
   lay->addWidget(riConstant, 1, 3, 1, 2, Qt::AlignRight);

   lay->addWidget(new QLabel(tr("use"), this), 3, 0, 1, 2, Qt::AlignLeft);
   lay->addWidget(new QLabel(tr("MALLS:"), this), 3, 1, 1, 2, Qt::AlignRight);
   mallsConstant = new AF4SciNotSpinBox( false, this);
   mallsConstant->setToolTip("Calibration Constant of the RI Detector");
   mallsConstant->setMaximumHeight(35);
   lay->addWidget(mallsConstant, 3, 3, 1, 2, Qt::AlignRight);


   rayleighGroup = new QGroupBox(this);
   rayleighLayout = new QGridLayout(rayleighGroup);
   rayleighButton = new QRadioButton("Rayleigh", rayleighGroup);
   voltButton = new QRadioButton("Signal [Volt]", rayleighGroup);
   voltButton->setChecked(true);
   connect(rayleighButton, &QRadioButton::toggled, this, &AF4SLSCalibrationFrame::setToRayleigh);
   connect(voltButton, &QRadioButton::toggled, this, &AF4SLSCalibrationFrame::setToVolt);
   rayleighLayout->addWidget(rayleighButton, 0, 0);
   rayleighLayout->addWidget(voltButton, 0, 4);
   lay->addWidget(rayleighGroup, 2, 0, 1, 5);

   for (int k = 0; k < NUMBER_OF_DETECTORS; ++k)
      angleLabels.append(new QLabel(tr("%1 :").arg(k+1), this));

   for (int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      useAngles.append(new AF4NumberedCheckBox(i, this));
      lay->addWidget(useAngles.at(i), 4+i, 0, 1, 1, Qt::AlignLeft);
      useAngles.at(i)->setToolTip("Use this angle signal in Evaluation");
      useAngles.at(i)->setMaximumHeight(35);

      connect(useAngles.at(i), qOverload<bool, int>(&AF4NumberedCheckBox::toggled),
              this, &AF4SLSCalibrationFrame::enableAngle);

      lay->addWidget(angleLabels.at(i), 4+i, 1, 1, 2, Qt::AlignRight);

      mallsAngleConstants.append(new QDoubleSpinBox(this));
      mallsAngleConstants.last()->setDecimals(4);
      mallsAngleConstants.last()->setSingleStep(0.001);
      mallsAngleConstants.last()->setMaximum(10.0);
      mallsAngleConstants.last()->setToolTip(tr("Calibration Constant %1 of MALLS at %2 degrees").arg(i).arg(::anglesDeg.at(i)));
      mallsAngleConstants.last()->setMaximumHeight(35);
      lay->addWidget(mallsAngleConstants.at(i), 4+i, 3, 1, 2, Qt::AlignRight);
   }

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   double value;
   bool boolValue;
   bool ok;


#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };

   value = settings.value(tr("%1/slscalibration/%2/calibMALLS").arg(prefix).arg(id), "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("%1/slscalibration/%2/calibMALLS").arg(prefix).arg(id), "1.0");
   mallsConstant->setValue(value);

   value = settings.value(tr("%1/slscalibration/%2/calibRI").arg(prefix).arg(id), "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("%1/slscalibration/%2/calibRI").arg(prefix).arg(id), "1.0");
   riConstant->setValue(value);

   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){

      boolValue = settings.value(tr("%1/slscalibration/%2/%3/enable").arg(prefix).arg(id).arg(i), true).toBool();
      useAngles.at(i)->setChecked(boolValue);
      mallsAngleConstants.at(i)->setEnabled(boolValue);
      value = settings.value(tr("%1/slscalibration/%2/%3/value").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("%1/slscalibration/%2/%3/value").arg(prefix).arg(id).arg(i), "1.0");
      mallsAngleConstants.at(i)->setValue(value);
   }
#undef CHECK_SETTINGS_CONVERSION

}

AF4SLSCalibrationFrame::~AF4SLSCalibrationFrame()
{
   writeSettings();
}

void AF4SLSCalibrationFrame::writeSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("%1/slscalibration/%2/calibMALLS").arg(prefix).arg(id), mallsConstant->value());
   settings.setValue(tr("%1/slscalibration/%2/calibRI").arg(prefix).arg(id), riConstant->value());
   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      settings.setValue(tr("%1/slscalibration/%2/%3/enable").arg(prefix).arg(id).arg(i), useAngles.at(i)->isChecked());
      settings.setValue(tr("%1/slscalibration/%2/%3/value").arg(prefix).arg(id).arg(i), mallsAngleConstants.at(i)->value());
   }
}

void AF4SLSCalibrationFrame::enableAngle(bool enable, int i) const
{
   if(i < angleLabels.size() && i < mallsAngleConstants.size()){
      angleLabels.at(i)->setEnabled(enable);
      mallsAngleConstants.at(i)->setEnabled(enable);
   }
}

void AF4SLSCalibrationFrame::setToRayleigh()
{
   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      useAngles.at(i)->setEnabled(false);
      enableAngle(false, i);
   }
}

void AF4SLSCalibrationFrame::setToVolt()
{
   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      useAngles.at(i)->setEnabled(true);
      enableAngle(useAngles.at(i)->isChecked(), i);
   }
}

SLSCalibrationParameters AF4SLSCalibrationFrame::getCalibParamters()
{
   return  SLSCalibrationParameters {
      getRIConstant(),
            getMallsConstant(),
            getUsedAngles(),
            getMallsAngleConstants()
   };
}

