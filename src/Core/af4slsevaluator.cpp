#include "af4slsevaluator.h"

using std::vector;
using std::string;

int AF4SLSEvaluator::evalRiMALLS_partZimmplot(const vecD &riData,
                                              matD &rayleighRatios,
                                              const vector<bool> &useDataPoint,
                                              const vecD &dnDcs,
                                              const double laserWl,
                                              const double riSolvent,
                                              const double calibRi,
                                              const double concCap,
                                              const double calibMalls,
                                              const vecD &calibMallsAngles,
                                              const vector<bool> &mallsDetectorRelevant,
                                              vecD *secondVirCoeffs,
                                              vecD *thirdVirCoeffs)
{
   molWeightsCalculated = false;
   radGyrsCalculated = false;

   /////////////////////////////
   // Calculate concntrations //
   /////////////////////////////



   //concentrations;
   FFFLog::logText(string("Calculate concentrations from RI."));
   if(calcConcFromRi(riData, useDataPoint, dnDcs, calibRi, concentrations)){
      FFFLog::logError(string("Error while calculating concentrations"));
      //if(concentrations) delete concentrations;
      return 20;
   }
   concetrationsCalculated = true;


   ///////////////////////////////////////////////////////////////////
   // Calculate x-Values for regression analysis (sin)^2(theta / 2) //
   ///////////////////////////////////////////////////////////////////

   int errorCode(0);

   matD xValues = calcXForZimmplot(concentrations,
                                   mallsDetectorRelevant,
                                   useDataPoint,
                                   secondVirCoeffs,
                                   thirdVirCoeffs,
                                   &errorCode
                                   );

   ////////////////////////////////////////////////////////////////////
   // Calculate y-Values for regression analysis K * c / R(theta, c) //
   ////////////////////////////////////////////////////////////////////

   matD yValues = calcYForZimmplot(concentrations,
                                   rayleighRatios,
                                   mallsDetectorRelevant,
                                   useDataPoint,
                                   dnDcs,
                                   laserWl,
                                   riSolvent,
                                   concCap,
                                   &errorCode
                                   );

   /////////////////////////////////////////////////////////////////////////
   // calculation of molWeight and radius of gyration via lin. regression //
   /////////////////////////////////////////////////////////////////////////

   molWeights = vecD(xValues.size());
   radGyrs = vecD(xValues.size());

   double slope(0.0);
   double offset(0.0);
   for(uint j = 0; j < xValues.size(); ++j){
      if(useDataPoint.at(j)){
         linearRegression(xValues.at(j), yValues.at(j), mallsDetectorRelevant, &slope, &offset);
      }
      if(concentrations.at(j) < concCap) {
         molWeights[j] = 0.0;
         radGyrs[j] = 0.0;
      } else {
         molWeights[j] = 1 / offset;
         radGyrs[j] = std::sqrt(3 * slope * molWeights.at(j));
         radGyrs[j] *= laserWl;
         radGyrs[j] /= 4 * PI * riSolvent;
         radGyrs[j] *= 1e7; // from cm to nm
      }
   }
   setNegativeToZero(radGyrs);
   setNegativeToZero(molWeights);
   molWeightsCalculated = true;
   radGyrsCalculated = true;

   return 0;
}

int AF4SLSEvaluator::evalUVVis_MALLS_partZimmplot(const vecD &uvVisData,
                                                  matD &rayleighRatios,
                                                  const vector<bool> &useDataPoint,
                                                  const vecD &epsilon,
                                                  const vecD &dnDcs,
                                                  const double laserWl,
                                                  const double riSolvent,
                                                  const double calibRi,
                                                  const double concCap,
                                                  const double calibMalls,
                                                  const vecD &calibMallsAngles,
                                                  const vector<bool> &mallsDetectorRelevant,
                                                  vecD *secondVirCoeffs,
                                                  vecD *thirdVirCoeffs)
{
   molWeightsCalculated = false;
   radGyrsCalculated = false;

   /////////////////////////////
   // Calculate concntrations //
   /////////////////////////////

   //vecD concentrations;
   FFFLog::logText(string("Calculate concentrations from absorbance."));
   if(calcConcFromUV_Vis(uvVisData, useDataPoint, epsilon, calibRi, concentrations)){
      FFFLog::logError(string("Error while calculating concentrations"));
      //if(concentrations) delete concentrations;
      return 20;
   }
   concetrationsCalculated = true;


   ///////////////////////////////////////////////////////////////////
   // Calculate x-Values for regression analysis (sin)^2(theta / 2) //
   ///////////////////////////////////////////////////////////////////

   int errorCode(0);

   matD xValues = calcXForZimmplot(concentrations,
                                   mallsDetectorRelevant,
                                   useDataPoint,
                                   secondVirCoeffs,
                                   thirdVirCoeffs,
                                   &errorCode
                                   );

   ////////////////////////////////////////////////////////////////////
   // Calculate y-Values for regression analysis K * c / R(theta, c) //
   ////////////////////////////////////////////////////////////////////

   matD yValues = calcYForZimmplot(concentrations,
                                   rayleighRatios,
                                   mallsDetectorRelevant,
                                   useDataPoint,
                                   dnDcs,
                                   laserWl,
                                   riSolvent,
                                   concCap,
                                   &errorCode
                                   );

   /////////////////////////////////////////////////////////////////////////
   // calculation of molWeight and radius of gyration via lin. regression //
   /////////////////////////////////////////////////////////////////////////

   molWeights = vecD(xValues.size());
   radGyrs = vecD(xValues.size());

   double slope(0.0);
   double offset(0.0);
   for(uint j = 0; j < xValues.size(); ++j){
      if(useDataPoint.at(j)){
         linearRegression(xValues.at(j), yValues.at(j), mallsDetectorRelevant, &slope, &offset);
      }
      if(concentrations.at(j) < concCap) {
         molWeights[j] = 0.0;
         radGyrs[j] = 0.0;
      } else {
         molWeights[j] = 1 / offset;
         radGyrs[j] = std::sqrt(3 * slope * molWeights.at(j));
         radGyrs[j] *= laserWl;
         radGyrs[j] /= 4 * PI * riSolvent;
         radGyrs[j] *= 1e7; // from cm to nm
      }
   }
   setNegativeToZero(radGyrs);
   setNegativeToZero(molWeights);
   molWeightsCalculated = true;
   radGyrsCalculated = true;

   return 0;
}

matD AF4SLSEvaluator::calcXForZimmplot(const vecD &concentrations,
                                       const std::vector<bool> &mallsDetectorRelevant,
                                       const std::vector<bool> &useDataPoint,
                                       vecD *secondVirCoeffs,
                                       vecD *thirdVirCoeffs,
                                       int *errorCode)
{
   matD xValues(concentrations.size()) ;
   for(uint j = 0; j < xValues.size(); ++j)
      xValues[j] = vecD(angles.size());
   for(uint k = 0; k < NUMBER_OF_DETECTORS; ++k){
      if(mallsDetectorRelevant.at(k)){
         double xValue = std::sin(angles.at(k));
         xValue *= xValue;
         for(uint j = 0; j < xValues.size(); ++j){
            if(useDataPoint.at(j)) xValues.at(j)[k] = xValue;
            else                    xValues.at(j)[k] = -1.0;
         }
      }
   }

   // add correction term for second virial coefficient if it was passed

   if(secondVirCoeffs){
      for(uint j = 0; j < xValues.size(); ++j){
         if(useDataPoint.at(j)){
            double tmpValue = 2 * secondVirCoeffs->at(j) * concentrations.at(j);
            for(uint k = 0; k < NUMBER_OF_DETECTORS; ++k){
               if(mallsDetectorRelevant.at(k))
                  xValues.at(j)[k] += tmpValue;
            }
         }
      }
   }

   // add correction term for third virial coefficient if it was passed

   if(thirdVirCoeffs){
      for(uint j = 0; j < xValues.size(); ++j){
         if(useDataPoint.at(j)){
            double tmpValue = 3 * thirdVirCoeffs->at(j) * concentrations.at(j) * concentrations.at(j);
            for(uint k = 0; k < angles.size(); ++k){
               if(mallsDetectorRelevant.at(k))
                  xValues.at(j)[k] += tmpValue;
            }
         }
      }
   }
   return xValues;
}

matD AF4SLSEvaluator::calcYForZimmplot(
      const vecD &concentrations,
      const matD &rayleighRatios,
      const vector<bool> &mallsDetectorRelevant,
      const vector<bool> &useDataPoint,
      const vecD &dnDcs,
      const double laserWl,
      const double riSolvent,
      const double concCap,
      int *errorCode)
{
   matD yValues(concentrations.size());
   for(uint j = 0; j < yValues.size(); ++j){
      yValues[j] = vecD(angles.size());

      if(concentrations.at(j) < concCap) continue;

      if(useDataPoint.at(j)){
         double slsK = 4 * PI * PI;
         slsK *= riSolvent * riSolvent;
         slsK *= dnDcs.at(j) * dnDcs.at(j);
         slsK /= std::pow(laserWl, 4) * NA;
         double slsConst = concentrations.at(j) * slsK;
         for(uint k = 0; k < angles.size(); ++k){
            if(mallsDetectorRelevant.at(k)){
               yValues.at(j)[k] = slsConst / (rayleighRatios.at(k).at(j));
            }
         }
      }
      else {
         for(uint k = 0; k < angles.size(); ++k){
            if(mallsDetectorRelevant.at(k))
               yValues.at(j)[k] = -1.0;
         }
      }
   }
   return yValues;
}

int AF4SLSEvaluator::calcRho(const vecD& stokesRadii,
                             const vecD& gyrRadii,
                             vecD& rhos)
{
   if(stokesRadii.size() != gyrRadii.size()){
      return 1;
   }
   rhos = vecD(stokesRadii.size());
   for(uint j = 0; j < rhos.size(); ++j){
      rhos[j] = gyrRadii.at(j) / stokesRadii.at(j);
   }
   return 0;
}

int AF4SLSEvaluator::calcConcFromRi(const vecD &riData,
                                    const vector<bool> &useDataPoint,
                                    const vecD &dnDcs,
                                    double calibRiConstant,
                                    vecD &resConcentrations
                                    )
{
   resConcentrations = vecD(riData.size());
   for(uint j = 0; j < resConcentrations.size(); j++){
      if(useDataPoint[j])
         //resConcentrations[j] = 1000 * (riData.at(j) * calibRiConstant) / dnDcs.at(j);
         // resConcentrations[j] = 1000.0 * (riData.at(j) / dnDcs.at(j));
         resConcentrations[j] =  (riData.at(j) / calibRiConstant / dnDcs.at(j));
      else
         resConcentrations[j] = -1.0;      
   }

   return 0;
}

int AF4SLSEvaluator::calcConcFromUV_Vis(const vecD &absorbanceData,
                                        const std::vector<bool> &useDataPoint,
                                        const vecD &epsilon,
                                        const double optPathlength,
                                        vecD &resConcentrations,
                                        const double unitFactor)
{   
   resConcentrations = vecD(absorbanceData.size());
   for(uint j = 0; j < resConcentrations.size(); j++){
      if(useDataPoint[j])
         resConcentrations[j] = absorbanceData[j] * epsilon[j] * optPathlength * unitFactor;
      else
         resConcentrations[j] = -1.0;
   }
   return 0;
}

int AF4SLSEvaluator::calcTotalRefIndices(const vecD &riData,
                                         const vector<bool> &useDataPoint,
                                         double calibRiConstant,
                                         double riSolvent,
                                         vecD &resRefIndices
                                         )
{
   resRefIndices = vecD(riData.size());

   for(uint j = 0; j < resRefIndices.size(); ++j){
      if(useDataPoint.at(j))
         resRefIndices[j] = riData.at(j) * calibRiConstant + riSolvent;
      else
         resRefIndices[j] = -1.0;
   }
   return 0;
}

int AF4SLSEvaluator::linearRegression(const vecD &xValues,
                                      const vecD &yValues,
                                      const vector<bool> &pointRelevant,
                                      double *slope,
                                      double *offset) const
{
   if((xValues.size() != yValues.size())
         || (xValues.size() != yValues.size())
         || (xValues.size() != pointRelevant.size())
         ){
      return 1;
   }

   double sumX(0.0);
   double sumY(0.0);
   double sumXY(0.0);
   double sumXSq(0.0);
   uint nAll = 0;
   for(uint i = 0; i < xValues.size(); ++i){
      if(pointRelevant.at(i)){
         sumX += xValues.at(i);
         sumY += yValues.at(i);
         sumXY += xValues.at(i) * yValues.at(i);
         sumXSq += xValues.at(i) * xValues.at(i);
         ++nAll;
      }
   }

   *slope = (nAll * sumXY - sumX * sumY) / (nAll * sumXSq - sumX * sumX);
   *offset = (sumY - ((*slope) * sumX)) / nAll;

   return 0;
}

int AF4SLSEvaluator::setNegativeToZero(vecD &values)
{
   for (uint j = 0; j < values.size(); ++j)
      if( values.at(j) < 0.0)
         values.at(j) = 0.0;
   return 0;
}

vecD AF4SLSEvaluator::getMolWeights(bool *exists) const
{
   if(exists) *exists = molWeightsCalculated;
   return molWeights;
}

vecD AF4SLSEvaluator::getRadGyrs(bool *exists) const
{
   if(exists) *exists = radGyrsCalculated;
   return radGyrs;
}

vecD AF4SLSEvaluator::getConcentrations(bool *exists) const
{
   if(exists) *exists = concetrationsCalculated;
   return concentrations;
}
