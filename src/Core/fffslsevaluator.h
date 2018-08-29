#ifndef FFFSLSEVALUATOR_H
#define FFFSLSEVALUATOR_H
#include "fffcalculator.h"
#include "../Gui/GeneralWidgets/ffflog.h"


/**************************************************************
***
***   FFFSLSEvaluator
***
**************************************************************/

/*! \class FFFSLSEvaluator
 * \brief The FFFSLSEvaluator class is used to evaluate data from
 *        a light scattering detector with hard coded angle positions
 *        At the current version, only Zimm-Plot evaluation in combination
 *        with a signal from a RI detector is possible.
 * \author Benedikt Haeusele
 * \version 1.0
 * \date July 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFSLSEvaluator final : public FFFCalculator
{
public:

   /*!
    * \brief FFFEvaluator default constructor for the class
    */
  explicit FFFSLSEvaluator();

   /*!
    * \brief FFFEvaluator default destructor for the class
    */
   ~FFFSLSEvaluator();

    FFFSLSEvaluator(const FFFSLSEvaluator& src) = delete;
    FFFSLSEvaluator& operator= (const FFFSLSEvaluator& src) = delete;
    FFFSLSEvaluator(FFFSLSEvaluator&& src) = delete;
    FFFSLSEvaluator& operator= (FFFSLSEvaluator&& src) = delete;



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

   vecD angles = {21.47 * PI / 360.0,
                  34.91 * PI / 360.0,
                  53.13 * PI / 360.0,
                  67.70 * PI / 360.0,
                  88.90 * PI / 360.0,
                  109.88 * PI / 360.0,
                  131.30 * PI / 360.0,
                  149.31 * PI / 360.0
                 };

   vecD concentrations;
   bool concetrationsCalculated = false;
   vecD molWeights;
   bool molWeightsCalculated = false;
   vecD radGyrs;
   bool radGyrsCalculated = false;

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
    * \brief calcConcFromRi creates a new vector with concentrations on heap
    * \param riData data from the RI detector
    * \param useDataPoint useDataPoint indicates the parts on the time axis
    *        from the data set that shall be evaluated
    * \param calibRIConstant calibration constant of the RI detector
    * \param riSolvent refractive index of the solvent
    * \param resRefIndices pointer to the vector of refractive indices after calculation
    * \return error Code
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
