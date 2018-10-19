#include "./af4diffevaluator.h"

using std::string;
using std::vector;

#define IS_ZERO(value, errorCode) {\
   if(isZero(value)) {\
   return errorCode;\
   }\
   }

int AF4DiffEvaluator::evalDiffCoeffs(vecD& times,
                                     double leftOffsetTime,
                                     double voidPeakTime,
                                     double elutionFlow,
                                     double crossFlow,
                                     double chWidth,
                                     double chVolume,
                                     double relFocusPoint,
                                     double chLength,
                                     bool reCalcChVolume,
                                     bool displayZeroMessages,
                                     double* progress,
                                     uint* currentLine)
{
   this->displayZeroMessages = displayZeroMessages;
   //////////////////////
   // Check parameters //
   //////////////////////


   if(progress) *progress = 0;
   IS_ZERO(elutionFlow, 2);
   IS_ZERO(crossFlow, 3);
   IS_ZERO(chWidth, 4);
   IS_ZERO(chLength, 5);


   //////////////////////////////////////////////////
   // adjust time axis according to leftOffsetTime //
   //////////////////////////////////////////////////

   for(double &t : times) t -= leftOffsetTime;
   voidPeakTime -= leftOffsetTime;

   //////////////////////////
   // calculate parameters //
   //////////////////////////


   if(reCalcChVolume) // use given voidPeak (corrected by offset) to get Volume
   {
      double var1 = (elutionFlow + crossFlow)/ crossFlow;
      double volumeFac = log((relFocusPoint / 100 - var1) / (1 - var1));
      chVolume = (crossFlow * voidPeakTime) / volumeFac;
   }
   else              // use given Volume to get voidPeak (corrected by offset)
   {
      double var1 = (elutionFlow + crossFlow)/ crossFlow;
      double volumeFac = log((relFocusPoint / 100 - var1) / (1 - var1));
      voidPeakTime = chVolume * volumeFac / crossFlow;
   }

   AF4Log::logText(string("Used void peak time: ").append(std::to_string(voidPeakTime)));

   double diffCoeff;
   double rMeas;

   uint maxIterations = 100;
   uint timeIndexBegin = indexFromOrderedVecD(times, voidPeakTime) + 1;


   evaluationStart = timeIndexBegin;
   uint timeIndexEnd = times.size();


   ///////////////////////////////////////////
   // Estimate total number of calculations //
   ///////////////////////////////////////////

   uint numberOfCalculations = timeIndexEnd - timeIndexBegin;

   //////////////////////////////
   // Start actual calculation //
   //////////////////////////////


   resDiffCoeff.resize(timeIndexEnd - timeIndexBegin);
   double lastResult = 1;
   bool ok = true;

   timeIndexEnd = times.size();
   lastResult = 1;
   uint calculationCounter = 0;
   for( uint timeIndex = timeIndexBegin; timeIndex < timeIndexEnd; timeIndex ++){
      // calculate the calculation progress:
      if(progress){
         *progress = (double) calculationCounter / (double) numberOfCalculations;
         if(*progress > 100.0) *progress = 100.0;
      }
      if(currentLine) *currentLine = timeIndex;
      rMeas = voidPeakTime / times.at(timeIndex);
      diffCoeff = calcDiffCoeff(rMeas, chVolume, crossFlow, chWidth, maxIterations, lastResult, timeIndex, &ok);
      if(!ok) return 6;
      resDiffCoeff[timeIndex - timeIndexBegin] = diffCoeff;
      lastResult = diffCoeff;
      calculationCounter++;
   }

   if(progress) *progress = 0.0;
   evaluated = true;

   return 0;

}

#define CORRECT_FROM_ZERO(value, timeIndex) {\
   if(isZero(value)){\
   value += 0.000000000000001;\
   if(displayZeroMessages)\
   AF4Log::logWarning(\
   QObject::tr(\
   "Forced division by zero was avoided at time index %1 by adjusting the divisor to 0.000000000000001")\
   .arg(timeIndex));\
   }\
   }

double AF4DiffEvaluator::calcDiffCoeff(double rMeas, double volume, double crossFlow,
                                       double chWidth, uint maxIterations, double lastResult, uint timeIndex, bool *ok)
{
   double diffCoeff = lastResult * 1000;
   double delta = diffCoeff / 2;
   double lambda;
   double twoLambda;
   double rCalc;
   double rDiff;
   double rmsDiff = 1.0;

   *ok = true;
   uint i=0;
   // bisection algorithm:
   while((rmsDiff > 0.00000000000000001) && (i < maxIterations)){
      CORRECT_FROM_ZERO(delta, timeIndex);
      CORRECT_FROM_ZERO(diffCoeff, timeIndex);
      lambda = (diffCoeff * volume) / (crossFlow * chWidth * chWidth);
      CORRECT_FROM_ZERO(lambda, timeIndex);
      twoLambda = 2 * lambda;
      rCalc = 6*lambda*(coth(1 / twoLambda) - twoLambda);
      rDiff = rCalc - rMeas;
      rmsDiff = rDiff * rDiff;

      if ( rDiff > 0 )
      {  // rCalc too big => omega has to be bigger;
         diffCoeff -= delta;
         delta /= 2;
      }
      else if ( rDiff < 0)
      {  // rCalc too small => omega has to be bigger;
         diffCoeff += delta;
         delta /= 2;
      }
      i++;
   }

   diffCoeff /= 60; // cm^2/min => cm^2/s
   return diffCoeff;
}

bool AF4DiffEvaluator::isZero(double x) const
{
   if ((x < 0.0) || (x > 0.0))
      return false;
   else {
      string message = string("Division by zero");
      message.append("; Premature Abortion of bisection forced,\n").
            append("while evaluating data point (time = ");
      AF4Log::logError(message);
      return true;
   }
}

int AF4DiffEvaluator::calcStokesRadii(vecD& diffCoeffs,
                                      double viscosity,
                                      double temperature)
{
   resStokesRadii.resize(diffCoeffs.size());
   viscosity /= 1000; // mPa * s (=centipoise) to SI-Unit (kg/(m*s))
   // Stokes-Einstein Equation
   for(uint j = 0; j < resStokesRadii.size(); ++j){
      double val = temperature * KB;
      val /=  6 * PI * viscosity * (diffCoeffs.at(j) / 1e4); // cm^2/s to m
      resStokesRadii[j] = val * 1e9;
   }
   rSCalculated = true;
   return 0;
}
