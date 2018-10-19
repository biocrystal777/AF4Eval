#include "af4calibparametersframe.h"

AF4CalibParametersFrame::AF4CalibParametersFrame(int channelId, int calibId,
                                                 const QString &channelName,
                                                 const QString &calibName,
                                                 //QWeakPointer<QPushButton> saveButton,
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
   //leftOffsetTime->setMaximumWidth(100);
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
   //voidPeakTime->setMinimum(0.00001);
   connect(leftOffsetTime.data(),qOverload<double>(&QDoubleSpinBox::valueChanged),
           voidPeakTime.data(),  &QDoubleSpinBox::setMinimum);
   //voidPeakTime->setMaximumWidth(100);
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
   connect(voidPeakTime.data(),qOverload<double>(&QDoubleSpinBox::valueChanged),
           elutionTime.data(),  &QDoubleSpinBox::setMinimum);
   //elutionTime->setMinimum(0.00001);
   //elutionTime->setMaximumWidth(100);
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
   connect(this, &AF4CalibParametersFrame::saveButtonClicked, this, &AF4CalibParametersFrame::saveSettings);
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
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("channels/%1/calib/%2/diffCoefficient").arg(channelId).arg(calibId), QVariant(getDiffCoefficient()));
   settings.setValue(tr("channels/%1/calib/%2/crossFlow").arg(channelId).arg(calibId),       QVariant(getCrossFlow()));
   settings.setValue(tr("channels/%1/calib/%2/temperature").arg(channelId).arg(calibId),     QVariant(getTemperature()));
   settings.setValue(tr("channels/%1/calib/%2/elutionFlow").arg(channelId).arg(calibId),     QVariant(getElutionFlow()));
   settings.setValue(tr("channels/%1/calib/%2/leftOffsetTime").arg(channelId).arg(calibId),  QVariant(leftOffsetTime->value()));
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
