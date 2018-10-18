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
   lay->addWidget(qwtLabel, 0, 0, 1, 1, Qt::AlignRight);
   stokesRad = new QDoubleSpinBox(this);
   stokesRad->setToolTip("Stokes Radius");
   stokesRad->setDecimals(2);
   stokesRad->setMaximum(990.0);
   stokesRad->setMinimum(0.1);
   stokesRad->setSingleStep(0.1);
   lay->addWidget(stokesRad, 0, 1, 1, 3);

   qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><mi>T</mi><mtext>&nbsp;/&nbsp;K</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("T / K"), QwtText::PlainText);
   //qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.2);
   qwtLabel->setToolTip(QString("Temperature"));
   lay->addWidget(qwtLabel, 1, 0, 1, 1, Qt::AlignRight);
   temperature = new QDoubleSpinBox(this);
   temperature->setToolTip("Temperature");
   temperature->setDecimals(1);
   temperature->setSingleStep(0.1);
   temperature->setMaximum(400);
   temperature->setMinimum(250);   
   lay->addWidget(temperature, 1, 1, 1, 3);

   //labelPtr = new QLabel("visc [mPa*s]", this);
   qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("<math><mi>&eta;</mi><mtext>&nbsp;/&nbsp;cP</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("η / cP"), QwtText::PlainText);
   //qwtLabel->setMinimumWidth(qwtLabel->size().width() * 1.2);
   qwtLabel->setToolTip(QString("Viscosity"));
   lay->addWidget(qwtLabel, 2, 0, 1, 1, Qt::AlignRight);
   viscosity = new QDoubleSpinBox(this);
   viscosity->setDecimals(5);
   viscosity->setMaximum(1e2);
   viscosity->setMinimum(1e-3);
   viscosity->setSingleStep(0.01);
   lay->addWidget(viscosity, 2, 1, 1, 3);

   dFrame = new QFrame(this);
   dFrame->setFrameStyle(0x1011);
   frameLay = new QHBoxLayout(dFrame);
   qwtLabel = new QwtTextLabel(dFrame);
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
   settings.setValue(tr("AF4StokesEinsteinCalculatorWidget/RS"),  stokesRad->value());
   settings.setValue(tr("AF4StokesEinsteinCalculatorWidget/T"),   temperature->value());
   settings.setValue(tr("AF4StokesEinsteinCalculatorWidget/eta"), viscosity->value());
}

void AF4StokesEinsteinCalculatorWidget::loadParameters()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   bool ok;
   double val = settings.value(tr("AF4StokesEinsteinCalculatorWidget/RS"), 10.0).toDouble(&ok);
   if(ok) stokesRad->setValue(val);
   else AF4Log::logWarning(tr("Stokes Radius could not found, set to default"));
   val = settings.value(tr("AF4StokesEinsteinCalculatorWidget/T"), 300.0).toDouble(&ok);
   if(ok) temperature->setValue(val);
   else AF4Log::logWarning(tr("Temperature could not be found, set to default"));
   val = settings.value(tr("AF4StokesEinsteinCalculatorWidget/eta"), 10.0).toDouble(&ok);
   if(ok) viscosity->setValue(val);
   else AF4Log::logWarning(tr("Viscosity could not be found, set to default"));
}
