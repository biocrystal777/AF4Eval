#ifndef AF4SLSEVALUATOR_H
#define AF4SLSEVALUATOR_H
#include "af4calculator.h"
#include "../Gui/GeneralWidgets/af4log.h"

/*! ***********************************************************
***
***  \class     AF4SLSEvaluator "src/Core/af4slsevaluator.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/

class AF4SLSEvaluator final : public AF4Calculator
{
public:

   /*!
    * \brief FFFEvaluator default constructor for the class
    */
   AF4SLSEvaluator(){}

   /*!
    * \brief FFFEvaluator default destructor for the class
    */
   ~AF4SLSEvaluator(){}

    AF4SLSEvaluator(const AF4SLSEvaluator& src) = delete;
    AF4SLSEvaluator& operator= (const AF4SLSEvaluator& src) = delete;
    AF4SLSEvaluator(AF4SLSEvaluator&& src) = delete;
    AF4SLSEvaluator& operator= (AF4SLSEvaluator&& src) = delete;



    int evalUVVis_MALLS_partZimmplot(const vecD &uvVisData,
                                     matD &rayleighRatios,
                                     const std::vector<bool> &useDataPoint,
                                     const vecD &epsilon,
                                     const vecD &dnDcs,
                                     const double laserWl,
                                     const double riSolvent,
                                     const double calibRi,
                                     const double concCap,
                                     const double calibMalls,
                                     const vecD &calibMallsAngles,
                                     const std::vector<bool> &mallsDetectorRelevant,
                                     vecD *secondVirCoeffs = nullptr,
                                     vecD *thirdVirCoeffs = nullptr
                                     );

   /*!
    * \brief FFFEvaluator evaluates data, concentration determination
    *        via RI detector, partial Zimm-Plot.
    *        For the resulting data, vector are created on heap and
    *        will have to be deleted manually OUTSIDE of this class!
    * \param riData data from the RI detector
    * \param rayleighRatios data from the MALLS detector (8 angles)
    * \param useDataPoint indicates the parts on the time axis
    *        from the data set that shall be evaluated
    * \param dnDcs refractive index increments for each time
    * \param laserWl wavelength of the lase
    * \param riSolvent refractive Index of the solvent
    * \param calibRi calibration constant of the riDetector,
    *        indicates
    * \param concCap minimal concentration for evaluation,
    *        times with smaller concentrations are skipped
    *        and result in 0
    * \param calibMalls calibration constant of the MALLS detector
    * \param mallsDetectorRelevant indicates which angles shall be
    *        considered for evaluation
    * \param secondVirCoeffs second Virial Coefficients for all times
    * \param thirdVirCoeffs third Virial Coefficients
    * \return errorCode
    */
   int evalRiMALLS_partZimmplot(const vecD &riData,
                                matD &rayleighRatios,
                                const std::vector<bool> &useDataPoint,
                                const vecD &dnDcs,
                                const double laserWl,
                                const double riSolvent,
                                const double calibRi,
                                const double concCap,
                                const double calibMalls,
                                const vecD &calibMallsAngles,
                                const std::vector<bool> &mallsDetectorRelevant,
                                vecD *secondVirCoeffs = nullptr,
                                vecD *thirdVirCoeffs = nullptr
         );

   /*!
    * \brief FFFEvaluator calculates the ratio of RG and RS
    */
   int calcRho(const vecD &stokesRadii,
               const vecD &gyrRadii,
               vecD &rhos
               );

   /*!
    * \brief getMolWeights returns the molecular weights from the evaluation
    * \param exists test parameter if an evaluation has already been conducted
    * \return molecular weights
    */
   vecD getMolWeights(bool *exists) const;

   /*!
    * \brief getRadGyrs returns the radii of gyration from the evaluation
    * \param exists test parameter if an evaluation has already been conducted
    * \return molecular weights
    */
   vecD getRadGyrs(bool *exists) const;

   vecD getConcentrations(bool *exists = nullptr) const;

private:



   vecD concentrations;
   bool concetrationsCalculated = false;
   vecD molWeights;
   bool molWeightsCalculated = false;
   vecD radGyrs;
   bool radGyrsCalculated = false;

   /*!
    * \brief calcXForZimmplot returns a matrix angles x time, containing the respective values for the linear regression for
    * \param concentrations
    * \param mallsDetectorRelevant
    * \param useDataPoint
    * \param secondVirCoeffs
    * \param thirdVirCoeffs
    * \param errorCode
    * \return matrix with
    */

   matD calcXForZimmplot(
         const vecD &concentrations,
         const std::vector<bool> &mallsDetectorRelevant,
         const std::vector<bool> &useDataPoint,
         vecD *secondVirCoeffs = nullptr,
         vecD *thirdVirCoeffs = nullptr,
         int *errorCode = nullptr
         );


  matD  calcYForZimmplot(const vecD &concentrations,
                         const matD &rayleighRatios,
                         const std::vector<bool> &mallsDetectorRelevant,
                         const std::vector<bool> &useDataPoint,
                         const vecD &dnDcs,
                         const double laserWl,
                         const double riSolvent,
                         const double concCap,
                         int *errorCode = nullptr
         );


   /*!
    * \brief calcConcFromRi creates a new vector with concentrations on heap
    * \param riData data from the RI detector
    * \param useDataPoint useDataPoint indicates the parts on the time axis
    *        from the data set that shall be evaluated
    * \param dnDcs refractive index increments
    * \param calibRIConstant calibration constant of the RI detector
    * \param resConcentrations pointer to the vector of concentrations after calculation
    * \return error Code
    *
    * \todo make resConcentrations return value!
    */
   int calcConcFromRi(const vecD &riData,
                      const std::vector<bool> &useDataPoint,
                      const vecD &dnDcs,
                      double calibRiConstant,
                      vecD &resConcentrations);

   int calcConcFromUV_Vis(const vecD &absorbanceData,
                          const std::vector<bool> &useDataPoint,
                          const vecD &epsilon,
                          const double optPathLength,
                          vecD &resConcentrations,
                          const double unitFactor = 1.0);

   /*!
    * \brief calcConcFromRi creates a new vector with concentrations;
    * \param riData data from the RI detector
    * \param useDataPoint useDataPoint indicates the parts on the time axis
    *        from the data set that shall be evaluated
    * \param calibRIConstant calibration constant of the RI detector
    * \param riSolvent refractive index of the solvent
    * \param resRefIndices vector of refractive indices after calculation
    * \return error Code
    *
    * \todo make the vector resrRefIndices return value!
    *
    *
    */
   int calcTotalRefIndices(const vecD &riData,
                           const std::vector<bool> &useDataPoint,
                           double calibRiConstant,
                           double riSolvent,
                           vecD &resRefIndices);

   /*!
    * \brief linearRegression calculates a linear regression for a given data set Y(X)
    * \param xValues values of the x-axis
    * \param yValues values of the y-axis
    * \param pointRelevant indicates which point of x and y shall be considered for the regression
    * \param slope of the regression line
    * \param offset pffest of the regression line
    * \return error Code
    *
    * \todo return slope and offset as tuple with structured binding when changing to C++17!
    *
    */
   int linearRegression(const vecD &xValues,
                        const vecD &yValues,
                        const std::vector<bool> &pointRelevant,
                        double *slope,
                        double *offset) const;

   /*!
    * \brief setNegativeToZero correct negative values and sets them so 0
    * \param values input values to be corrected
    * \return error Code
    */
   int setNegativeToZero(vecD &values);

};

#endif // FFFSLSEVALUATOR_H
