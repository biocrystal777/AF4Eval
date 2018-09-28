#include "af4stokeseinsteincalculatorwidget.h"
#include "../../Core/af4datatypes.h"

AF4StokesEinsteinCalculatorWidget::AF4StokesEinsteinCalculatorWidget(QWidget *parent) :
   QDialog(parent)
{
   this->setWindowTitle("D Calculator");
   lay = new QGridLayout(this);

   QwtTextLabel *qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString(" <math><msub><mi>R</mi><mtext>S</mtext></msub><mtext>&nbsp;/&nbsp;&mu;m</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("R_S / nm"), QwtText::PlainText);
   //qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.2);
   qwtLabel->setToolTip(QString("Stokes Radius"));
   lay->addWidget(qwtLabel, 0, 0, 1, 2, Qt::AlignRight);
   stokesRad = new QDoubleSpinBox(this);
   stokesRad->setDecimals(2);
   stokesRad->setMaximum(990.0);
   stokesRad->setMinimum(0.1);
   stokesRad->setSingleStep(0.1);
   lay->addWidget(stokesRad, 0, 2, 1, 3);

   qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><mi>T</mi><mtext>&nbsp;/&nbsp;K</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("T / K"), QwtText::PlainText);
   //qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.2);
   qwtLabel->setToolTip(QString("Temperature"));
   lay->addWidget(qwtLabel, 1, 0, 1, 2, Qt::AlignRight);
   temperature = new QDoubleSpinBox(this);
   temperature->setToolTip("Temperature");
   temperature->setDecimals(1);
   temperature->setSingleStep(0.1);
   temperature->setMaximum(1000.0);
   temperature->setMinimum(0.001);
   temperature->setMaximumWidth(100);
   lay->addWidget(temperature, 1, 2, 1, 3);

   //labelPtr = new QLabel("visc [mPa*s]", this);
   qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><mi>&eta;</mi><mtext>&nbsp;/&nbsp;cP</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("η / cP"), QwtText::PlainText);
   //qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.2);
   qwtLabel->setToolTip(QString("Viscosity"));
   lay->addWidget(qwtLabel, 2, 0, 1, 2, Qt::AlignRight);
   viscosity = new QDoubleSpinBox(this);
   viscosity->setDecimals(5);
   viscosity->setMaximum(100.0);
   viscosity->setMinimum(0.0001);
   viscosity->setSingleStep(0.01);
   lay->addWidget(viscosity, 2, 2, 1, 3);

   qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><mi>D</mi><mtext>&nbsp; /&nbsp; cm²/s </mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("D / (cm²/s) "), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Diffusion coefficient"));
   lay->addWidget(qwtLabel, 3, 0, 1, 2, Qt::AlignRight);
   diffCoefficient = new QLineEdit(this);
   diffCoefficient->setReadOnly(true);
   lay->addWidget(diffCoefficient, 3, 2, 1, 2);

   loadParameters();
   connect(stokesRad,   qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AF4StokesEinsteinCalculatorWidget::reCalculate);
   connect(temperature, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AF4StokesEinsteinCalculatorWidget::reCalculate);
   connect(viscosity,   qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AF4StokesEinsteinCalculatorWidget::reCalculate);
   reCalculate();
}

AF4StokesEinsteinCalculatorWidget::~AF4StokesEinsteinCalculatorWidget()
{
   saveParameters();
}

void AF4StokesEinsteinCalculatorWidget::reCalculate()
{
   constexpr lDouble fac = (lDouble) KB / ( (lDouble) 6.0 * (lDouble) PI) * (lDouble) 1e16; // unit conversion factor
   lDouble D = fac * temperature->value() / ( viscosity->value() * stokesRad->value());
   diffCoefficient->setText(QString::number(D, 'E', 4));
   saveParameters();
}

void AF4StokesEinsteinCalculatorWidget::saveParameters()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("diffCalculator/RS"), stokesRad->value());
   settings.setValue(tr("diffCalculator/T"), temperature->value());
   settings.setValue(tr("diffCalculator/eta"), viscosity->value());
}

void AF4StokesEinsteinCalculatorWidget::loadParameters()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok;
   double val = settings.value(tr("diffCalculator/RS"), 10.0).toDouble(&ok);
   if(ok) stokesRad->setValue(val);
   else AF4Log::logWarning(tr("Stokes Radius could not found, set to default"));
   val = settings.value(tr("diffCalculator/T"), 300.0).toDouble(&ok);
   if(ok) temperature->setValue(val);
   else AF4Log::logWarning(tr("Temperature could not be found, set to default"));
   val = settings.value(tr("diffCalculator/eta"), 10.0).toDouble(&ok);
   if(ok) viscosity->setValue(val);
   else AF4Log::logWarning(tr("Viscosity could not be found, set to default"));
}
