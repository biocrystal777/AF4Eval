#include "af4calibrator.h"
using std::to_string;

CalibErrorCode AF4Calibrator::checkParameters()
{
   if(isZero(params.voidPeakTime)) return CalibErrorCode::voidTimeZero;
   if(isZero(params.elutionFlow))  return CalibErrorCode::eluFlowZero;
   if(isZero(params.elutionTime))  return CalibErrorCode::eluTimeZero;
   if(isZero(params.crossFlow))    return CalibErrorCode::crossFlowZero;
   if(isZero(params.diffCoeff))    return CalibErrorCode::diffCoeffZero;
   //if(params.leftOffsetTime >= params.voidPeakTime) return CalibErrorCode::voidTimeTooSmall;
   if(params.voidPeakTime   >= params.elutionTime)  return CalibErrorCode::eluTimeTooSmall;
   // if no error found,  "unlock" calibration methods
   paramsChecked = true;
   return CalibErrorCode::noError;
}

CalibResult AF4Calibrator::calibrate_classic()
{
   CalibResult result { .width = 0.0, .volume = 0.0 , .errorCode = CalibErrorCode::ParamsNotChecked , .sqDelta = 0.0 };
   if(!paramsChecked) return result;

   // adjust time axis according to leftOffsetTime
   const double tvoid  = params.voidPeakTime;        // params.leftOffsetTime;  // remove focussing time offset
   const double te     = params.elutionTime;         // - params.leftOffsetTime;   // remove focussing time offset
   const double D      = params.diffCoeff * 60.0;    // cm^2/s => cm^2/min
   const double Ve     = params.elutionFlow;
   const double Vc     = params.crossFlow;
   const double z_perc = params.relFocusPoint / 100.0;                // percentage to ratio

   // (1) Calculate Volume:
   double V0 =0.0;
   {
   double flowRatio = (Ve + Vc)/ Vc;
   double hydVolumeDivisor = log((z_perc - flowRatio) / (1 - flowRatio));
   V0 = (Vc * tvoid) / hydVolumeDivisor;
   }

   // (2) Calculate RMeas:
   double rMeas = tvoid / te;

   // (3) Initialize w and δ
   double rmsDiff = 1.0;
   double minWidth = 0.00001;
   double maxWidth = 10;
   double w = (minWidth + maxWidth)/ 2;
   double delta = (maxWidth - minWidth)/2;
   const uint maxIterations = 100;
   uint i=0;

   // (4) calculate channel width w by bisection that |RMeas - Rcalc| =! min
   while (delta > 0.0 && i < maxIterations ){
      double lambda = (D * V0) / (Vc * w * w);
      double twoLambda = 2 * lambda;
      double rCalc = 6*lambda*(coth(1 / twoLambda) - twoLambda);
      double rDiff = rCalc - rMeas;
      rmsDiff = rDiff * rDiff;
      if ( rDiff > 0 )
      {  // rCalc too big => omega has to be increased
         w += delta;
         delta /= 2;
      }
      else if ( rDiff < 0 )
      {  // rCalc too small => omega has to be decreased
         w -= delta;
         delta /= 2;
      }
      else break;
      ++i;
   }
   // -> package results
   result = CalibResult{ w, V0, CalibErrorCode::noError, rmsDiff};
   return result;
}

CalibResult AF4Calibrator::calibrate_geometric()
{
   CalibResult result { .width = 0.0, .volume = 0.0 , .errorCode = CalibErrorCode::ParamsNotChecked , .sqDelta = 0.0 };
   if(!paramsChecked) return result;

   // unpack all parameters and adjust units
   const double tvoid  = params.voidPeakTime;          // min
   const double te     = params.elutionTime;           // min
   const double D      = params.diffCoeff * 60.0;      // cm^2/s => cm^2/min
   //const double Ve   = params.elutionFlow;         // <= not used for this calculation!
   const double Vc     = params.crossFlow;             // ml/min
   const double z_perc = params.relFocusPoint / 100.0; // percentage to ratio

   const double L1 = chDims.length1; // cm
   const double L2 = chDims.length2; // cm
   const double L3 = chDims.length3; // cm
   const double b0 = chDims.b0;      // cm
   const double bL = chDims.bL;      // cm

   // (1) Calculate Rmeas:
   const double rMeas = tvoid / te;
   // (2&3) Initialize λ and δ_λ, find λ such that |RMeas - Rcalc| =! min by bisection:
   double rDiff;
   const double lambda = RToLambda(rMeas, &rDiff);
   // (4) calculate substitution term S
   const double S = lambda * Vc / D;  // (cm³/min)/(cm²/min) = cm
   // (5) calculate passed channel area A_z:
   double Az {0.0};
   const double A3  = 0.5 * bL *L3;
   const double L12 = L1 + L2;
   const double L   = L12 + L3;
   const double z0  = z_perc * L; // cm
   if(z0 >= L1){ // focus position within the "long" channel section L2 ("distal focussing")
      const double bDelta = (b0 - bL);
      const double m2     = (-bDelta) / (2.0 * L2);
      const double t2     = 0.5 * (b0 + (L1 / L2) * bDelta );
      Az = (L12 - z0) * ( m2 * (L12 + z0) + 2.0*t2 ) + A3;
   }
   else {        // focus position within the "short" channel section L1 ("proximal focussing")
      const double m1 = b0 / (2.0 * L1);
      Az = m1 * (squared(L1) - squared(z0)) + 0.5 * (b0 + bL) * L2 + A3;
   }
   // (6) calculate w
   const double w = Az / S;
   // (7) calculate V^geo
   const double Vgeo = Az * w;


   qDebug() << "tvoid"  << tvoid;
   qDebug() << "te"     << te;
   qDebug() << "rMeas"  << rMeas;
   qDebug() << "lambda" << lambda;
   qDebug() << "z0"     << z0;
   qDebug() << "L1"     << L1;
   qDebug() << "L2"     << L2;
   qDebug() << "L3"     << L3;
   qDebug() << "b0"     << b0;
   qDebug() << "bL"     << bL;
   qDebug() << "D"      << D;
   qDebug() << "S"      << S;
   qDebug() << "A3"     << A3;
   qDebug() << "Az"     << Az;
   qDebug() << "w"      << w;

   // package results
   result = { .width = w, .volume = Vgeo, .errorCode = CalibErrorCode::noError , .sqDelta = rDiff };
   return result;
}

CalibResult AF4Calibrator::calibrate_hydrodynamic()
{
   CalibResult result;

   return result;
}

//void AF4Calibrator::calcGeometVolume(const double L1, const double L2, const double L3, const double L, const double b0, const double bL, const double zL)
//{

//}

/*
bool AF4Calibrator::zeroErrorMessage(int pos) const
{
   std::string message = std::string(
            "Division by zero; Premature Abortion of Calibration forced.\n");
   message.append("Reached chWidth = ").append(to_string((double)w)).
         append(", RMS = ").append(to_string(rmsDiff)).
         append(", delta = ").append(to_string((double)delta)).append(" , Position:").
         append(to_string(pos));
   AF4Log::logError(message);
   return true;
}
*/
