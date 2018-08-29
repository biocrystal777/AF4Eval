#ifndef FFFCALCULATOR_H
#define FFFCALCULATOR_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include "fffconstants.h"
#include "fffdatatypes.h"
#include "fffparameterstructs.h"
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


typedef std::vector<vecD*> vectorMatrix;

class FFFCalculator
{
protected:
   /*!
    * \brief FFFCalculator is the standard constructor
    * \param data is the data matrix to be processed by the class
    */
   explicit FFFCalculator(){}

   /*!
    * \brief Default Destructor
    */
   virtual ~FFFCalculator(){}

    FFFCalculator(const FFFCalculator& src) = delete;
    FFFCalculator& operator= (const FFFCalculator& src) = delete;
    FFFCalculator(FFFCalculator&& src) = delete;
    FFFCalculator& operator= (FFFCalculator&& src) = delete;

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

#endif // FFFCALCULATOR_H
