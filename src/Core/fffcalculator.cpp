#include "fffcalculator.h"

using std::vector;
using std::string;

uint FFFCalculator::findIndexFromTime(const vecD &timeVector, const double time) const
{

   uint index(0);
   double oldDiff = std::numeric_limits<double>::max();   
   for(uint i = 0; i < timeVector.size(); ++i){
      if(timeVector.at(i) > 0.0){
         double newDiff = std::abs(timeVector.at(i) - time);
         if(newDiff < oldDiff){
            index = i;
            oldDiff  = newDiff;
         }
      }
   }
   return index;
}

double FFFCalculator::coth(double x) const
{
   return (1 / std::tanh(x));
}

bool FFFCalculator::isZero(double x) const
{
   if ((x < 0.0) || (x > 0.0))
      return false;
   else {
      FFFLog::logError(string("Division by zero; operation aborted."));
      return true;
   }
}

