#ifndef FFFCALIBRATOR_H
#define FFFCALIBRATOR_H

#include "fffcalculator.h"

/**************************************************************
***
***   FFFCalibrator
***
**************************************************************/

/*!
 * \class FFFCalibrator
 * \brief The FFFCalibrator class is used to calculate
 *        omega (channelWidth) from a set of parameters
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */


class FFFCalibrator final : public FFFCalculator
{

public:

   /*!
    * \brief FFFCalibrator is the standard constructor
    * \param data can be passed, but is not used at the moment
    */
   explicit FFFCalibrator();
   FFFCalibrator(const FFFCalibrator& src) = delete;
   FFFCalibrator& operator= (const FFFCalibrator& src) = delete;
   FFFCalibrator(FFFCalibrator&& src) = delete;
   FFFCalibrator& operator= (FFFCalibrator&& src) = delete;


   /*!
    * \brief ~FFFCalibrator Default destructor
    */
   ~FFFCalibrator();

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

#endif // FFFCALIBRATOR_H
