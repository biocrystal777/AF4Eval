#include "af4calculator.h"

using std::vector;
using std::string;

uint AF4Calculator::findIndexFromTime(const vecD &timeVector, const double time) const
{
   uint index = std::distance( timeVector.begin(), std::lower_bound(timeVector.begin(), timeVector.end(), time) );
   if( index && std::abs(timeVector[index] - time  ) < std::abs(timeVector[index-1] -time))
      return index;
   else
      return index-1;
}

bool AF4Calculator::isZero(double x) const
{
   if ((x < 0.0) || (x > 0.0))
      return false;
   else {
      AF4Log::logError(string("Division by zero; operation aborted."));
      return true;
   }
}

