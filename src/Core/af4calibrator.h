#ifndef AF4CALIBRATOR_H
#define AF4CALIBRATOR_H
#include "af4evalmacros.h"
#include "af4calculator.h"

enum struct CalibErrorCode : uint {
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

struct CalibResult {
   double width;
   double volume;
   CalibErrorCode errorCode;
   double sqDelta;
};

/*! ***********************************************************
***
***  \class     AF4Calibrator "src/Core/af4calibrator.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
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
    * \brief ~FFFCalibrator Default destructor
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

   CalibResult calibrate_classic();
   CalibResult calibrate_geometric();
   CalibResult calibrate_hydrodynamic();

   //inline double getChWidth() const { return w;}
   //inline double getHydrodynVolume() const { return V0;}
   //inline double getGeometVolume() const { return Vg;}

   /*
   void calcGeometVolume(
         const double L1,
         const double L2,
         const double L3,
         const double L,
         const double b0,
         const double bL,
         const double zL
         );

         */
private:
   const ChannelDims chDims;
   const ParametersForCalibration params;
   bool paramsChecked = false;


   double w = 0.0;
   double rmsDiff = 0.0;
   double delta = 0.0;
   double V0 = 0.0;
   double Vg = 0.0;

//   bool zeroErrorMessage(int pos) const;

   /*!
    * \brief isZero
    * \param x determines whether a double is 0.0 or not
    * \return bool
    */
   //inline bool isZero(double x, int pos) const {
   //   return !(std::abs(x) >= std::numeric_limits<double>::min()) ? false : zeroErrorMessage(pos);
   //}
   NO_COPY_ASSIGNMENT_CTORS(AF4Calibrator)
};

#endif // AF4CALIBRATOR_H
