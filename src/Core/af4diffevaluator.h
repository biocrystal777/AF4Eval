#ifndef AF4DIFFEVALUATOR_H
#define AF4DIFFEVALUATOR_H

#include "af4calculator.h"
#include <QDebug>
#include <QObject>

/*! ***********************************************************
***
***  \class     AF4DiffEvaluator "src/Core/af4diffevaluator.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/

class AF4DiffEvaluator final : public AF4Calculator
{
public:

   /*!
    * \brief FFFEvaluator default constructor for the class
    */
   explicit AF4DiffEvaluator() {}

   /*! \brief standard destructor
    */
   ~AF4DiffEvaluator(){}

    AF4DiffEvaluator(const AF4DiffEvaluator& src) = delete;
    AF4DiffEvaluator& operator= (const AF4DiffEvaluator& src) = delete;
    AF4DiffEvaluator(AF4DiffEvaluator&& src) = delete;
    AF4DiffEvaluator& operator= (AF4DiffEvaluator&& src) = delete;


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

   bool displayZeroMessages = true;
   bool evaluated = false;
   bool rSCalculated = false;

   uint evaluationStart = 0;

   vecD resDiffCoeff;
   vecD resStokesRadii;
};

#endif // AF4DIFFEVALUATOR_H
