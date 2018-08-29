#include "fffdiffcorrector.h"
#include "./fffparameterstructs.h"
#include "../Gui/Deconvolution/fffdiffcorrectionwidget.h"
#include <iostream>

// add warning message later if(displayZeroMessages) ;

using std::string;
using std::vector;


#if QWT_VERSION == 0x060102

int FFFDiffCorrector::diffCorrection(vecD &times,
                                     const matD &data,
                                 //    uint algorithm,
                                 //    uint parallelMode,
                                 //    uint baseLineCorrMode,
                                     FFFDiffCorrParameters p,
                                     ChannelDims dims,
                                     ChannelDimsFromCalib dimsFromCalib,
                                     bool keepIntegralSize,
                                     vecD& diffCoeffs,
                                     uint *newStartIndex,
                                     uint *newEndIndex,
                                     bool displayZeroMessages,
                                     const bool writeCSV,
                                     const bool writeGnuplot,
                                     const DeconWriteFreq writeFreq,
                                     uint normMode,
                                     QVector<double>* plotTData,
                                     QVecMatrix<double>* plotRefData,
                                     QVecMatrix<double>* plotCorrData,
                                     omp_QVector<bool> const* plotDataToUpdate,
                                     uint *actIteration,
                                     std::vector<uint>* actIterations_par,
                                     //const bool trackRMS,
                                     const bool checkBackConv,
                                     vector<vector<lDouble> > *backConvDiff
                                     //  bool makeSnapshots,
                                     //  QList<uint>* snapShotList,
                                     //  vector<vectorMatrix*>** snapShotResults
                                     )
#endif

{
   int errorCode(0);

   const double leftOffsetTime = p.leftOffsettime;
   const double elutionFlow = p.elutionFlow;
   const double crossFlow = p.crossFlow;
   const double r0 = p.r0;
   double startTime = p.startTime;
   double endTime = p.endTime;
   const double relFocusPoint = p.relFocusPoint;
   const uint itMin = p.itMin;
   const uint itMax = p.itMax;
   const double maxRMS = p.maxRMS;
   const double leftAutoBaseBorder = p.leftAutoBaseBorder; // in %
   const double rightAutoBaseBorder = p.rightAutoBaseBorder; // in %
   double voidPeakTime = p.voidPeakTime;

   const double chWidth = dimsFromCalib.chWidth;
   const double chLength = dims.chLength;
   double chVolume = dimsFromCalib.hydrodynVolume;
   const double b0 = dims.b0;
   const double bL = dims.bL;
   const uint algorithm = p.algorithm;
   const uint parallelMode = p.parallelMode;
   const uint baseLineCorrMode = p.baseLineCorrMode;


   if(p.recalcVolume)
   {

      if(leftOffsetTime >= voidPeakTime) return 20;
      if(voidPeakTime >= startTime)      return 21;

      voidPeakTime -= leftOffsetTime;
      double var1 = (elutionFlow + crossFlow)/ crossFlow;
      double volumeFac = log((relFocusPoint / 100 - var1) / (1 - var1));
      chVolume = (crossFlow * voidPeakTime) / volumeFac;
   }
   else
   {
      double var1 = (elutionFlow + crossFlow)/ crossFlow;
      double volumeFac = log((relFocusPoint / 100 - var1) / (1 - var1));
      voidPeakTime = chVolume * volumeFac / crossFlow;


      if(leftOffsetTime >= voidPeakTime) return 20;
      if(voidPeakTime >= startTime)      return 21;
   }


   ///////////////////////////
   // check time parameters //
   ///////////////////////////


   if(startTime >= endTime)           return 22;



   logTextToMainThread(tr("Ve: %1").arg(elutionFlow));



   logTextToMainThread(tr("Vc: %1").arg(crossFlow));
   logTextToMainThread(tr("ω: %1").arg(chWidth));
   logTextToMainThread(tr("L: %1").arg(chLength));
   logTextToMainThread(tr("V0: %1").arg(chVolume));
   logTextToMainThread(tr("b0: %1").arg(b0));
   logTextToMainThread(tr("bL: %1").arg(bL));
   logTextToMainThread(tr("r0: %1").arg(r0));
   logTextToMainThread(tr("start: %1").arg(startTime));
   logTextToMainThread(tr("end: %1").arg(endTime));
   logTextToMainThread(tr("z%: %1").arg(relFocusPoint));
   logTextToMainThread(tr("itMin: %1").arg(itMin));
   logTextToMainThread(tr("itMax: %1").arg(itMax));
   logTextToMainThread(tr("maxRMS: %1").arg(maxRMS));
   logTextToMainThread(tr("leftCorr: %1").arg(leftAutoBaseBorder));
   logTextToMainThread(tr("rightCorr: %1").arg(rightAutoBaseBorder));
   logTextToMainThread(tr("voidPeak: %1").arg(voidPeakTime));







   if(actIteration) *actIteration = 0;
   if(!(algorithm == 1 || algorithm == 2)) return 50;

   if(backConvDiff) backConvDiff->resize(data.size());
   for(vector<lDouble> &b : *backConvDiff) b.resize(itMax);


   //////////////////////////////////////////////////
   // adjust time axis according to leftOffsetTime //
   //////////////////////////////////////////////////

   for(uint j = 0; j < times.size(); j++)
      times.at(j) -= leftOffsetTime;

   startTime -= leftOffsetTime;
   endTime -= leftOffsetTime;

   ////////////////////////////
   // Tests input parameters //
   ////////////////////////////

   uint start = findIndexFromTime(times, startTime);
   uint end = findIndexFromTime(times, endTime);


   if((end > times.size()) || (start > times.size()) || (start > end)) return 35;

   if(newStartIndex) *newStartIndex = start;

   if(newEndIndex) *newEndIndex = end;
   if(voidPeakTime <= 0.0) return 1;
   if(elutionFlow  <= 0.0) return 2;
   if(elutionFlow  <= 0.0) return 3;
   if(chWidth      <= 0.0) return 4;
   if(chLength     <= 0.0) return 5;
   if(maxRMS       <= 0.0) return 6;

   ////////////////////////////////////////////////
   // create working copy for relevant data from //
   // data matrix, diffusion Coefficients etc.   //
   ////////////////////////////////////////////////

   uint wDataLength = end - start;

   workData.resize(data.size());
   for(uint i = 0; i < data.size(); i++){
      workData[i] =  vecD(wDataLength);
      for(uint j=0; j < wDataLength; j++){
         workData[i][j] = data.at(i).at(j + start);
      }
   }

   workTimes.resize(wDataLength);
   for(uint j=0; j < wDataLength; j++){
      workTimes[j] = times.at(j + start);
   }

   workDCoeffs = std::move(vecD (wDataLength));;
   for(uint j=0; j < wDataLength; j++){
      workDCoeffs[j] = diffCoeffs.at(j + start);
   }

   ////////////////////////////////
   // Do baseline autocorrection //
   ////////////////////////////////

   if(baseLineCorrMode == 2){
      if(parallelMode == 1) linearBaseLineCorrection_par(workData, leftAutoBaseBorder, rightAutoBaseBorder);
      else if(parallelMode == 0) linearBaseLineCorrection(workData, leftAutoBaseBorder, rightAutoBaseBorder);
   }
   else if(baseLineCorrMode == 1){
      if(parallelMode == 1) linearBaseLineCorrection_par(workData, leftAutoBaseBorder, rightAutoBaseBorder);
      else if(parallelMode == 0) linearBaseLineCorrection(workData, leftAutoBaseBorder, rightAutoBaseBorder);
   }

   /*
      uint leftBorderIndex  = uint (wDataLength * leftAutoBaseBorder  / 100);
      uint rightBorderIndex = uint (wDataLength * (1 - (rightAutoBaseBorder / 100)));
#pragma omp parallel// private(thread_id)
      {
#pragma omp for
         for(uint i = 0; i < workData.size(); ++i){
            // base line angle points
            double X1 = 0.0;
            double Y1 = std::numeric_limits<double>::max();
            for(uint j = 0; j <= leftBorderIndex; ++j){
               double test = workData[i][j];
               if(test < Y1){
                  Y1 = test;
                  X1 = workTimes[j];
               }
            }
            double X2 = 0.0;
            double Y2 = std::numeric_limits<double>::max();
            for(uint j = rightBorderIndex; j < wDataLength; ++j){
               double test = workData[i][j];
               if(test < Y2){
                  Y2 = test;
                  X2 = workTimes[j];
               }
            }
            // base line parameters
            double slope = (Y2 - Y1) / (X2 - X1);
            double offset = Y2 - (slope * X2);
            // subtract base line
            for(uint j = 0; j < wDataLength; ++j)
               workData[i][j] -= slope * workTimes[j] + offset;
         }
      }
      */


   //////////////////////
   // Tests input data //
   //////////////////////

   for(uint j = 0; j < wDataLength; j++) if(workDCoeffs[j] <= 0.0) return 7;
   offsetCorrection(workData);

   //////////////////////////////
   // calculate rMeas +        //
   // adapt working data sizes //
   //////////////////////////////

   start = 0;
   if(!calcRMeas(voidPeakTime, &start)){
      return 31;
   }

   start = 0;
   if(!calcLambda(//voidPeakTime, elutionFlow,
                  crossFlow,
                  chWidth,
                  chVolume,
                  //relFocusPoint, chLength,
                  &start, &errorCode)) return 32;
   //uint jOld;
   uint newSize = lambda.size();
   if(lambda.size() == 0) return 66;

   // adapt length of parameter vectors calculated before
   if(start != 0){
      workTimes.erase(workTimes.begin(), workTimes.begin() + start);
      workDCoeffs.erase(workDCoeffs.begin(), workDCoeffs.begin() + start);
      for(vecD &w : workData) w.erase(w.begin(), w.begin() + start);
      *newStartIndex += start;
   }

   //////////////////////////////
   // calculate chi +          //
   // adapt working data sizes //
   //////////////////////////////
   //qDebug() << 10;
   start = 0;
   if(!calcChi(&start, &errorCode)){
      return errorCode;
   }
   newSize = chi.size();
   if(start != 0){ // adapt length of parameter vectors calculated before
      rMeas.erase(rMeas.begin(), rMeas.begin() + start);
      lambda.erase(lambda.begin(), lambda.begin() + start);
      workTimes.erase(workTimes.begin(), workTimes.begin() + start);
      workDCoeffs.erase(workDCoeffs.begin(), workDCoeffs.begin() + start);
      for(vecD &w : workData) w.erase(w.begin(), w.begin() + start);
      *newStartIndex += start;
   }
   //////////////////////////////
   // calculate sigmaSq +      //
   // adapt working data sizes //
   //////////////////////////////
   start = 0;
   if(!calcSigmaSq(//voidPeakTime,
                   elutionFlow, crossFlow, chWidth, chVolume, chLength, b0, bL, relFocusPoint, &start, &errorCode)){

      return errorCode;
   }

   //double factor = 15.0;
   //for(double &s : sigmaSq) s *= factor;




   newSize = sigmaSq.size();
   if(start != 0){ // adapt length of parameter vectors calculated before
      rMeas.erase(rMeas.begin(), rMeas.begin() + start);
      lambda.erase(lambda.begin(), lambda.begin() + start);
      chi.erase(chi.begin(), chi.begin() + start);
      workTimes.erase(workTimes.begin(), workTimes.begin() + start);
      workDCoeffs.erase(workDCoeffs.begin(), workDCoeffs.begin() + start);
      for(vecD &w : workData) w.erase(w.begin(), w.begin() + start);
      *newStartIndex += start;
   }

   finalLength = newSize;

   // create snapshot containers
   /*
   if(makeSnapshots){
      *snapShotResults = new vector<vectorMatrix*>(snapShotList->size());
      for(int h = 0; h < snapShotList->size(); ++h){
         (**snapShotResults)[h] = new vectorMatrix(data.size());
         for(uint i = 0; i < data.size(); ++i)
            (*((*snapShotResults)->at(h)))[i] = new vecD(finalLength);
      }
   }*/

   qDebug() << workTimes.size() << workDCoeffs.size() << sigmaSq.size() << workData[3].size();

   for(uint i = 0; i < workTimes.size(); ++i){
      qDebug() << workTimes[i] << ","
               << workDCoeffs[i] << ","
               << sigmaSq[i] << ","
               << workData[3][i];
   }

#if QWT_VERSION == 0x060102
   // set initial plot values



   /*
    *
    * DEBUG MOVE CONSTRUCTOR OF omp_QVecMatrix LATER!!!
    *
    */


   *plotTData = QVector<double>::fromStdVector(workTimes);

   plotRefData->resize(workData.size());
   for(uint i = 0; i < workData.size(); ++i){
      (*plotRefData)[i] = QVector<double>::fromStdVector(workData[i]);
      ///for(uint j = 0; j < workData.at(i).size(); ++j)

      //(*plotRefData)[i][j] = workData[i][j];
   }

   plotCorrData->resize(workData.size());
   for(uint i = 0; i < workData.size(); ++i){
      for(uint j = 0; j < workData.at(i).size(); ++j){
         (*plotCorrData)[i] = QVector<double>::fromStdVector(workData[i]);
      }
   }
   // omp_QVecMatrix<double>::omp_fromStdMat(workData);
   /*
   omp_QVecMatrix<double> refData(omp_QVecMatrix<double>::omp_fromStdMat(workData));
   *plotRefData = refData; //(omp_QVecMatrix<double>::omp_fromStdMat(workData));
   omp_QVecMatrix<double> corrData(omp_QVecMatrix<double>::omp_fromStdMat(workData));
   *plotCorrData = corrData;
   */
   this->signalsToUpdate = *plotDataToUpdate;
   emit initPlot_C();
#endif


   ////////////////////////////////////
   // actual deconvolution algorithm //
   ////////////////////////////////////

   if(algorithm == 1){

#if QWT_VERSION == 0x060102
      errorCode = iterativeRelaxation(r0,
                                      itMin,
                                      itMax,
                                      maxRMS,
                                      keepIntegralSize,
                                      normMode,
                                      actIteration
                                      );
   }
   else if(algorithm == 2) {
      if(parallelMode == 0)
         errorCode = richardsonLucy(itMax,
                                    actIteration,
                                    checkBackConv,
                                    backConvDiff
                                    // vector<vector<lDouble> > *trackRMS
                                    // plotT,
                                    // plotY,
                                    // plotColor,
                                    // thread,
                                    //  makeSnapshots,
                                    //  snapShotList,
                                    //  *snapShotResults
                                    );
      else if(parallelMode == 1){
         if( (writeCSV || writeGnuplot) && (writeFreq != DeconWriteFreq::last) )
            errorCode = richardsonLucy_parLogPlot(itMax,
                                                  actIterations_par,
                                                  checkBackConv,
                                                  backConvDiff,
                                                  plotCorrData,
                                                  writeCSV,
                                                  writeGnuplot,
                                                  writeFreq
                                                  );
         else
            errorCode = richardsonLucy_par(itMax,
                                           actIterations_par,
                                           checkBackConv,
                                           backConvDiff,
                                           plotCorrData
                                           );
      }
 #endif
   }
   calculationHasRun = true;
   return errorCode;
}

int FFFDiffCorrector::iterativeRelaxation(double r0,
                                          uint itMin,
                                          uint itMax,
                                          double maxRMS,
                                          bool keepIntegralSize,
                                          uint normMode,
                                          uint* actIteration
                                          )

{
   /////////////////////////////
   // calculate old integrals //
   /////////////////////////////
   vecD oldIntegralValues(workData.size());
   double thisIntegralValue;
   if(keepIntegralSize){
      for(uint i = 0; i < workData.size(); i++){
         thisIntegralValue = 0;
         for (uint j = 0; j < workData.at(i).size(); j++){
            thisIntegralValue += workData.at(i).at(j);
         }
         oldIntegralValues.at(i) = thisIntegralValue;
      }
   }
   ////////////////////////////
   // normalize working data //
   ////////////////////////////

   if(!offsetNormalize(workData, normMode)){
      return 30;
   }

   //////////////////////////////////
   // show normalized working data //
   //////////////////////////////////

#if QWT_VERSION == 0x060102
   emit updatePlot();
#endif //QWT_VERSION
   //////////////////////////////////////////////
   // allocate memory for GWFolded and results //
   //////////////////////////////////////////////

   GWFolded = matD();
   for(uint i = 0; i < workData.size(); i++){
      GWFolded.push_back(vecD(finalLength));
   }

   /////////////////////////////////////
   // allocate memory for dataCorr    //
   // (i.e. the deconvoluted results) //
   /////////////////////////////////////

   dataCorr = matD();
   for (uint i = 0; i < workData.size(); i++){
      dataCorr.push_back(vecD(workData.at(i)));
   }
   /////////////////////////////
   // Iterative deconvolution //
   /////////////////////////////

   newRMS = 0;
   double oldRMS = DBL_MAX;
   double deltaRMS = DBL_MAX;
   double tWij;
   double tFij;
   double sqRMS=0;
   uint snapShotIndex(0);
   for (uint k=0; k < itMax; k++){
      calcGWFolded(normMode);
      for(uint i = 0; i < dataCorr.size(); i++){
         for(uint j = 0; j < finalLength; j++){
            tWij = dataCorr.at(i).at(j);
            tFij = workData[i][j];
            dataCorr[i][j] = r0 * tWij * (1 - tWij) * (tFij - GWFolded[i][j]);
            dataCorr[i][j] +=  tWij;
         }
      }
      for(uint j = 0; j < finalLength; j++){
         sqRMS = workData[0][j] - dataCorr[0][j];
         sqRMS *= sqRMS;
         newRMS += sqRMS;
      }

      // plot

#if QWT_VERSION == 0x060102
      emit updatePlot();
#endif
      // snapshots

      /* if(makeSnapshots && isContainedinList(k, snapShotList)){
         for(uint i = 0; i < dataCorr.size(); ++i){
            for(uint j = 0; j < finalLength; ++j){
               (*(snapShotResults->at(snapShotIndex)->at(i)))[j] = dataCorr[i][j];
            }
         }
         ++snapShotIndex;
      }
      */
      newRMS = sqrt(newRMS/finalLength);
      deltaRMS = fabs(newRMS - oldRMS);
      oldRMS = newRMS;
      newRMS = 0;
      if((deltaRMS < maxRMS) && (k >= itMin)) break;
      if(actIteration) *actIteration = k + 1;
   }
   offsetNormalize(dataCorr, normMode);

   ////////////////////////////////////////////////
   // calculate new integrals and resize results //
   ////////////////////////////////////////////////

   if(keepIntegralSize){
      vecD newIntegralValues(dataCorr.size());
      for(uint i = 0; i < dataCorr.size(); i++){
         thisIntegralValue = 0;
         for (uint j =0; j < dataCorr.at(i).size(); j++)
            thisIntegralValue += dataCorr[i][j];
         newIntegralValues.at(i) = thisIntegralValue;
      }
      double resizingFactor;
      for(uint i = 0; i < dataCorr.size(); i++){
         resizingFactor = oldIntegralValues.at(i) / newIntegralValues.at(i);
         for(uint j = 0; j < dataCorr.at(i).size(); j++)
            dataCorr[i][j] *= resizingFactor;
      }
   }
   return 0;
}

int FFFDiffCorrector::richardsonLucy(const uint itMax,
                                     uint *actIteration,
                                     bool checkBackConv,
                                     vector<vector<lDouble> > *trackBackConv
                                     //matD *plotT,
                                     //matD *plotY,
                                     //vector<QColor> *plotColor,
                                     //void *thread,
                                     //bool makeSnapshots,
                                     //QList<uint> *snapShotList,
                                     //std::vector<vectorMatrix*>* snapShotResults
                                     )
{

   const uint dataLength = workTimes.size();

   correctMatrixToPositive(workData);

   ///////////////////
   // Draw raw data //
   ///////////////////


   emit logTextToMainThread(tr("Calculate Broadening function Matrix."));

   if(workTimes.size() < 2){
      emit logErrorToMainThread(tr("Window too small. Less than 2 measurement points"));
      return 50;
   }

   const double deltaT = std::abs(workTimes.at(dataLength-1) - workTimes[0]) /
         (double) dataLength;

   matD broadMatrix(dataLength);

   for(uint i = 0; i < dataLength; ++i){
      broadMatrix[i] = vecD(dataLength);
      for(uint j = 0; j < dataLength; ++j){
         double matrixEntry = workTimes[i] - workTimes[j];
         matrixEntry *= matrixEntry;
         matrixEntry /= 2 * sigmaSq[j];
         matrixEntry = std::exp(-matrixEntry);
         matrixEntry *= deltaT / (std::sqrt(2 * PI * sigmaSq[j]));
         broadMatrix[i][j] = matrixEntry;
      }
   }
   emit logTextToMainThread(tr("Allocate memory."));

   vecD vecC(dataLength);
   for(uint i = 0; i < dataLength; ++i){
      double vecValue = 0.0;
      for(uint j = 0; j < broadMatrix.size(); ++j)
         vecValue += broadMatrix[j][i];
      vecC[i] = vecValue;
   }

   /////////////////////////////////////
   // allocate memory for dataCorr    //
   // (i.e. the deconvoluted results) //
   // and initialize the values       //
   /////////////////////////////////////
   emit logTextToMainThread(tr("Initialize deconvoluted matrix with starting data."));

   dataCorr.clear();
   for (uint i=0; i < workData.size(); i++){
      dataCorr.push_back(vecD(workData.at(i)));
   }

   /////////////////////
   // Start Iteration //
   /////////////////////

   emit logTextToMainThread(tr("Start iterations."));

   for(uint i = 0; i < dataCorr.size(); ++i){
      vecD vecB(dataLength);
      vecD vecM(dataLength);
      double vecMVal(0.0);
      double vecBVal(0.0);
      // update reference graph image
      for(uint it = 0; it < itMax; ++it){
         *actIteration =it;
         // calculate vecB
         for(uint j = 0; j < dataLength; ++j){
            vecBVal = 0.0;
            for(uint jSum = 0; jSum < dataLength; ++jSum)
               vecBVal += broadMatrix[j][jSum] * dataCorr[i][jSum];
            vecB[j] = vecBVal;
         }
         // calculate vecM
         for(uint j = 0; j < dataLength; ++j){
            vecMVal = 0;
            for(uint jSum = 0; jSum < dataLength; ++jSum)
               vecMVal += broadMatrix[jSum][j] * workData[i][jSum] / vecB[jSum];
            vecM[j] = vecMVal / vecC[j];
         }
         // calculate new W
         for(uint j = 0; j < dataLength; ++j)
            dataCorr[i][j] =  vecM[j] * dataCorr[i][j];
#if QWT_VERSION == 0x060102

         if(signalsToUpdate[i]) updatePlot();
#endif
      }
   }
   return 0;
}



int FFFDiffCorrector::richardsonLucy_par(const uint itMax,
                                         vector<uint> *actIterations,
                                         bool checkBackConv,
                                         vector<vector<lDouble> > *trackBackConv,
                                         QVecMatrix<double> *const plotCorrYData
                                         )
{


   const uint dataLength = workTimes.size();
   const uint noOfSignals = workData.size();

   correctMatrixToPositive(workData);

   emit logTextToMainThread(tr("Calculate Broadening function Matrix."));

   if(workTimes.size() < 2){
      emit logErrorToMainThread(tr("Window too small. Less than 2 measurement points"));
      return 50;
   }

   const double deltaT = std::abs(workTimes.at(dataLength-1) - workTimes[0]) / (double) dataLength;
   matD broadMatrix(dataLength);

   for(uint i = 0; i < dataLength; ++i){
      broadMatrix[i] = vecD(dataLength);
      for(uint j = 0; j < dataLength; ++j){
         double matrixEntry = workTimes[i] - workTimes[j];
         matrixEntry *= matrixEntry;
         matrixEntry /= 2 * sigmaSq[j];
         matrixEntry = std::exp(-matrixEntry);
         matrixEntry *= deltaT / (std::sqrt(2 * PI * sigmaSq[j]));
         broadMatrix[i][j] = matrixEntry;
      }
   }
   emit logTextToMainThread(tr("Allocate memory."));


   vecD vecC(dataLength);
   for(uint i = 0; i < dataLength; ++i){
      double vecValue = 0.0;
      for(uint j = 0; j < broadMatrix.size(); ++j)
         vecValue += broadMatrix[j][i];
      vecC[i] = vecValue;
   }
   /////////////////////////////////////
   // allocate memory for dataCorr    //
   // (i.e. the deconvoluted results) //
   /////////////////////////////////////


   dataCorr.resize(noOfSignals);
   for(vecD &d : dataCorr) d.resize(dataLength);


   /////////////////////
   // Start Iteration //
   /////////////////////

   emit logTextToMainThread(tr("Start iterations."));

   matD vecWOld(noOfSignals);
   for(uint i = 0; i < workData.size(); ++i) vecWOld[i] = vecD(workData.at(i));

#pragma omp parallel// private(thread_id)
   {         //thread_id = 0;
#pragma omp for
      for(uint i = 0; i < noOfSignals; ++i){
         vecD vecB(dataLength);
         vecD vecM(dataLength);

         for(uint it = 0; it < itMax; ++it){

            vecD vecWNew = vecD(dataLength);


            //assign number of iterations for individual threads
            if(i < actIterations->size())
               actIterations->at(i) = it;
            // calculate vecB
            for(uint j = 0; j < dataLength; ++j){
               double vecBVal = 0.0;
               for(uint jSum = 0; jSum < dataLength; ++jSum)
                  vecBVal += broadMatrix[j][jSum] * vecWOld[i][jSum];
               vecB[j] = vecBVal;
            }

            // calculate vecM
            for(uint j = 0; j < dataLength; ++j){
               double vecMVal = 0.0;
               for(uint jSum = 0; jSum < dataLength; ++jSum)
                  vecMVal += broadMatrix[jSum][j] * workData[i][jSum] / vecB[jSum];
               vecM[j] = vecMVal / vecC[j];
            }

            for(uint j = 0; j < dataLength; ++j)
               vecWNew[j] = vecM[j] * vecWOld[i][j];


#if QWT_VERSION == 0x060102

            (*plotCorrYData)[i] = QVector<double>::fromStdVector(vecWNew);//set(i,plotCorrLoc);
            emit updatePlot();

#endif

            if(checkBackConv){

               vecD backConvTest = backConvolutionRL(vecWNew, broadMatrix, deltaT);

               lDouble newRMS = sqDiff(backConvTest, workData[i]);
               if(i == 0){
                  QFile file(tr("/home/bluemage/test/RLTests/it%1.csv").arg(it));
                  file.open(QIODevice::WriteOnly);
                  QDataStream out(&file);
                  out << "times," << "raw data," << "deconv," << "reconv\n";
                  for(uint j = 0; j < dataLength; ++j){
                     out << workTimes[j] << "," << workData[0][j] << "," << vecWNew[j] << "," << backConvTest[j] << "\n";\
                  }
                  file.close();
               }
               trackBackConv->at(i)[it] = newRMS;
               vecWOld[i] = std::move(vecWNew);
            }
            else
               vecWOld[i] = std::move(vecWNew);
         }
         dataCorr[i] = std::move(vecWOld[i]);

         if(i < signalsToUpdate.omp_size() && signalsToUpdate[i]) emit updatePlot();
      }
   }
   return 0;
}


int FFFDiffCorrector::richardsonLucy_parLogPlot(const uint itMax,
                                                vector<uint> *actIterations,
                                                bool checkBackConv,
                                                vector<vector<lDouble> > *trackBackConv,
                                                QVecMatrix<double> *const plotCorrYData,
                                                const bool writeCSV,
                                                const bool useGnuPlot,
                                                const DeconWriteFreq writeFreq
                                                )
{


   for(uint &i : *actIterations) i = 0;

   const uint dataLength = workTimes.size();
   const uint noOfSignals = workData.size();

   correctMatrixToPositive(workData);

   emit logTextToMainThread(tr("Calculate Broadening function Matrix."));

   if(workTimes.size() < 2){
      emit logErrorToMainThread(tr("Window too small. Less than 2 measurement points"));
      return 50;
   }

   matD broadMatrix = calcRichLucyBroadMatrix(workTimes, sigmaSq);

   emit logTextToMainThread(tr("Allocate memory."));

   vecD vecC(dataLength);
   for(uint i = 0; i < dataLength; ++i){
      double vecValue = 0.0;
      for(uint j = 0; j < broadMatrix.size(); ++j)
         vecValue += broadMatrix[j][i];
      vecC[i] = vecValue;
   }
   /////////////////////////////////////
   // allocate memory for dataCorr    //
   // (i.e. the deconvoluted results) //
   /////////////////////////////////////

   dataCorr.resize(noOfSignals);
   for(vecD &d : dataCorr) d.resize(dataLength);

   /////////////////////
   // Start Iteration //
   /////////////////////

   emit logTextToMainThread(tr("Start iterations."));

   matD vecWOld(noOfSignals);



   uint itsPerParPhase = getItsPerParPhase(writeFreq);
   uint itsThisParPhase = itsPerParPhase;
   const uint noOfParPhases = itMax / itsPerParPhase + 1;

   for(uint parPhase = 0; parPhase < noOfParPhases; ++parPhase){
      // initialize vecWOld either with workData or with results from the recent parPhase
      if(parPhase == 0) for(uint i = 0; i < workData.size(); ++i) vecWOld[i] = vecD(workData.at(i));
      else for(uint i = 0; i < workData.size(); ++i) vecWOld[i] = vecD(dataCorr.at(i));
      // less its last parPhase
      if(parPhase == noOfParPhases - 1) parPhase = itMax % itsPerParPhase;
#pragma omp parallel// private(thread_id)
      {         //thread_id = 0;
#pragma omp for
         for(uint i = 0; i < noOfSignals; ++i){
            vecD vecB(dataLength);
            vecD vecM(dataLength);
            vecD vecWNew = vecD(dataLength);
            for(uint it = 0; it < itsThisParPhase; ++it){
               //increase individual iteration number for each thread
               if(i < actIterations->size()) ++(actIterations->at(i));
               // calculate vecB
               for(uint j = 0; j < dataLength; ++j){
                  double vecBVal = 0.0;
                  for(uint jSum = 0; jSum < dataLength; ++jSum)
                     vecBVal += broadMatrix[j][jSum] * vecWOld[i][jSum];
                  vecB[j] = vecBVal;
               }

               // calculate vecM
               for(uint j = 0; j < dataLength; ++j){
                  double vecMVal = 0.0;
                  for(uint jSum = 0; jSum < dataLength; ++jSum)
                     vecMVal += broadMatrix[jSum][j] * workData[i][jSum] / vecB[jSum];
                  vecM[j] = vecMVal / vecC[j];
               }

               for(uint j = 0; j < dataLength; ++j)
                  vecWNew[j] = vecM[j] * vecWOld[i][j];


#if QWT_VERSION == 0x060102

               (*plotCorrYData)[i] = QVector<double>::fromStdVector(vecWNew);//set(i,plotCorrLoc);               
               emit updatePlot();
#endif

            }
            dataCorr[i] = vecWNew;
            if(i < signalsToUpdate.omp_size() && signalsToUpdate[i]) emit updatePlot();
         }   // end omp parallel for
      }      // end omp parallel
   }
   return 0;
}


vecD FFFDiffCorrector::backConvolutionRL(const vecD &vecW,
                                         const matD &broadMatrix,
                                         const double deltaT)
{
   vecD conv(vecW.size());
   for(uint i=0; i < vecW.size(); ++i){
      double cSum = 0.0;
      for(uint j=0; j < vecW.size(); ++j)
         cSum += vecW[j] * broadMatrix[i][j];// deltaT;
      conv[i] = cSum;
   }
   return conv;
}

lDouble FFFDiffCorrector::sqDiff(const vecD &vec1,
                                 const vecD &vec2)
{
   lDouble result = 0.0;
   for(uint j=0; j < vec1.size(); ++j)
      result += lDouble(vec1[j]) * lDouble(vec2[j]);
   return result;
}






matD FFFDiffCorrector::getResults()
{
   return dataCorr;
}



bool FFFDiffCorrector::calcRMeas(double voidPeak, uint *startIndex, int *errorCode)
{
   rMeas.resize(workTimes.size());

   vecD tempRMeas = vecD(workTimes.size());

   for(uint j = 0; j < workTimes.size(); j++){
      rMeas[j] = voidPeak / workTimes[j];
      if(rMeas[j] <= 0.0){
         *errorCode = 8;
         return false;
      }
   }
   *startIndex = 0;
   return true;
}


bool FFFDiffCorrector::calcLambda(//double voidPeakTime,
                                  //double elutionFlow,
                                  double crossFlow,
                                  double chWidth,
                                  double chVolume,
                                  //double relFocusPoint,
                                  //double chLength,
                                  uint *startIndex,
                                  int *errorCode
                                  )
{

#define IS_ZERO(value, errorVal){\
   if(isZero(value)){\
   *errorCode = errorVal;\
   return false; \
}}
/*
   double volume(0.0);
   {
      double tmpFlow = (elutionFlow + crossFlow)/ crossFlow;
      IS_ZERO(tmpFlow, 52);
      double volumeDivisor = log((relFocusPoint / 100 - tmpFlow) / (1 - tmpFlow));
      IS_ZERO(tmpFlow, 53);
      volume = (crossFlow * voidPeakTime) / volumeDivisor;
      IS_ZERO(tmpFlow, 54);
   }
*/
   double factor = chVolume * 60 / (chWidth * chWidth * crossFlow);
   IS_ZERO(factor, 11);

   uint length = workTimes.size();
   vecD tempLambda(length);

   for(uint j = 0; j < length; j++){
      tempLambda.at(j) = factor * workDCoeffs.at(j);
      IS_ZERO(tempLambda.at(j), 12);
   }

   // find all Lambda(t) with  values >= 0.5 and remove them in the final vector
   uint initSkip = 0;

   for(uint j = 0; j < length; j++){
      if(tempLambda.at(j) >= 0.5) initSkip++;
      else break;
   }

   uint newLength = length - initSkip;
   *startIndex += initSkip;


   lambda.resize(newLength);

   for(uint j = 0; j < newLength; j++)
      lambda.at(j) = tempLambda.at(j + initSkip);
   return true;

#undef IS_ZERO
}

bool FFFDiffCorrector::calcChi(uint *startIndex, int *errorCode)
{

   uint length = workTimes.size();
   vecD tempChi(length);
   for(uint j = 0; j < length; j++){
      double lam = lambda.at(j);
      double exp1oLam = exp((-1) / lam);
      double summand1 = (28.0 * lam * lam + 1.0)*(1.0 - exp1oLam);
      double summand2 = (-10.0) * lam * (1.0 + exp1oLam);
      double summand3 = (-1.0) / (3.0 * lam * lam);
      double summand4 = (-2.0) / lam;
      double summand5 = (-1.0) / (lam * (1.0 - exp1oLam));
      summand5 *= 4.0 * lam * (1.0 - summand5) - 1.0 / (3.0 * lam) - 6.0;
      const double summand6 = 4.0;
      double sumFactor = summand1 + summand2 + summand3 + summand4 + summand5 + summand6;
      double thisChi = 24.0 * lam * lam * lam;
      thisChi /= (1.0 + exp1oLam - 2.0 * lam * (1.0 - exp1oLam));
      thisChi *= sumFactor;
      if(isZero(thisChi)){*errorCode = 13; }
      tempChi[j] = thisChi;
   }

   uint initSkip = 0;
   for(uint j = 0; j < length; j++){
      if(tempChi.at(j) <= 0.0) initSkip++;
      else break;
   }
   length -= initSkip;
   *startIndex = initSkip;
   chi.resize(length);
   for(uint j = 0; j < length; j++){
      chi.at(j) = tempChi.at(j + initSkip);
   }

   return true;
}

bool FFFDiffCorrector::calcSigmaSq(//double voidPeakTime,
                                   double elutionFlow,
                                   double crossFlow,
                                   double chWidth,
                                   double chVolume,
                                   double chLength,
                                   double b0,
                                   double bL,
                                   double relFocusPoint,
                                   uint *startIndex,
                                   int *errorCode)
{

#define IS_ZERO(value, errorVal){\
   if(isZero(value)){\
   *errorCode = errorVal;\
   return false; \
}}

   uint length = workTimes.size();
   vecD tempSigmaSq(length);

   double tAverageFlow(0.0);
   {
      double vA = crossFlow * chWidth / chVolume;
      tAverageFlow = elutionFlow + crossFlow;
      tAverageFlow -= vA *
            (b0 * relFocusPoint - relFocusPoint * relFocusPoint * (b0-bL) ) /
            2 *chLength;
      tAverageFlow /= chWidth * (b0  - (b0 - bL) * relFocusPoint / chLength );
   }


   double tD = 0;
   double dataSq = 0;
   for(uint j = 0; j < length; j++){
      tD = workDCoeffs.at(j) ;
      dataSq = workTimes.at(j);
      dataSq *= dataSq;
      //  old Version with elutionFlow in plate height definition

      tempSigmaSq.at(j) = (2 * tD
                           / (rMeas[j] * elutionFlow))
            + (chi[j] * chWidth * chWidth * elutionFlow / tD);

      tempSigmaSq.at(j) *= dataSq / chLength;
      IS_ZERO(tempSigmaSq.at(j), 14);
   }
   uint initSkip = 0;
   for(uint j = 0; j < length; j++){
      if(tempSigmaSq.at(j) <= 0.0) initSkip++;
      else break;
   }
   length -= initSkip;
   *startIndex += initSkip;


   sigmaSq.resize(length);
   for(uint j = 0; j < length; j++){
      sigmaSq.at(j) = tempSigmaSq.at(j + initSkip);
   }
#undef IS_ZERO

   return true;
}

bool FFFDiffCorrector::calcGWFolded(uint normMode)
{

   double thisTimeJ;
   double thisTimeJSum;
   double thisSummand;
   double thisGW;
   double thisSigmaSq;

   double sqTimeDiff = 0;


   for (uint i=0; i < GWFolded.size(); i++){

      for (uint j = 0; j < finalLength; j++){
         thisGW = 0;
         thisTimeJ = workTimes.at(j);
         for (uint jSum = 0; jSum < finalLength; jSum++){
            thisTimeJSum = workTimes.at(jSum);
            sqTimeDiff = thisTimeJ - thisTimeJSum;
            sqTimeDiff *= sqTimeDiff;
            thisSigmaSq = sigmaSq[jSum];
            thisSummand = dataCorr[i][jSum] / sqrt(thisSigmaSq);
            thisSummand *= exp((-1) * sqTimeDiff / (2 * thisSigmaSq));
            thisGW += thisSummand;
         }
         GWFolded[i][j] = thisGW;
      }
   }
   normalize(GWFolded, normMode);
   return true;
}

bool FFFDiffCorrector::isContainedinList(uint i, QList<uint> *l, uint *pos)
{

   bool r = false;
   for(uint t : *l )
      if(t == i){
         r = true;
         if(pos) *pos = t;
      }
   return r;
}

matD FFFDiffCorrector::calcRichLucyBroadMatrix(const vecD &workTimes, const vecD &sigmaSq)
{
   const uint dataLength = workTimes.size();
   const double deltaT = std::abs(workTimes.at(dataLength-1) - workTimes[0]) /
         (double) dataLength;
   matD broadMatrix(dataLength);
   for(uint i = 0; i < dataLength; ++i){
      broadMatrix[i] = vecD(dataLength);
      for(uint j = 0; j < dataLength; ++j){
         double matrixEntry = workTimes[i] - workTimes[j];
         matrixEntry *= matrixEntry;
         matrixEntry /= 2 * sigmaSq[j];
         matrixEntry = std::exp(-matrixEntry);
         matrixEntry *= deltaT / (std::sqrt(2 * PI * sigmaSq[j]));
         broadMatrix[i][j] = matrixEntry;
      }
   }
   return broadMatrix;
}

bool FFFDiffCorrector::offsetNormalize(matD& dataToNormalize,
                                       uint mode)
{

   bool ok;
   switch(mode){
   case 0:
      ok = offsetNormalizeMode0(dataToNormalize);
      break;
   default:
      ok = offsetNormalizeMode0(dataToNormalize);
      break;
   }
   return ok;
}

bool FFFDiffCorrector::offsetNormalize(vecD& dataToNormalize, uint mode)
{
   bool ok;
   switch(mode){
   case 0:
      ok = offsetNormalizeMode0(dataToNormalize);
      break;
   default:
      ok = offsetNormalizeMode0(dataToNormalize);
   }
   return ok;
}

bool FFFDiffCorrector::normalize(matD& dataToNormalize,
                                 uint mode)
{
   bool ok;
   switch(mode){
   case 0:
      ok = normalizeMode0(dataToNormalize);
      break;
   default:
      ok = normalizeMode0(dataToNormalize);
   }
   return ok;
}

bool FFFDiffCorrector::normalize(vecD& dataToNormalize, uint mode)
{
   bool ok;
   switch(mode){
   case 0:
      ok = normalizeMode0(dataToNormalize);
      break;
   default:
      ok = normalizeMode0(dataToNormalize);
   }
   return ok;
}

bool FFFDiffCorrector::offsetNormalizeMode0(matD& dataToNormalize)
{
   double dataMin;
   double dataMax;
   for (uint i = 0; i < dataToNormalize.size(); i++){

      minElementIndex(dataToNormalize.at(i), dataToNormalize.at(i).size() -1, &dataMin);
      maxElementIndex(dataToNormalize.at(i), dataToNormalize.at(i).size() -1, &dataMax);

      if(dataMin < 0.0){
         if(dataMin == dataMax){
            return false;
         }

         for(uint j = 0; j < dataToNormalize.at(i).size(); j++)
            dataToNormalize[i][j] =
                  (dataToNormalize[i][j] -  dataMin) / (dataMax - dataMin);
      } else {

         if((dataMax <= 0.0) && (dataMax >= 0.0)){
            return false;
         }

         for(uint j = 0; j < dataToNormalize.at(i).size(); j++)
            dataToNormalize[i][j] = dataToNormalize[i][j] / dataMax;
      }

   }

   return true;
}

bool FFFDiffCorrector::offsetNormalizeMode0(vecD& dataToNormalize)
{

   double dataMin;
   double dataMax;
   minElementIndex(dataToNormalize, dataToNormalize.size() -1, &dataMin);
   maxElementIndex(dataToNormalize, dataToNormalize.size() -1, &dataMax);

   if(dataMin < 0.0){
      if(dataMin == dataMax){

         return false;
      }
      for(uint j = 0; j < dataToNormalize.size(); j++)
         dataToNormalize.at(j) = (dataToNormalize.at(j) - dataMin) / (dataMax - dataMin);
   } else {
      if((dataMax <= 0.0) && (dataMax >= 0.0)){
         return false;
      }
      for(uint j = 0; j < dataToNormalize.size(); j++)
         dataToNormalize.at(j) = dataToNormalize.at(j) / dataMax;
   }
   return true;
}

bool FFFDiffCorrector::normalizeMode0(matD& dataToNormalize)
{
   double dataMax;

   for (uint i = 0; i < dataToNormalize.size(); i++){
      maxElementIndex(dataToNormalize.at(i), dataToNormalize.at(i).size() -1, &dataMax);
      if((dataMax <= 0.0) && (dataMax >= 0.0)){
         return false;
      }
      for(uint j = 0; j < dataToNormalize.at(i).size(); j++)
         dataToNormalize.at(i).at(j) = dataToNormalize.at(i).at(j) / dataMax;
   }
   return true;
}

bool FFFDiffCorrector::normalizeMode0(vecD& dataToNormalize)
{
   double dataMax;
   maxElementIndex(dataToNormalize, dataToNormalize.size() -1, &dataMax);
   if((dataMax <= 0.0) && (dataMax >= 0.0)){
      return false;
   }
   for(uint j = 0; j < dataToNormalize.size(); j++)
      dataToNormalize.at(j) = dataToNormalize.at(j) / dataMax;
   return true;
}

bool FFFDiffCorrector::isZero(double x) const
{
   if ((x < 0.0) || (x > 0.0)){
      return false;
   } else {
      return true;
   }
}

uint FFFDiffCorrector::minElementIndex(const vecD& searchData,
                                       uint endIndex,
                                       double* minElementValue) const
{
   uint resultingIndex = 0;
   double elementValue = searchData.at(0);
   for (uint j = 0; j < endIndex; j++){
      if(searchData.at(j) < elementValue){
         resultingIndex = j;
         elementValue = searchData.at(j);
      }
   }
   if(minElementValue) *minElementValue = elementValue;
   return resultingIndex;
}

uint FFFDiffCorrector::maxElementIndex(const vecD& searchData,  uint endIndex,
                                       double* maxElementValue) const
{
   uint resultingIndex = 0;
   double elementValue = searchData.at(0);
   for (uint j = 0; j < endIndex; j++){
      if( searchData.at(j) > elementValue){
         resultingIndex = j;
         elementValue = searchData.at(j);
      }
   }
   if(maxElementValue) *maxElementValue = elementValue;
   return resultingIndex;
}

void FFFDiffCorrector::offsetCorrection(matD &v)
{
   double min = matrixMinimum(v);
   if(min < 0.0){
      min = abs(min);
      for(uint i = 0; i < v.size(); ++i){
         vecD vTmp = v.at(i);
         for(uint j = 0; j < vTmp.size(); ++j){
            vTmp[j] += min;
         }
      }
      emit logWarningToMainThread(tr("Negative data triggered additional offset correction.\n\
                                     Shifted value by %1").arg(min)
                                     );
   }
}

double FFFDiffCorrector::matrixMinimum(matD &v)
{
   double min = v.at(0).at(0);
   for(uint i = 0; i < v.size(); ++i){
      vecD vTmp = v.at(i);
      for(uint j = 0; j < vTmp.size(); ++j){
         if(vTmp.at(j) < min) min = vTmp.at(j);
      }
   }
   return min;
}

void FFFDiffCorrector::correctVectorToPositive(vecD &vec)
{
   for(uint j = 0; j < vec.size(); ++j)
      correctToPositive(vec[j]);
}

void FFFDiffCorrector::correctMatrixToPositive(matD &mat)
{
   for(uint i = 0; i < mat.size(); ++i)
      for(uint j = 0; j < mat[i].size(); ++j)
         correctToPositive(mat[i][j]);
}

void FFFDiffCorrector::constBaseLineCorrection(matD &workData)
{
   for(uint i = 0; i < workData.size(); ++i){
      double minVal = *std::min_element(workData[i].begin(), workData[i].end());
      for(double &val : workData[i]) val -= minVal;
   }
}

void FFFDiffCorrector::constBaseLineCorrection_par(matD &workData)
{

#pragma omp parallel// private(thread_id)
   {
#pragma omp for
      for(uint i = 0; i < workData.size(); ++i){
         double minVal = *std::min_element(workData[i].begin(), workData[i].end());
         for(double &val : workData[i]) val -= minVal;
      }
   }
}

void FFFDiffCorrector::linearBaseLineCorrection(matD &workData, double leftAutoBaseBorder, double rightAutoBaseBorder)
{
   const uint wDataLength = workData[0].size();
   const uint leftBorderIndex  = uint (wDataLength * leftAutoBaseBorder  / 100);
   const uint rightBorderIndex = uint (wDataLength * (1 - (rightAutoBaseBorder / 100)));
   for(uint i = 0; i < workData.size(); ++i){
      // base line angle points
      double X1 = 0.0;
      double Y1 = std::numeric_limits<double>::max();
      for(uint j = 0; j <= leftBorderIndex; ++j){
         double test = workData[i][j];
         if(test < Y1){
            Y1 = test;
            X1 = workTimes[j];
         }
      }
      double X2 = 0.0;
      double Y2 = std::numeric_limits<double>::max();
      for(uint j = rightBorderIndex; j < wDataLength; ++j){
         double test = workData[i][j];
         if(test < Y2){
            Y2 = test;
            X2 = workTimes[j];
         }
      }
      // base line parameters
      double slope = (Y2 - Y1) / (X2 - X1);
      double offset = Y2 - (slope * X2);
      // subtract base line
      for(uint j = 0; j < wDataLength; ++j)
         workData[i][j] -= slope * workTimes[j] + offset;
   }
}

void FFFDiffCorrector::linearBaseLineCorrection_par(matD &workData, double leftAutoBaseBorder, double rightAutoBaseBorder)
{


   const uint wDataLength = workData[0].size();
   const uint leftBorderIndex  = uint (wDataLength * leftAutoBaseBorder  / 100);
   const uint rightBorderIndex = uint (wDataLength * (1 - (rightAutoBaseBorder / 100)));
#pragma omp parallel// private(thread_id)
   {
#pragma omp for
      for(uint i = 0; i < workData.size(); ++i){

         // base line angle points
         double X1 = 0.0;
         double Y1 = std::numeric_limits<double>::max();
         for(uint j = 0; j <= leftBorderIndex; ++j){
            double test = workData[i][j];
            if(test < Y1){
               Y1 = test;
               X1 = workTimes[j];
            }
         }

         double X2 = 0.0;
         double Y2 = std::numeric_limits<double>::max();
         for(uint j = rightBorderIndex; j < wDataLength; ++j){
            double test = workData[i][j];
            if(test < Y2){
               Y2 = test;
               X2 = workTimes[j];
            }
         }

         double slope = (Y2 - Y1) / (X2 - X1);
         double offset = Y2 - (slope * X2);

         for(uint j = 0; j < wDataLength; ++j)
            workData[i][j] -= slope * workTimes[j] + offset;
      }
   }
}

void FFFDiffCorrector::emitUpdatePlot()
{
   emit updatePlot();
}
