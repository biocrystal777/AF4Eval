#ifndef FFFDIFFEVALUATOR_H
#define FFFDIFFEVALUATOR_H


#include "fffcalculator.h"
#include <QDebug>
#include <QObject>

/**************************************************************
***
***   FFFDiffEvaluator
***
**************************************************************/

/*! \class FFFDiffEvaluator
 * \brief The FFFEvaluator class is used to find a Diffusion Coefficient corresponding to
 *        the values of the time axis each using a bisection. By this calculation, a conversion
 *        from time to diffusion coefficients is being performed.
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFDiffEvaluator final : public FFFCalculator
{
public:

   /*!
    * \brief FFFEvaluator default constructor for the class
    */
   explicit FFFDiffEvaluator();

   /*! \brief standard destructor
    */
   ~FFFDiffEvaluator();

    FFFDiffEvaluator(const FFFDiffEvaluator& src) = delete;
    FFFDiffEvaluator& operator= (const FFFDiffEvaluator& src) = delete;
    FFFDiffEvaluator(FFFDiffEvaluator&& src) = delete;
    FFFDiffEvaluator& operator= (FFFDiffEvaluator&& src) = delete;


   /*!
    * \brief evaluate
    * \param leftOffsetTime
    * \param voidPeak
    * \param elutionFlow
    * \param crossFlow
    * \param chWidth
    * \param toProcess
    * \param progress
    * \returns errorCode
    */
   int evalDiffCoeffs(vecD &times,
                      double leftOffsetTime,
                      double voidPeakTime,
                      double elutionFlow,
                      double crossFlow,
                      double chWidth,
                      double chVolume,
                      double relFocusPoint,
                      double chLength,
                      bool recalcChVolume,
                      bool displayZeroMessages = true,
                      double* progress = nullptr,
                      uint* currentLine = nullptr);

   // Simple conversion via stokes einstein equation
   int calcStokesRadii(vecD& diffCoeffs,
                             double viscosity,
                             double temperature);



   /*!
    * \brief getResDiffCoeff returns the calculated Diffusion Coefficients.
    *        Values are stored as vector on heap. Therefore, the vector has
    *        to be deleted later externally if it is not longer neeeded anymore!
    * \return values pointer to the data on heap
    */
   inline vecD getResDiffCoeff() const
   {
      if(!evaluated) FFFLog::logError(std::string("Data have to be evaluated before reading is possible!"));
      return resDiffCoeff;
   }

   inline vecD getResStokesRadii() const
   {
      if(!rSCalculated) FFFLog::logError(std::string("Stokes-Radii have to be computed before!"));
      return resStokesRadii;
   }

   /*!
    * \brief getEvaluationStart returns the index, where the evaluation has started
    *        (i.e. close behind the voidPeak time)
    * \returns index value
    */
   inline uint getEvaluationStart() const
   {
      if(!evaluated) FFFLog::logError(std::string("Data have to be evaluated before there will be an an evaluation Start!"));
      return evaluationStart;
   }

protected:

   bool displayZeroMessages;

   /*!
    * \brief calcDiffCoeff calculates a diffusion coefficient by bisection
    *        and returns the value on the heap
    * \param rMeas
    * \param volume
    * \param crossFlow
    * \param chWidth
    * \param maxIterations
    * \param lastResult
    * \param ok
    * \return
    */
   double calcDiffCoeff(double rMeas, double volume, double crossFlow,
                        double chWidth, uint maxIterations, double lastResult, uint timeIndex, bool *ok);

   /*!
    * \brief isZero checks if a double is 0.0 (used to prevent dividing by zero)
    * \param x double to be checked
    * \param silent if stated as true, the method won't log a message if a value becomes zero
    * \return bool if the the double was 0.0
    */
   virtual bool isZero(double x) const;

   bool evaluated = false;
   bool rSCalculated = false;

   uint evaluationStart = 0;

   vecD resDiffCoeff;
   vecD resStokesRadii;
};

#endif // FFFDIFFEVALUATOR_H
