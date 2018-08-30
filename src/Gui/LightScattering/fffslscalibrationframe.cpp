#include "fffslscalibrationframe.h"

#ifndef CHECK_SETTINGS_CONVERSION
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   FFFLog::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };
#endif // CHECK_SETTINGS_CONVERSION

void FFFSLSCalibrationFrame::setRIConstant(double val) const
{
   riConstant->setValueM(val);
}

void FFFSLSCalibrationFrame::setMallsConstant(double val) const
{
   mallsConstant-> setValueM(val);
}

QList<bool> FFFSLSCalibrationFrame::getUsedAngles()
{
   QList<bool> usedAngles;
   for (FFFNumberedCheckBox *box : *useAngles) usedAngles.append( box->isChecked() );
   return usedAngles;
}

QList<double> FFFSLSCalibrationFrame::getMallsAngleConstants()
{
   QList<double> mallsAngles;
   for (QDoubleSpinBox *box : *mallsAngleConstants) mallsAngles.append( box->value() );
   return mallsAngles;
}

FFFSLSCalibrationFrame::FFFSLSCalibrationFrame(const QString &prefix,
                                               int id,
                                               QWidget *parent) :
   QFrame(parent), prefix(prefix), id(id)
{
   this->setFrameStyle(0x1011);
   lay = new QGridLayout(this);
   lay->addWidget(new QLabel(tr("<b>Optical Calibration Constants</b>"), this),0, 0, 1, 5, Qt::AlignLeft);
   lay->addWidget(new QLabel(tr("RI:"), this), 1, 1, 1, 2, Qt::AlignRight);
   riConstant = new FFFTwoBoxWidget(tr("Calibration Constant of the RI Detector"),this);
   riConstant->setMaximumHeight(35);
   lay->addWidget(riConstant, 1, 3, 1, 2, Qt::AlignRight);

   lay->addWidget(new QLabel(tr("use"), this), 3, 0, 1, 2, Qt::AlignLeft);
   lay->addWidget(new QLabel(tr("MALLS:"), this), 3, 1, 1, 2, Qt::AlignRight);
   mallsConstant = new FFFTwoBoxWidget("Calibration Constant of the RI Detector", this);
   mallsConstant->setMaximumHeight(35);
   lay->addWidget(mallsConstant, 3, 3, 1, 2, Qt::AlignRight);


   rayleighGroup = new QGroupBox(this);
   rayleighLayout = new QGridLayout(rayleighGroup);
   rayleighButton = new QRadioButton("Rayleigh", rayleighGroup);
   voltButton = new QRadioButton("Signal [Volt]", rayleighGroup);
   voltButton->setChecked(true);
   QObject::connect(rayleighButton, SIGNAL(toggled(bool)), this, SLOT(setToRayleigh(bool)));
   QObject::connect(voltButton, SIGNAL(toggled(bool)), this, SLOT(setToVolt(bool)));
   rayleighLayout->addWidget(rayleighButton, 0, 0);
   rayleighLayout->addWidget(voltButton, 0, 4);
   lay->addWidget(rayleighGroup, 2, 0, 1, 5);

   angleLabels = new QList<QLabel*>();
   for (int k = 0; k < NUMBER_OF_DETECTORS; ++k){
      angleLabels->append(new QLabel(tr("%1 :").arg(k+1), this));
   }

   mallsAngleConstants = new QList<QDoubleSpinBox*>();
   useAngles = new QList<FFFNumberedCheckBox*>();
   for (int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      useAngles->append(new FFFNumberedCheckBox(i, this));
      lay->addWidget(useAngles->at(i), 4+i, 0, 1, 1, Qt::AlignLeft);
      useAngles->at(i)->setToolTip("Use this angle signal in Evaluation");
      useAngles->at(i)->setMaximumHeight(35);

      QObject::connect(useAngles->at(i), SIGNAL(checkedID(int,bool)), this, SLOT(enableAngle(int,bool)));

      lay->addWidget(angleLabels->at(i), 4+i, 1, 1, 2, Qt::AlignRight);

      mallsAngleConstants->append(new QDoubleSpinBox(this));
      mallsAngleConstants->last()->setDecimals(4);
      mallsAngleConstants->last()->setSingleStep(0.001);
      mallsAngleConstants->last()->setMaximum(10.0);
      mallsAngleConstants->last()->setToolTip(tr("Calibration Constant %1 of MALLS at %2 degrees").arg(i).arg(::anglesDeg.at(i)));
      mallsAngleConstants->last()->setMaximumHeight(35);
      lay->addWidget(mallsAngleConstants->at(i), 4+i, 3, 1, 2, Qt::AlignRight);
   }

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   double value;


   bool boolValue;
   bool ok;

   value = settings.value(tr("%1/slscalibration/%2/calibMALLS").arg(prefix).arg(id), "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("%1/slscalibration/%2/calibMALLS").arg(prefix).arg(id), "1.0");
   mallsConstant->setValueM(value, 501);

   value = settings.value(tr("%1/slscalibration/%2/calibRI").arg(prefix).arg(id), "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("%1/slscalibration/%2/calibRI").arg(prefix).arg(id), "1.0");
   riConstant->setValueM(value, 502);



   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){

      boolValue = settings.value(tr("%1/slscalibration/%2/%3/enable").arg(prefix).arg(id).arg(i), true).toBool();
      useAngles->at(i)->setChecked(boolValue);
      mallsAngleConstants->at(i)->setEnabled(boolValue);

      value = settings.value(tr("%1/slscalibration/%2/%3/value").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("%1/slscalibration/%2/%3/value").arg(prefix).arg(id).arg(i), "1.0");
      mallsAngleConstants->at(i)->setValue(value);
   }
}

FFFSLSCalibrationFrame::~FFFSLSCalibrationFrame()
{
   writeSettings();
   if(useAngles) delete useAngles; useAngles = nullptr;
   if(angleLabels) delete angleLabels; angleLabels = nullptr;
   if(mallsAngleConstants) delete mallsAngleConstants; mallsAngleConstants = nullptr;
}

void FFFSLSCalibrationFrame::writeSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("%1/slscalibration/%2/calibMALLS").arg(prefix).arg(id), mallsConstant->value());
   settings.setValue(tr("%1/slscalibration/%2/calibRI").arg(prefix).arg(id), riConstant->value());
   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      settings.setValue(tr("%1/slscalibration/%2/%3/enable").arg(prefix).arg(id).arg(i), useAngles->at(i)->isChecked());
      settings.setValue(tr("%1/slscalibration/%2/%3/value").arg(prefix).arg(id).arg(i), mallsAngleConstants->at(i)->value());
   }
}

void FFFSLSCalibrationFrame::enableAngle(int i, bool enable) const
{
   if(i < angleLabels->size() && i < mallsAngleConstants->size()){
      angleLabels->at(i)->setEnabled(enable);
      mallsAngleConstants->at(i)->setEnabled(enable);
   }
}

void FFFSLSCalibrationFrame::setToRayleigh(bool)
{
      for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){
         useAngles->at(i)->setEnabled(false);
         enableAngle(i,false);
      }
}

void FFFSLSCalibrationFrame::setToVolt(bool)
{
   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      useAngles->at(i)->setEnabled(true);
      enableAngle(i,useAngles->at(i)->isChecked());
   }
}

SLSCalibrationParameters FFFSLSCalibrationFrame::getCalibParamters()
{
   SLSCalibrationParameters p;
   p.usedAngles = getUsedAngles();
   p.angleConstants = getMallsAngleConstants();
   p.riConstant = getRIConstant();
   p.mallsConstant = getMallsConstant();
   return p;
}
