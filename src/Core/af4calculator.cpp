#include "af4calculator.h"

using std::vector;
using std::string;

uint AF4Calculator::findIndexFromTime(const vecD &timeVector, const double time) const
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

double AF4Calculator::coth(double x) const
{
   return (1 / std::tanh(x));
}

bool AF4Calculator::isZero(double x) const
{
   if ((x < 0.0) || (x > 0.0))
      return false;
   else {
      FFFLog::logError(string("Division by zero; operation aborted."));
      return true;
   }
}

