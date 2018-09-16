#include "ffftwoboxwidget.h"

FFFTwoBoxWidget::FFFTwoBoxWidget(const QString &toolTipString, QWidget *parent) :
   QWidget(parent)
{


   this->setToolTip(toolTipString);
   layout = new QHBoxLayout(this);
   significand = new QDoubleSpinBox(this);

   QObject::connect(significand, SIGNAL(valueChanged(QString)), this, SLOT(callValueChanged()));
   layout->addWidget(significand, 0, Qt::AlignLeft);
   layout->addWidget(new QLabel("E", this), 0);
   exponent = new QSpinBox(this);
   QObject::connect(exponent, SIGNAL(valueChanged(QString)), this, SLOT(callValueChanged()));
   significand->setSingleStep(0.01);
   significand->setDecimals(5);
   significand->setRange(1.0, 9.99999);
   exponent->setRange(-20, 20);
   layout->addWidget(exponent, 0, Qt::AlignLeft);
   layout->addStretch(4);
}

FFFTwoBoxWidget::~FFFTwoBoxWidget()
{
}

double FFFTwoBoxWidget::value() const
{

   return significand->value() * ((double) std::pow(10, exponent->value()));
}

void FFFTwoBoxWidget::setValueS(double signif, int exp, int callID)
{

   if(( 1.0 <= std::fabs(signif)) && (std::fabs(signif) < 10.00)){
      significand->setValue(signif);
      exponent->setValue(exp);
   }
   else if ((0.0 <= signif) && (signif <= 0.0)){
      significand->setValue(0.0);
      exponent->setValue(0);
   } else {
      AF4Log::logWarning(tr("Significand was not adjusted correctly for the TwoBoxWidget:"));
      AF4Log::logWarning(tr("Significand: %1, Exponent: %2, callID: %3").arg(signif).arg(exp).arg(callID));
   }
}

void FFFTwoBoxWidget::setValueM(double value, int callID)
{
   int exp;
   double signif = FFFTwoBoxWidget::calcSignificand(value, &exp);
   setValueS(signif, exp, callID);
}

void FFFTwoBoxWidget::setMaximumS(double sign, int exp)
{
   maximumSign = sign;
   maximumExp = exp;
   exponent->setMaximum(exp);
   significand->setMaximum(sign);
}

void FFFTwoBoxWidget::setMaximum(double value)
{
   int exp;
   double signif = FFFTwoBoxWidget::calcSignificand(value, &exp);
   setMaximumS(signif, exp);
}

void FFFTwoBoxWidget::setMaxExp(int exp)
{
   this->exponent->setMaximum(exp);
}

void FFFTwoBoxWidget::setMinimumS(double sign, int exp)
{
   minimumSign = sign;
   minimumExp = exp;
   exponent->setMinimum(exp);
   significand->setMinimum(sign);
}

void FFFTwoBoxWidget::setMinimum(double value)
{
   int exp;
   double signif = FFFTwoBoxWidget::calcSignificand(value, &exp);
   setMinimumS(signif, exp);
}

void FFFTwoBoxWidget::setMinExp(int exp)
{
   this->exponent->setValue(exp);
}

void FFFTwoBoxWidget::setReadOnly(bool r)
{
   significand->setReadOnly(r);
   exponent->setReadOnly(r);
}

void FFFTwoBoxWidget::callValueChanged()
{
   valueChanged();
   valueChanged(this->value());
}


void FFFTwoBoxWidget::reNormValues(double signInput)
{
   significand->blockSignals(true);

   int newExponent = exponent->value();
   if(std::fabs(signInput) > 0){
      while(signInput >= 10.0){
         signInput /= 10.0;
         newExponent++;
      }
      while(signInput < 1.0){
         signInput *= 10.0;
         newExponent--;
      }
   } else {
      newExponent = 0;
   }
   significand->setValue(signInput);
   exponent->setValue(newExponent);
   significand->blockSignals(false);
}

double FFFTwoBoxWidget::calcSignificand(double number, int *exponent)
{
   // remove sign for convenience and initialize exponent result:
   double absNumber = std::fabs(number);
   (*exponent) = 0;
   // special cases 0.0 and -0.0 :
   if (!(absNumber > 0.0)){
      return absNumber;
   }
   // calculate, until the actual significand has an exponent = 1;
   // adjust, thereby, the  exponent of the actual number;
   while(absNumber < 1.0){
      absNumber *= 10.0;
      (*exponent)--;
   }
   int test = 0;
   while(absNumber >= 10.0){
      absNumber /= 10.0;

      (*exponent)++;

      test++;
      if (test == 20) break;
   }

   // reinsert sign:
   if(number > 0.0){
      return absNumber;
   } else {
      return -absNumber;
   }
}

void FFFTwoBoxWidget::setSignifandStep(double stepSize)
{
   significand->setSingleStep(stepSize);
}

void FFFTwoBoxWidget::setSignifandDecimals(uint number)
{
   significand->setDecimals(number);
}

