#ifndef AF4CALCULATOR_H
#define AF4CALCULATOR_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include "./af4constants.h"
#include "./af4datatypes.h"
#include "./af4parameterstructs.h"
#include "../Gui/GeneralWidgets/af4log.h"

/*! ***********************************************************
***
***  \class     AF4Calculator "src/Core/af4calculator.h"
***  \author    Benedikt Häusele
***  \brief     collection of general AF4-related equations
***             and numerical solutions
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/

class AF4Calculator
{
public:
   /*!
    * \brief indexFromOrderedVector returns the index corresponding to a
    *        time on the 0th column of the data matrix ( = time)
    * \param time The time value of the index of interest
    * \returns an index
    */
   uint indexFromOrderedVecD(const vecD &v, const double target) const;

protected:
   /*!
    * \brief FFFCalculator is the standard constructor
    * \param data is the data matrix to be processed by the class
    */
   AF4Calculator(){}

   /*!
    * \brief Default Destructor
    */
   virtual ~AF4Calculator(){}

   /*!
    * \brief coth returns the hyperbolic cotangent of the passed argument x
    * \param x passed argument x
    * \returns return value
    */
   inline double coth(double x) const {  return (1.0 / std::tanh(x)); }

   /*!
    * \brief squared
    * \param x
    * \return returns x²
    */
   inline double squared(double x) const { return x * x; }

   /*!
    * \brief (S1) Pol_CF calculates the squared polynom βξ²+γξ+δ for
    *        for calcuclations making use of the "hydrodynamic" approach
    * \param beta
    * \param gamma
    * \param delta
    * \param xi
    * \return P
    */
   double Pol_CF(const double beta,
                 const double gamma,
                 const double delta,
                 const double xi);
   /*!
    * \brief  IntPosDisc_CF (S2) calculates a conversion factor summand CF_i as integral over dξ
    *         with the nasty integrand function case that the discriminant Δ_i > 0
    *         for calcuclations making use of the "hydrodynamic" approach
    * \param alpha
    * \param beta
    * \param gamma
    * \param delta
    * \param discr   discriminant of the integrands expression
    * \param s0      lower integral border (position between ξ=0 and ξ=L and s0 < s1)
    * \param s1      upper integral border (position between ξ=0 and ξ=L and s1 > s0)
    * \param mi      i-th slope
    * \return
    */
   double IntPosDisc_CF_i(const double alpha,
                          const double beta,
                          const double gamma,
                          const double delta,
                          const double discr,
                          const double s0,
                          const double s1,
                          const double m_i);

   /*!
    * \brief  IntNegDisc_CF (S3) calculates a conversion factor summand CF_i as integral over dξ
    *         with the nasty integrand function case that the discriminant Δ_i < 0
    *         for calcuclations making use of the "hydrodynamic" approach
    * \param alpha
    * \param beta
    * \param gamma
    * \param delta
    * \param discr   discriminant of the integrands expression
    * \param s0      lower integral border (position between ξ=0 and ξ=L and s0 < s1)
    * \param s1      upper integral border (position between ξ=0 and ξ=L and s1 > s0)
    * \param mi      i-th slope
    * \return CF_i (i.e. CF_2 or CF_3)
    */
   double IntNegDisc_CF_i(const double alpha,
                          const double beta,
                          const double gamma,
                          const double delta,
                          const double discr,
                          const double s0,
                          const double s1,
                          const double m_i);

   /*!
    * \brief Int_CF_1 (S4) calculates the integral for channel section 1 over dξ
    *        with the "simplified" intgerand funtction
    *        for calcuclations making use of the "hydrodynamic" approach
    * \param beta
    * \param delta
    * \param s0
    * \param s1
    * \param m1
    * \return CF_1
    */
   double Int_CF_1(const double beta,
                   const double delta,
                   const double s0,
                   const double s1,
                   const double m_1);
   /*!
    * \brief  RToLambda implements a simple bisection method to convert
    *         R to λ with the relationship R = 6λ( coth( 1/(2λ) - 2λ ) )
    * \param  R
    * \return λ
    */
   double RToLambda(const double R, double *RMS = nullptr);

   /*!
    * \brief  isZero checks if a double x is Zero and return a corresponding bool
    * \param  x
    * \return result
    */
   virtual bool isZero(double x) const;

   /*!
    * \brief logText logging function
    * \param  message to be logged
    */
   void logText(const std::string &message) const { AF4Log::logText(message); }

   /*!
    * \brief logWarning logging function
    * \param  message to be logged
    */
   void logWarning(const std::string &message) const { AF4Log::logWarning(message); }

   /*!
    * \brief logError logging function
    * \param  message to be logged
    */
   void logError(const std::string &message) const { AF4Log::logError(message); }

private:
   AF4Calculator(const AF4Calculator& src) = delete;
   AF4Calculator& operator= (const AF4Calculator& src) = delete;
   AF4Calculator(AF4Calculator&& src) = delete;
   AF4Calculator& operator= (AF4Calculator&& src) = delete;

};

#endif // AF4CALCULATOR_H
