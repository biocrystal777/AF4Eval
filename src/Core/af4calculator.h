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
#include "../Gui/GeneralWidgets/ffflog.h"

/*! ***********************************************************
***
***  \class     AF4Calculator "src/Core/af4calculator.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/

class AF4Calculator
{
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

    AF4Calculator(const AF4Calculator& src) = delete;
    AF4Calculator& operator= (const AF4Calculator& src) = delete;
    AF4Calculator(AF4Calculator&& src) = delete;
    AF4Calculator& operator= (AF4Calculator&& src) = delete;

   /*!
    * \brief findIndexFromTime returns the index corresponding to a
    *        time on the 0th column of the data matrix ( = time)
    * \param time The time value of the index of interest
    * \returns an index
    */

public:
   uint findIndexFromTime(const vecD &timeVector, const double time) const;
protected:

   /*!
    * \brief coth returns the Hyperbolic Cotangent of the passed argument x
    * \param x passed argument x
    * \returns return value
    */
   double coth(double x) const;

   /*!
    * \brief isZero checks if a double x is Zero and return a corresponding bool
    * \param x
    * \return result
    */
   virtual bool isZero(double x) const;

   /*!
    * \brief logText logging function
    * \param message message to be logged
    */
   inline void logText(const std::string &message) const { FFFLog::logText(message); }

   /*!
    * \brief logWarning logging function
    * \param message message to be logged
    */
   inline void logWarning(const std::string &message) const { FFFLog::logWarning(message); }

   /*!
    * \brief logError logging function
    * \param message message to be logged
    */
   inline void logError(const std::string &message) const { FFFLog::logError(message); }
};

#endif // AF4CALCULATOR_H
