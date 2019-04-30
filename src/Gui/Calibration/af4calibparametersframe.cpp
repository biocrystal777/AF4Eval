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

   auto makeSpinBox = [this](QDoubleSpinBox *&spinBox, QString labelString, QString toolTip, int row, int column, int rowSpan, int columnSpan, int maxWidth )  {
      QwtTextLabel *label = new QwtTextLabel(this);
      label->setText(labelString, QwtText::PlainText);
      label->setToolTip(toolTip);
      lay->addWidget(label, row, 0, Qt::AlignLeft);
      spinBox = new QDoubleSpinBox(this);
      spinBox->setToolTip(toolTip);
      spinBox->setMaximumWidth(maxWidth);
      lay->addWidget(spinBox, row, column, rowSpan, columnSpan);
   };

   auto configSpinBox = [this](QDoubleSpinBox *spinBox, int decimals, double singleStep, double minimum, double maximum){
      spinBox->setDecimals(decimals);
      spinBox->setSingleStep(singleStep);
      spinBox->setMinimum(minimum);
      spinBox->setMaximum(maximum);
   };

   makeSpinBox(temperature, QString("T / K"),         QString("Temperature"), 0, 1, 1, 2, 100);
   configSpinBox(temperature, 1, 0.1, 0.1, 500.0);

   makeSpinBox(elutionFlow, QString("Ve / (ml/min)"), QString("Elution Flow"), 1, 1, 1, 2, 100);
   configSpinBox(elutionFlow, 3, 0.1, 0.1, 10.0);

   makeSpinBox(crossFlow, QString("Vc / (ml/min)"),   QString("Cross Flow"), 2, 1, 1, 2, 100);
   configSpinBox(crossFlow, 3, 0.1, 0.1, 10.0);

   makeSpinBox(relFocusPoint, QString("z% / %"),      QString("Relative position of the focussing point\ncompared to the length of the channel"), 3, 1, 1, 2, 100);
   configSpinBox(relFocusPoint, 1, 0.1, 0.1, 100.0);

    /**************************************
    *
    * second column
    *
    *************************************/

   auto makeSharedSpinBox = [this](QSharedPointer<QDoubleSpinBox> &spinBox, QString labelString, QString toolTip, int row, int column, int rowSpan, int columnSpan) {
      QwtTextLabel *label = new QwtTextLabel(this);
      label->setText(labelString, QwtText::PlainText);
      label->setToolTip(toolTip);
      lay->addWidget(label, row, 3, Qt::AlignLeft);
      spinBox =  QSharedPointer<QDoubleSpinBox>(new QDoubleSpinBox(this));
      spinBox->setToolTip(toolTip);
      lay->addWidget(spinBox.data(), row, column, rowSpan, columnSpan);
   };

   makeSharedSpinBox(leftOffsetTime, QString("t_off / min"), QString("Left offset time"), 0, 4, 1, 2);
   configSpinBox(leftOffsetTime.data(), 3, 0.005, 0.000, 30.0);

   makeSharedSpinBox(voidPeakTime, QString("t_void / min"), QString("void peak time"), 1, 4, 1, 2);
   configSpinBox(voidPeakTime.data(), 3, 0.005, 0.001, 30.0);
   connect(leftOffsetTime.data(),qOverload<double>(&QDoubleSpinBox::valueChanged),
           voidPeakTime.data(),  &QDoubleSpinBox::setMinimum);

   makeSharedSpinBox(elutionTime, QString("t_e / min"), QString("elution time"), 2, 4, 1, 2);
   configSpinBox(elutionTime.data(), 3, 0.05, 0.001, 500.0);
   connect(voidPeakTime.data(),qOverload<double>(&QDoubleSpinBox::valueChanged),
           elutionTime.data(),  &QDoubleSpinBox::setMinimum);

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

CtrlBoxRefs AF4CalibParametersFrame::getCtrlBoxRefs()
{
   return CtrlBoxRefs {
      QWeakPointer<QDoubleSpinBox>(this->leftOffsetTime),
            QWeakPointer<QDoubleSpinBox>(this->voidPeakTime),
            QWeakPointer<QDoubleSpinBox>(this->elutionTime)
   };
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

   loadSetting(QString("diffCoefficient"), [this](double v) -> bool { return this->setDiffCoefficient(v);}, 1e-9 );
   loadSetting(QString("crossFlow"),       [this](double v) -> bool { return this->setCrossFlow(v)      ;}, 1.0  );
   loadSetting(QString("relFocusPoint"),   [this](double v) -> bool { return this->setRelFocusPoint(v)  ;}, 0.1  );
   loadSetting(QString("temperature"),     [this](double v) -> bool { return this->setTemperature(v)    ;}, 300  );
   loadSetting(QString("elutionFlow"),     [this](double v) -> bool { return this->setElutionFlow(v)    ;}, 1    );
   loadSetting(QString("leftOffsetTime"),  [this](double v) -> bool { return this->setLeftOffsetTime(v) ;}, 10   );
   loadSetting(QString("voidPeakTime"),    [this](double v) -> bool { return this->setVoidPeakTime(v)   ;}, 12   );
   loadSetting(QString("elutionTime"),     [this](double v) -> bool { return this->setElutionTime(v)    ;}, 20   );
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

