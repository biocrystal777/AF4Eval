#include "fffcalibrator.h"
using std::to_string;

FFFCalibrator::FFFCalibrator()
{
}

FFFCalibrator::~FFFCalibrator()
{
}

#define IS_ZERO(value, pos) {if(isZero(value, pos)) return false;}

bool FFFCalibrator::calibrate(const ChannelDims &d,
                              const ParametersForCalibration &p)
{


   IS_ZERO(p.voidPeakTime, 1);
   IS_ZERO(p.elutionFlow, 2);
   IS_ZERO(p.elutionTime, 3);
   IS_ZERO(p.crossFlow, 4);
   IS_ZERO(p.diffCoeff, 5);


   // check timeParameters
   if(p.leftOffsetTime >= p.voidPeakTime) return false;
   if(p.voidPeakTime >= p.elutionTime) return false;


   // adjust time axis according to leftOffsetTime
   const double tvoid      = p.voidPeakTime - p.leftOffsetTime;  // remove focussing time offset
   const double te         = p.elutionTime - p.leftOffsetTime;   // remove focussing time offset
   const double D          = p.diffCoeff * 60;                   // cm^2/s => cm^2/min
   const double Ve         = p.elutionFlow;
   const double Vc         = p.crossFlow;
   const double z_perc     = p.relFocusPoint / 100;                // percentage to ratio

   {
   double flowRatio = (Ve + Vc)/ Vc;
   double hydVolumeDivisor = log((z_perc - flowRatio) / (1 - flowRatio));
   V0 = (Vc * tvoid) / hydVolumeDivisor;
   }

   double rMeas = tvoid / te;
   this->rmsDiff = 1.0;
   double minWidth = 0.00001;
   double maxWidth = 10;
   w = (minWidth + maxWidth)/ 2;
   delta = (maxWidth - minWidth)/2;
   const uint maxIterations = 50;
   uint i=0;

   // calculate channel width by bisection
   while (delta > 0.0 && i < maxIterations ){
      double lambda = (D * V0) / (Vc * w * w);
      double twoLambda = 2 * lambda;
      double rCalc = 6*lambda*(coth(1 / twoLambda) - twoLambda);
      double rDiff = rCalc - rMeas;
      this->rmsDiff = rDiff * rDiff;
      if ( rDiff > 0 )
      {  // rCalc too big => omega has to be increased
         w += delta;
         delta /= 2;
      }
      else if ( rDiff < 0)
      {  // rCalc too small => omega has to be decreased
         w -= delta;
         delta /= 2;
      }      
      ++i;
   }

   FFFLog::logText(std::string("Calibration calculated a channel height w of ").append(to_string(w * 10000)).append(" µm.") );
   FFFLog::logText(std::string("Geometrically calculated Volume is ").append(to_string(V0)).append(" ml.") );

   calcGeometVolume(d.length1, d.length2, d.length3, d.chLength, d.b0, d.bL, z_perc * d.chLength);

   FFFLog::logText(std::string("Hydrodynamically calculated Volume is ").append(to_string(Vg)).append(" ml.") );
   return true;
}

void FFFCalibrator::calcGeometVolume(const double L1, const double L2, const double L3, const double L, const double b0, const double bL, const double zL)
{

}


#undef IS_ZERO

bool FFFCalibrator::isZero(double x, int pos) const
{
   if ((x < 0.0) || (x > 0.0))
      return false;
   else {
      std::string message = std::string(
               "Division by zero; Premature Abortion of Calibration forced.\n");
      message.append("Reached chWidth = ").append(to_string((double)w)).
            append(", RMS = ").append(to_string(rmsDiff)).
            append(", delta = ").append(to_string((double)delta)).append(" , Position:").
            append(to_string(pos));
      FFFLog::logError(message);
      return true;
   }
}
