#include "af4scinotspinbox.h"

//-//////////////////////////////////////
//
//   Public stuff
//
//-//////////////////////////////////////

AF4SciNotSpinBox::AF4SciNotSpinBox(const bool signedBox, QWidget *parent) :
   QWidget(parent), signedBox(signedBox)
{
   layout = new QHBoxLayout(this);
   significand = new QDoubleSpinBox(this);
   significand->setKeyboardTracking(false);   

   connect(significand, qOverload<double>(&QDoubleSpinBox::valueChanged),
           [this] () {
      adjustSpinBoxLimits();
      adjustInputToFormat();
      emit valueChanged();
      emit valueChanged(this->value()); } );

   layout->addWidget(significand, 0, Qt::AlignLeft);

   eLabel = new QLabel("E", this);
   layout->addWidget(eLabel ,0);

   exponent = new QSpinBox(this);
   exponent->setKeyboardTracking(true);
   connect(exponent, qOverload<int>(&QSpinBox::valueChanged),
           [this] () {
      adjustSpinBoxLimits();
      emit valueChanged();
      emit valueChanged(this->value()); } );

   setSignifandDecimals(5);
   setSignifandSingleStep(0.05);
   adjustSpinBoxLimits();
   layout->addWidget(exponent, 0, Qt::AlignLeft);
}

void AF4SciNotSpinBox::setValue(double value, bool *ok)
{
   const double minVal = valAsDouble(minSig, minExp);
   const double maxVal = valAsDouble(maxSig, maxExp);
   if(value < minVal){
      value = minVal;
      if(ok) *ok = false;
   }
   else if(value > maxVal) {
      value = maxVal;
      if(ok) *ok = false;
   }
   else
      if(ok) *ok = true;
   int exp;
   double signif = AF4SciNotSpinBox::calcSignificand(value, &exp);   
   setValueIntern(signif, exp);
   adjustSpinBoxLimits();
}

void AF4SciNotSpinBox::setMaximum(double value, bool *ok)
{
   int exp;
   double signif = AF4SciNotSpinBox::calcSignificand(value, &exp);
   setMaximumIntern(signif, exp, ok);

}

double AF4SciNotSpinBox::maximum()
{
   return maxSig * pow10(maxExp);
}


double AF4SciNotSpinBox::minimum()
{
   return minSig * pow10(minExp);
}

void AF4SciNotSpinBox::setMinimum(double value, bool *ok)
{
   int exp;
   double signif = AF4SciNotSpinBox::calcSignificand(value, &exp);
   setMinimumIntern(signif, exp, ok);
}

void AF4SciNotSpinBox::setReadOnly(bool r)
{
   significand->setReadOnly(r);
   exponent->setReadOnly(r);
}

void AF4SciNotSpinBox::setSignifandSingleStep(double stepSize, bool *ok)
{
   const double minStepSize = pow10(-significand->decimals());
   if(stepSize < minStepSize){
      stepSize = minStepSize;
      if(ok) *ok = false;
   }
   else if(stepSize > constMaxStepSize){
      stepSize = constMaxStepSize;
      if(ok) *ok = false;
   }
   else
      if(ok) *ok = true;
   significand->blockSignals(true);
   significand->setSingleStep(stepSize);
   significand->blockSignals(false);
}

void AF4SciNotSpinBox::setSignifandDecimals(int dec)
{
   significand->blockSignals(true);
   significand->setDecimals(dec);
   maxSig = maxSigForDec();
   adjustSpinBoxLimits();
   significand->blockSignals(false);
}


//-//////////////////////////////////////
//
//   protected stuff
//
//-//////////////////////////////////////


void AF4SciNotSpinBox::setValueIntern(double sig, int exp)
{
   significand->blockSignals(true);
   exponent->blockSignals(true);
   significand->setValue(sig);
   exponent->setValue(exp);
   significand->blockSignals(false);
   exponent->blockSignals(false);
}

void AF4SciNotSpinBox::setMaximumIntern(double sig, int exp, bool *ok)
{
   if(ok) *ok=true;
   // set maxSign if it is in maximal range of this class
   if(sig < 10.0)
      maxSig = sig;
   else {
      maxSig = maxSigForDec();
      if(ok) *ok = false;
   }
   // set maxExp if it is in maximal range of this class
   if(exp > constMaxExpLimit ){
      this->maxExp = constMaxExpLimit;
      if(ok) *ok = false;
   }
   else if(exp < constMinExpLimit ) {
      this->maxExp = constMinExpLimit;
      if(ok) *ok = false;
   }
   else
      this->maxExp = exp;
   // apply changes to spin boxes
   adjustSpinBoxLimits();
}

void AF4SciNotSpinBox::setMinimumIntern(double sig, int exp, bool *ok)
{
   if(ok) *ok=true;
   // set minSign if it is in maximal range of this class
   const double minDispSig = pow10(-(significand->decimals()));
   if(minDispSig < sig)
      this->minSig = sig;
   else {
      this->minSig = minDispSig;
      if(ok) *ok = false;
   }
   // set minExp if it is in maximal range of this class
   if(exp > constMaxExpLimit ){
      this->minExp = constMaxExpLimit;
      if(ok) *ok = false;
   }
   else if(exp < constMinExpLimit ) {
      this->minExp = constMinExpLimit;
      if(ok) *ok = false;
   }
   else
      this->minExp = exp;
   // apply changes to spin boxes
   adjustSpinBoxLimits();
}



void AF4SciNotSpinBox::adjustInputToFormat()
{
   double sig = significand->value();
   int modExp=0;
   while(fabs(sig) >= 10.0 ){
      sig /= 10.0;
      ++modExp;
   }
   while(fabs(sig) < 1.0 ){
      sig *= 10.0;
      --modExp;
   }
   if(modExp != 0){
      setValueIntern(sig, exponent->value() + modExp);
   }
}

void AF4SciNotSpinBox::adjustSpinBoxLimits()
{
   // if a default MinMax is used for the spinbox
   // values out of the range of 1.0-9.999.... will
   // have to be caught by adjustInputToFormat()
   const double defaultMin = 0.0;
   const double defaultMax = 99.9;

   exponent->blockSignals(true);
   exponent->setRange(minExp, maxExp);

   exponent->blockSignals(false);

   significand->blockSignals(true);
   if      (exponent->value() == minExp) significand->setRange(minSig,    defaultMax);
   else if (exponent->value() == maxExp) significand->setRange(defaultMin, maxSig);
   else                                  significand->setRange(defaultMin, defaultMax);
   significand->blockSignals(false);
}


double AF4SciNotSpinBox::calcSignificand(double number, int *exponent)
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
