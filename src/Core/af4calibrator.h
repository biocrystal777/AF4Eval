#ifndef AF4CALIBRATOR_H
#define AF4CALIBRATOR_H
#include "af4evalmacros.h"
#include "af4calculator.h"

enum struct CalibErrorCode : uint
{
   noError           = 0,
   voidTimeZero      = 1,
   eluFlowZero       = 2,
   eluTimeZero       = 3,
   crossFlowZero     = 4,
   diffCoeffZero     = 5,
   voidTimeTooSmall  = 6,
   eluTimeTooSmall   = 7,
   ParamsNotChecked  = 8
};

struct CalibResult
{
   double width; // in cm
   double volume; // in mm
   CalibErrorCode errorCode;
   double sqDelta;
};

/*! ***********************************************************
***
***  \class     AF4Calibrator "src/Core/af4calibrator.h"
***  \author    Benedikt Häusele
***  \details   The numbers of the commented steps within the calibration functions refer
***             to the steps in the corresponding publication
***             "Automated batch evaluation of diffusion coefficients from Asymmetrical
***              Flow Field-Flow Fractionation data via void peak determination I –
***              Theory and measurement uncertainties (Supplementary information)"
***              (pp. 11-16)
***  \version   1.2
***  \date      2020
***  \todo      Restructure and parts like numerical CF calculations from extract functions!
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/

class AF4Calibrator final : public AF4Calculator
{

public:

   /*!
    * \brief FFFCalibrator is the standard constructor
    * \param
    */
   AF4Calibrator(const ChannelDims &d,
                 const ParametersForCalibration &p) :
      chDims(d), params(p) {}

   /*!
    * \brief ~FFFCalibrator Default destructor virtual
    */
   virtual ~AF4Calibrator(){}

   /*!
    * \brief calibrate the actual calibration function
    * \param leftOffsetTime offset that has to be substrated
    *        by all other time values [min]
    * \param voidPeakTime time of the voidpeak [min]
    * \param elutionFlow integrated flow speed [ml/min] of the elution direction
    * \param elutionTime elution time of the eluated substance
    *        (i.e. the time with the maximum signal of the substance with
    *        the diffusion Coefficient passed below [min]
    * \param crossFlow speed of the crossflow [ml/min]
    * \param diffCoeficient diffusion Coefficient for the
    *        substance eluated at elutionTime [cm^2/s]
    * \param ok determines whether the calculation has be performed successfully or not
    * \return
    */
   CalibErrorCode checkParameters();

   /*!
    * \brief calibrate_classic implements the basic AF4 calibrtaion using tvoid, D and z%
    *        for volume calibration
    * \return
    */
   CalibResult calibrate_classic();

   /*!
    * \brief calibrate_approxGeo implements a variant of calibrate_classic under consideration o
    *         of the complete
    * \return
    */
   CalibResult calibrate_approxGeo();

   /*!
    * \brief calibrate_geometric() implements a calibration using the geometric properties and skips the
    *        focus information
    * \return
    */
   CalibResult calibrate_geometric();

   /*!
    * \brief calibrate_hydrodynamic calibration using the direct relationship of tvoid and w via a linear conversion factor
    *                               which encapsulated all geometric and hydrodynamic information without external calibration measurement
    * \return
    */
   CalibResult calibrate_hydrodynamic();

   /*!
    * \brief calibrate_tVoidFree calibration using geometrical propertioes and a linear conversion factor in order to substitute tvoid
    * \return
    */
   CalibResult calibrate_tVoidFree();

private:
   const ChannelDims chDims;
   const ParametersForCalibration params;
   bool paramsChecked = false;

   NO_COPY_MOVE_CTORS(AF4Calibrator)
};


#endif // AF4CALIBRATOR_H
