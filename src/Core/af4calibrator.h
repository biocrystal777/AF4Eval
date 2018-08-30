#ifndef AF4CALIBRATOR_H
#define AF4CALIBRATOR_H

#include "af4calculator.h"

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
   AF4Calibrator(){}
   AF4Calibrator(const AF4Calibrator& src) = delete;
   AF4Calibrator& operator= (const AF4Calibrator& src) = delete;
   AF4Calibrator(AF4Calibrator&& src) = delete;
   AF4Calibrator& operator= (AF4Calibrator&& src) = delete;


   /*!
    * \brief ~FFFCalibrator Default destructor
    */
   ~AF4Calibrator(){}

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

   bool calibrate(const ChannelDims &d,
                  const ParametersForCalibration &p);

   inline double getChWidth() const { return w;}
   inline double getHydrodynVolume() const { return V0;}
   inline double getGeometVolume() const { return Vg;}

   void calcGeometVolume(
         const double L1,
         const double L2,
         const double L3,
         const double L,
         const double b0,
         const double bL,
         const double zL
         );


protected:
   double w = 0.0;
   double rmsDiff = 0.0;
   double delta = 0.0;
   double V0 = 0.0;
   double Vg = 0.0;

   /*!
    * \brief isZero
    * \param x determines whether a double is 0.0 or not
    * \return bool
    */
   bool isZero(double x, int pos) const;
};

#endif // AF4CALIBRATOR_H
