#include "af4calculator.h"

using std::vector;
using std::string;

uint AF4Calculator::indexFromOrderedVecD(const vecD &v, const double target) const
{
   uint index = std::distance( v.begin(), std::lower_bound(v.begin(), v.end(), target) );
   if( index && std::abs(v[index] - target ) < std::abs(v[index-1] -target))
      return index;
   else
      return index-1;
}

double AF4Calculator::RToLambda(const double R)
{
   const double lambdaMin{1e-8};
   const double lambdaMax{1e2};
   double lambda = (lambdaMin + lambdaMax) * 0.5;
   double delta  = (lambdaMax - lambdaMin) * 0.25;
   for(uint i = 0; i < 50; ++i){
      // calculate test variable with test λ
      double RTest = coth( 1.0 / (2.0 * lambda ) );
      RTest -= 2.0 * lambda;
      RTest *= 6.0 * lambda;
      // adapt with test λ
      if      (RTest > R) lambda += delta;
      else if (RTest < R) lambda -= delta;
      else                break;
      delta *= 0.5;
   }
   return lambda;
}

bool AF4Calculator::isZero(double x) const
{
   if(x == 0.0) return true;
   else         return false;

   /*
   if ((x < 0.0) || (x > 0.0))
      return false;
   else {
      AF4Log::logError(string("Division by zero; operation aborted."));
      return true;
   }
   */
}

