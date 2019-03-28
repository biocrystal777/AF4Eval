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

// simple math (return βξ²+γξ+δ)
double AF4Calculator::Pol_CF(const double beta,
                             const double gamma,
                             const double delta,
                             const double xi)
{
   return beta * squared(xi) + gamma * xi + delta;
}

// nothing to comment, it's just math, I can't help it
double AF4Calculator::IntPosDisc_CF_i(const double alpha,
                                      const double beta,
                                      const double gamma,
                                      const double delta,
                                      const double discr,
                                      const double s0,
                                      const double s1,
                                      const double m_i)
{

   qDebug() << "Int pos Disc s0 to s1: " << s0 << s1 ;
   double CF_i  = log( Pol_CF(beta, gamma, delta, s1) / Pol_CF(beta, gamma, delta, s0) );
   qDebug() << " Pol s0 " << Pol_CF(beta, gamma, delta, s0);
   qDebug() << " Pol s1 " << Pol_CF(beta, gamma, delta, s1);
   qDebug() << "ln Pol/lnPol " << CF_i;
   CF_i /= 2.0 * beta;
   const double tmp0 = sqrt(discr);
   qDebug() << "sqrt disc " << tmp0;
   const double tmp1 = atan( ( 2.0 * beta * s1 + gamma ) / tmp0 );
   qDebug() << "atan s1" << tmp1;
   const double tmp2 = atan( ( 2.0 * beta * s0 + gamma ) / tmp0 );
   qDebug() << "atan s0" << tmp2;
   const double tmp3 =  ( 2.0 / tmp0 ) * ( alpha - gamma / beta ) * ( tmp1 - tmp2 );
   qDebug() << "tmp3" << tmp3;
   qDebug() << "___________";
   CF_i += tmp3;
   CF_i *= m_i;
   return CF_i;
}

// see comment above
double AF4Calculator::IntNegDisc_CF_i(const double alpha,
                                      const double beta,
                                      const double gamma,
                                      const double delta,
                                      const double discr,
                                      const double s0,
                                      const double s1,
                                      const double m_i)
{
   qDebug() << "Int NEG Disc s0 to s1: " << s0 << s1;
   double CF_i  = log( Pol_CF(beta, gamma, delta, s1) / Pol_CF(beta, gamma, delta, s0) );
   qDebug() << " Pol s0 " << Pol_CF(beta, gamma, delta, s0);
   qDebug() << " Pol s1 " << Pol_CF(beta, gamma, delta, s1);
   qDebug() << "ln Pol/lnPol " << CF_i;
   CF_i /= 2.0 * beta;

   const double tmp0 = sqrt(-discr);
   qDebug() << "sqrt(-discr)" << CF_i;
   const double twoBeta = 2.0 * beta;
   double tmp1 = twoBeta * s1 + gamma;
   tmp1       *= twoBeta * s0 + gamma;   
   tmp1        = tmp0 - (tmp1/tmp0);
   tmp1        = twoBeta * (s1 - s0) / tmp1;
   qDebug() << "arg atanh" << tmp1;
   tmp1        = atanh(tmp1);
   tmp1        = (2.0 / tmp0) * (alpha - gamma/beta) * tmp1;
   CF_i       -= tmp1;
   CF_i       *= m_i;
   qDebug() << "___________";
   return CF_i;
}

// see comment above
double AF4Calculator::Int_CF_1(const double beta,
                               const double delta,
                               const double s0,
                               const double s1,
                               const double m_1)
{
   const double tmp0 = delta / beta;
   double CF_1 = tmp0 + squared(s1);
   CF_1       /= tmp0 + squared(s0);
   qDebug() << "CF1 log arg" << CF_1;
   CF_1        = m_1 / (2.0 * beta) * log( CF_1 );
   return CF_1;
}

double AF4Calculator::RToLambda(const double R, double *RMS)
{
   const double lambdaMin{1e-4};
   const double lambdaMax{1e2};
   double lambda = (lambdaMin + lambdaMax) * 0.5;
   double delta  = (lambdaMax - lambdaMin) * 0.25;
   double RTest {0.0};
   for(uint i = 0; i < 100; ++i){
      // calculate test variable with test λ
      RTest = coth( 1.0 / (2.0 * lambda ) );
      RTest -= 2.0 * lambda;
      RTest *= 6.0 * lambda;
      // adapt with test λ
      if      (RTest > R) lambda -= delta;
      else if (RTest < R) lambda += delta;
      else                break;
      delta *= 0.5;
      qDebug() << "lambda" << i << lambda;
   }   
   if(RMS) *RMS = squared(RTest - R);
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

