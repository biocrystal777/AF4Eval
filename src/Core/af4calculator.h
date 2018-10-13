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
   inline double coth(double x)  const {  return (1.0 / std::tanh(x)); }


   /*!
    * \brief  RToLambda implements a simple bisection method to convert
    *         R to λ with the relationship R = 6λ( coth( 1/(2λ) - 2λ ) )
    * \param  R
    * \return λ
    */
   double RToLambda(const double R);

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
