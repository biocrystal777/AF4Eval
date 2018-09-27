#include "af4calculator.h"

using std::vector;
using std::string;

uint AF4Calculator::indexFromOrderedVector(const vecD &v, const double target) const
{
   uint index = std::distance( v.begin(), std::lower_bound(v.begin(), v.end(), target) );
   if( index && std::abs(v[index] - target ) < std::abs(v[index-1] -target))
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

