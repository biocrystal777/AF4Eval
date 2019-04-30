#include "af4stokeseinsteincalculatorwidget.h"
#include "../../Core/af4datatypes.h"

AF4StokesEinsteinCalculatorWidget::AF4StokesEinsteinCalculatorWidget(QWidget *parent) :
   QDialog(parent)
{
   this->setWindowTitle("D Calculator");
   lay = new QGridLayout(this);

   auto makeSpinBox =
         [this](QDoubleSpinBox *&spinBox, QString labelString, QString toolTip,
         int row, int column, int rowSpan, int columnSpan) {
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

   makeSpinBox  (stokesRad,   "R_S / nm",   "Stokes Radius",  1, 1, 1, 3);
   configSpinBox(stokesRad,         1, 0.1, 0.1, 9.9e2);
   makeSpinBox  (temperature, "T / K",     "Temperature",     2, 1, 1, 3);
   configSpinBox(temperature,       1, 0.1, 0.1, 9.9e3);
   makeSpinBox  (viscosity,   "η / cP",   "Viscosity",        3, 1, 1, 3);
   configSpinBox(viscosity,         4, 0.001, 0.1, 9.9e2);

   dFrame = new QFrame(this);
   dFrame->setFrameStyle(0x1011);
   frameLay = new QHBoxLayout(dFrame);
   QwtTextLabel *qwtLabel = new QwtTextLabel(dFrame);
   //qwtLabel->setText(QString("<math><mi>D</mi><mtext>&nbsp; /&nbsp; cm²/s </mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("D / (cm²/s) "), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Diffusion coefficient"));
   frameLay->addWidget(qwtLabel);
   //diffCoefficient = new QLineEdit(this);
   diffCoefficient = new AF4SciNotSpinBox(false, this);
   diffCoefficient->setMinimum(1e-15);
   diffCoefficient->setMaximum(1e10);
   diffCoefficient->setReadOnly(true);
   frameLay->addWidget(diffCoefficient);
   lay->addWidget(dFrame, 3, 0, 1, 4);

   loadParameters();
   connect(stokesRad,   qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AF4StokesEinsteinCalculatorWidget::reCalculate);
   connect(temperature, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AF4StokesEinsteinCalculatorWidget::reCalculate);
   connect(viscosity,   qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AF4StokesEinsteinCalculatorWidget::reCalculate);
   reCalculate();

   accepter = new QPushButton("Take this D", this);
   connect(accepter, &QPushButton::clicked, [this](){ emit newDiffCoeff(diffCoefficient->value()); accept(); } );
   lay->addWidget(accepter, 4, 0, 1, 2);

   rejecter = new QPushButton("Cancel", this);
   connect(rejecter, &QPushButton::clicked, this, &AF4StokesEinsteinCalculatorWidget::reject );
   lay->addWidget(rejecter, 4, 2, 1, 2);
}

AF4StokesEinsteinCalculatorWidget::~AF4StokesEinsteinCalculatorWidget()
{
   saveParameters();
}

void AF4StokesEinsteinCalculatorWidget::reCalculate()
{
   // constant factor  kB / (6π) * UNIT
   constexpr lDouble fac = (lDouble) KB / ( 6.0L * (lDouble) PI) *  1e16L; // unit conversion factor
   lDouble D = fac * temperature->value() / ( viscosity->value() * stokesRad->value());
   diffCoefficient->setValue(static_cast<double>(D));
   saveParameters();
}

void AF4StokesEinsteinCalculatorWidget::saveParameters()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   settings.setValue("AF4StokesEinsteinCalculatorWidget/RS",      stokesRad->value());
   settings.setValue(tr("AF4StokesEinsteinCalculatorWidget/T"),   temperature->value());
   settings.setValue(tr("AF4StokesEinsteinCalculatorWidget/eta"), viscosity->value());
}

void AF4StokesEinsteinCalculatorWidget::loadParameters()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");

   auto loadSetting = [this, &settings](QString paramKey, std::function< void (double) > setVal, double defaultVal){
      bool ok;
      double val = settings.value(tr("AF4StokesEinsteinCalculatorWidget/%1").arg(paramKey), defaultVal).toDouble(&ok);
      setVal(val);
      if(!ok) AF4Log::logWarning(tr("%1 could not found, set to %2").arg(paramKey).arg(defaultVal));
   };

   loadSetting("RS",  [this](double v){ stokesRad->setValue(v);   },  77.7);
   loadSetting("T",   [this](double v){ temperature->setValue(v); }, 333.3);
   loadSetting("eta", [this](double v){ viscosity->setValue(v);   },  11.1);
}
