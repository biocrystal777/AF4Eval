#include "fffdiffcorrectionthread.h"

using std::string;
using std::vector;


//#if QWT_VERSION == 0x060102
void FFFDiffCorrectionThread::emitUpdatePlot(){
   emit updatePlot();
}

void FFFDiffCorrectionThread::emitInitPlot()
{
   emit this->initPlot();
}
//#endif



void FFFDiffCorrectionThread::runThread()
{
   if(!valuesHaveBeenSet) return;
   matD results;
   matD writeResults;

   matD dataToProcess(signalIndices.size());
   for(int i = 0; i < signalIndices.size(); ++i)
      dataToProcess[i] = inFileData.at(signalIndices.at(i));


   FFFDiffCorrector diffCorrector; // new FFFDiffCorrector();
   QObject::connect(&diffCorrector, SIGNAL(logTextToMainThread(QString)), this, SLOT(logTextSlot(QString)));
   QObject::connect(&diffCorrector, SIGNAL(logWarningToMainThread(QString)), this, SLOT(logWarningSlot(QString)));
   QObject::connect(&diffCorrector, SIGNAL(logErrorToMainThread(QString)), this, SLOT(logErrorSlot(QString)));
//#if QWT_VERSION == 0x060102
   QObject::connect(&diffCorrector, SIGNAL(updatePlot()), this, SLOT(emitUpdatePlot()));
   QObject::connect(&diffCorrector, SIGNAL(initPlot_C()), this, SLOT(emitInitPlot()));
   QObject::connect(this, SIGNAL(signalsToUpdateChanged(const omp_QVector<bool>&)),
                    &diffCorrector, SLOT(setSignalsToUpdate(const omp_QVector<bool>&)));
//#endif


   uint oldStartIndex = diffCorrector.findIndexFromTime(inFileData.at(timeIndex), diffCorrParameters.startTime);
   uint newStartIndex;
   uint newEndIndex;

   *(progress_par) = vector<uint>(dataToProcess.size());

   vector<vector<lDouble> > rmsTracker(dataToProcess.size());

//#if QWT_VERSION == 0x060102

   int errorCode = diffCorrector.diffCorrection(inFileData.at(timeIndex),
                                                dataToProcess,
                                                diffCorrParameters,
                                                dims,
                                                dimsFromCalib,
                                                keepIntegralSize,
                                                inFileData.at(dCIndex),
                                                &newStartIndex,
                                                &newEndIndex,
                                                this->displayCalculationWarnings,
                                                this->writeCSV,
                                                this->writeGnuplot,
                                                this->writeFreq,
                                                0,
                                                plotTData,
                                                plotRefData,
                                                plotCorrData,
                                                plotDataToUpdate,
                                                progress,
                                                progress_par,
                                                checkReConv,
                                                &rmsTracker
                                                );
//#endif


   if(errorCode){
      logError(tr("Diffusion correction could not be finished."));
      switch(errorCode) {
      case 1: logError("1: voidPeak time must not be 0."); break;
      case 2: logError("2: elution flow must not be 0."); break;
      case 3: logError("3: cross flow mmust not be 0."); break;
      case 4: logError("channel Width must not be 0."); break;
      case 5: logError("channel Length must not be 0."); break;
      case 6: logError("RMSmax must not be 0."); break;
      case 7: logError("A diffusion coefficient read from file was 0."); break;
      case 8: logError("An rMeas value has become 0."); break;
      case 9: logError("An intermediate value has become 0 during the calculation of lambda."); break;
      case 10: logError("An intermediate value has become 0 during the calculation of lambda."); break;
      case 11: logError("An intermediate value has become 0 during the calculation of lambda."); break;
      case 12: logError("A resulting lambda has become 0."); break;
      case 13: logError("A resulting chi has become 0."); break;
      case 14: logError("A resulting sigmaSq has become 0."); break;
      case 20: logError(tr("t<sub>void</sub> time must not be smaller than t<sub>0</sub>")); break;
      case 21: logError(tr("t<sub>start</sub> time must not be smaller than t<sub>void</sub>.")); break;
      case 30: logError("Error occured during first offset normalization."); break;
      case 31: logError("Error occured during calculation of parameters rMeas.");
      case 32: logError("Error occured during calculation of parameters lambda."); break;
      case 33: logError("Error occured during calculation of parameters chi."); break;
      case 34: logError("Error occured during calculation of parameters sigmaSq."); break;

      default: logError(tr("Error code %1 not specified.").arg(errorCode)); break;
      }
      finished();
      return;
   }
   else {
      if(diffCorrParameters.parallelMode == 0)
         logText(tr("Diffusion Correction finished after %1 iterations.").arg(*progress));
      else
         logText(tr("Diffusion Correction finished after %1 iterations.").arg(diffCorrParameters.itMax));
   }

   if (oldStartIndex != newStartIndex){
      logWarning(tr("Diffusion Correction Start had to be shifted:"));
      logWarning(tr("Stated time %1").arg(dataToProcess.at(timeIndex).at(oldStartIndex)));
      logWarning(tr("Used Time %1").arg(dataToProcess.at(timeIndex).at(newStartIndex)));
   }
   results = diffCorrector.getResults();


   uint oldLength = inFileData.at(timeIndex).size();
   uint rightChop = inFileData.at(timeIndex).size() - newEndIndex;
   uint newLength = oldLength - newStartIndex - rightChop;

   // chop data left and right

   for (uint i = 0; i < inFileData.size() ; ++i){
      for (uint j = 0; j < newLength; ++j){
         inFileData.at(i).at(j) = inFileData.at(i).at(j + newStartIndex); //transfer window from left to right
      }
      inFileData.at(i).resize(newLength); // chop right
   }
   writeResults.push_back(inFileData.at(timeIndex)); // time
   writeResults.push_back(inFileData.at(dCIndex)); // diffCoeffs
   if(rSIndexSet) writeResults.push_back(inFileData.at(rSIndex));

   for (uint i = 0; i < results.size(); ++i) writeResults.push_back(results.at(i));// corrected Data

   //for (uint i = 0; i < dataToProcess.size(); ++i)      writeResults.push_back(dataToProcess.at(i));
   vector<string> writingHeadLines;
   writingHeadLines.push_back(string("time"));
   writingHeadLines.push_back(string("Diffusion coefficient"));
   if(rSIndexSet) writingHeadLines.push_back(string("Stokes Radius"));



   for (int i = 0; i < signalIndices.size(); ++i)
      writingHeadLines.push_back(//string("After diffCorrection: ").append(
                                    inFileHeadLines.at(signalIndices.at(i))//)
                                 );

   //for (int i = 0; i < signalIndices.size(); ++i)
   //   writingHeadLines.push_back(//string("Raw Data: ").append(
   //                              inFileHeadLines.at(signalIndices.at(i))//)
   //         );


   FFFCsvWriter writer(outputFileName.toStdString());
   if(writer.writeFile(writeResults, writingHeadLines))
      logText(tr("File Written."));
   else logError(tr("File could not be written."));

   vector<string> rmsHeadLines = {"iteration","RMS"};
   FFFCsvWriter writerRMS(outputFileName.append("rms1.csv").toStdString());
   matD rmsData1(2);
   rmsData1[0].resize(diffCorrParameters.itMax);
   std::iota (std::begin(rmsData1[0]), std::end(rmsData1[0]), 1);
   rmsData1[1].resize(diffCorrParameters.itMax);
   for(uint i = 0; i < diffCorrParameters.itMax; ++i){
      rmsData1[1][i] = rmsTracker[0][i];
   }

   writerRMS.writeFile(rmsData1,rmsHeadLines);


   emit finished();
}

void FFFDiffCorrectionThread::emitSignalsToUpdateChanged(const omp_QVector<bool> &s)
{
   emit signalsToUpdateChanged(s);
}


void FFFDiffCorrectionThread::logTextSlot(QString message)
{
   emit logText(message);
}

void FFFDiffCorrectionThread::logWarningSlot(QString message)
{
   emit logWarning(message);
}

void FFFDiffCorrectionThread::logErrorSlot(QString message)
{
   emit logError(message);
}
