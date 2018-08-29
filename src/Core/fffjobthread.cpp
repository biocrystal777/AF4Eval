#include "fffjobthread.h"

using std::string;
using std::vector;

FFFJobThread::FFFJobThread(int threadId, QObject *parent) :
   QObject(parent),
   threadId(threadId),
   //headLines(nullptr),
   //data(nullptr),
   resultsRL(nullptr),
   resultsIR(nullptr)
   //threadFinished(false)
{
   QObject::connect(this, SIGNAL(finished()), this, SLOT(emitNumberedFinish()));
   this->diffCoeffs = diffCoeffs;
   this->stokesRadii = stokesRadii;
}

void FFFJobThread::setValues(const QString& inputFilePath,
                             const ExpFFFParameters& expFFFParameters,
                             const ChannelDims& channelCalibParameters,
                             const DiffCorrParameters& diffCorrParameters,
                             const SLSParameters& slsParameters,
                             const SLSCalibrationParameters& slsCalibrationParameters,
                             const SampleProperties& sampleProperties)
{
   this->inputFileName = inputFilePath;
   this->expFFFParameters = expFFFParameters;
   this->channelCalibParameters = channelCalibParameters;
   this->diffCorrParameters = diffCorrParameters;
   this->slsParameters = slsParameters;
   this->slsCalibrationParameters = slsCalibrationParameters;
   this->sampleProperties = sampleProperties;
}

void FFFJobThread::runThread()
{


   /////////////////////////////////
   // Check and parse RI/SLS file //
   /////////////////////////////////

   if(!QFile::exists(inputFileName)){
      logWarning(tr("File %1 does not exist!").arg(inputFileName));
      //threadFinished = true;
      emit finished();
      return;
   }
   const QString filePrefix = inputFileName.split('.').first();
   FFFCsvParser parser(inputFileName.toStdString(), ',', '.');
   bool ok;
   int errorCode;
   uint errorInLine;
   errorCode = parser.parseFile(&errorInLine);
   if(errorCode){
      logError(tr("Error %1").arg(errorCode));
      logError(tr("Diffusion Evaluation Aborted while reading the input file."));
      //this->threadFinished = true;
      emit finished();
      return;
   }
   else logText(tr("File %1 read").arg(inputFileName));
   parser.getData();

   headLines = parser.getHeadLines();
   data = parser.getData();

   QList<bool> activated;
   activated.append(false);
   activated.append(true);
   for(uint i = 2; i < headLines.size(); i++)
      activated.append(false);

   const uint timeIndex = 0; // time is first column
   vecD times = data.at(timeIndex);


   //double progress = 0;
   //uint currentLine = 0;
   FFFDiffEvaluator evaluator;
/*
   errorCode = evaluator.evalDiffCoeffs(times,
                                        expFFFParameters.leftOffsetTime,
                                        expFFFParameters.voidPeakTime,
                                        expFFFParameters.elutionFlow,
                                        expFFFParameters.crossFlow,
                                        channelCalibParameters.chWidth,
                                        channelCalibParameters.chVolume,
                                        expFFFParameters.relFocusPoint,
                                        channelCalibParameters.chLength,
                                        false,
                                        &progress,
                                        &currentLine);
*/
   if(errorCode){
      logError(tr("Error while evaulating data: "));
      switch(errorCode){
      case 1: logError(tr("Error code 1: void peakt time must not be 0.")); break;
      case 2: logError(tr("Error code 2: elution flow must not be 0.")); break;
      case 3: logError(tr("Error code 3: cross flow must not be 0.")); break;
      case 4: logError(tr("Error code 4: channel Width must not be 0.")); break;
      case 5: logError(tr("Error code 5: leftoffsetTime must not be bigger than voidPeak")); break;
      }
     // delete data; data = nullptr;
     // delete headLines; headLines = nullptr;
      //this->threadFinished = true;
      emit finished();
      return;
   }
   diffCoeffs = evaluator.getResDiffCoeff();
   evaluator.calcStokesRadii(diffCoeffs, expFFFParameters.viscosity, expFFFParameters.temperature);
   stokesRadii = evaluator.getResStokesRadii();

   ////////////////////////////////
   // write data from evaluation //
   ////////////////////////////////
   {
      QString outFile = filePrefix + tr("_eval.csv");
      FFFCsvWriter writer(outFile.toStdString());
      //vectorMatrix writeResults = vectorMatrix();
      matD writeResults;
      // copy "used" times from data to result matrix
      uint startTimeIndex = evaluator.getEvaluationStart();
      // chop beginning of data matrix to evaluation begin;
      uint oldLength = data.at(0).size();
      uint newLength = oldLength - startTimeIndex;
      for (uint i = 0; i < data.size() ; i++){
         for (uint j = 0; j < newLength; j++){
            data[i][j] = data[i][j + startTimeIndex];
         }
         data[0].resize(newLength);
      }
      writeResults.push_back(data.at(timeIndex)); // time
      writeResults.push_back(diffCoeffs); // diffcoeffs
      writeResults.push_back(stokesRadii);
      for (uint i = 1; i < data.size(); i++) // data
         if(i != timeIndex) writeResults.push_back(data.at(i));
      std::vector<std::string> resultHeadLines;
      resultHeadLines.push_back(std::string("time"));
      resultHeadLines.push_back(std::string("Diffusion Coefficients [cm^2/min]"));
      resultHeadLines.push_back(std::string("Stokes Radius [nm]"));
      for(uint i=1; i < headLines.size(); i++)
         if(i != timeIndex)
            resultHeadLines.push_back(headLines[i]);
      ok = writer.writeFile(writeResults, resultHeadLines);
      if(!ok) logError(tr("output file could not be written."));
   }


   /////////////////////////////////////////////////////
   // Continue with diffusion corrections if required //
   /////////////////////////////////////////////////////


   // cut time column in seperate data set (signalData)

   matD signalData(data.size() - 1);
   for(uint i = 0; i < signalData.size(); ++i){
      signalData[i] = vecD(data.at(i+1));
      //for(uint j = 0; j < signalData->at(i)->size(); ++j){
       //  (*signalData->at(i))[j] = data->at(i+1)->at(j);
      // }
   }

   if(diffCorrParameters.useIR){
      //FFFDiffCorrParameters par = convertToFFFDiffCorrParameters(1);
      //FFFDiffCorrector irCorrector;
      uint newStartIndex;
      uint newEndIndex;




#if QWT_VERSION == 0x060102



#endif

      if(errorCode) evalErrorCodeFromDiffCorr(errorCode, "Iterative Relaxation");
      else
         writeDiffCorrData(filePrefix,
                           timeIndex,
                           times,
                           &diffCoeffs,
                           &stokesRadii,
                           newStartIndex,
                           newEndIndex,
                           diffCorrParameters,
                           resultsIR,
                           1);
   }

   if(diffCorrParameters.useRL){
      //FFFDiffCorrParameters par = convertToFFFDiffCorrParameters(2);
      //FFFDiffCorrector rlCorrector;
      uint newStartIndex;
      uint newEndIndex;


#if QWT_VERSION == 0x060102
      /*
      errorCode = rlCorrector.diffCorrection(times,
                                             signalData,
                                             1,
                                             0,
                                             false,
                                             par,
                                             true,
                                             diffCoeffs,
                                             &newStartIndex,
                                             &newEndIndex,
                                             true,
                                             false,
                                             false,
                                             DeconWriteFreq::last,
                                             0,
                                             nullptr,
                                             nullptr,
                                             false,
                                             nullptr
                                             );
                                             */
#endif

      if(errorCode) evalErrorCodeFromDiffCorr(errorCode, "Iterative Relaxation");
      else
         writeDiffCorrData(filePrefix,
                           timeIndex,
                           times,
                           &diffCoeffs,
                           &stokesRadii,
                           newStartIndex,
                           newEndIndex,
                           diffCorrParameters,
                           resultsRL,
                           2);
   }






   //   freeMemory();
   //this->threadFinished = true;



   ///////////////////////////////////////////
   // Evaluate static light scattering data //
   ///////////////////////////////////////////
/*
   if(slsParameters.zimm){



   //////////////////////////////////////////
   // Retransform data vectors from parser //
   //////////////////////////////////////////

   vecD *times = data->at(timeIndex);
   vecD *diffCoeffs = nullptr;
   if(diffCoeffChosen) diffCoeffs = data->at(diffCoeffIndex);
   vecD *stokesRadii = nullptr;
   if(stokesRadiusChosen) stokesRadii = data->at(rSIndex);

   vectorMatrix *riData = nullptr;
   extractMatrixFromData(data, riIndices, &riData, numberOfPeaks, "Refractive Indices");

   vector<vectorMatrix*> *mallsDataTransposed = nullptr;
   extractTensorFromData(data, mallsIndices, &mallsDataTransposed,
                         NUMBER_OF_DETECTORS, numberOfPeaks, "MALLS Indices");

   vector<vectorMatrix*> *mallsData = new vector<vectorMatrix*>(numberOfPeaks);
   for (int i = 0; i < numberOfPeaks; ++i){
      (*mallsData)[i] = new vectorMatrix(times->size());
      for (uint j = 0; j < times->size(); ++j){
         (*(mallsData->at(i)))[j] = new vecD(NUMBER_OF_DETECTORS);
         for(uint k = 0; k < NUMBER_OF_DETECTORS; ++k){
            (*(mallsData->at(i)->at(j)))[k] = mallsDataTransposed->at(k)->at(i)->at(j);
         }
      }
   }

   for (uint k = 0; k < mallsDataTransposed->size(); ++k) delete mallsDataTransposed->at(k);
   delete mallsDataTransposed;

   ///////////////////////////////
   // create parameter vectors  //
   ///////////////////////////////

   vector<bool> *useDataPoint = new vector<bool>(riData->at(0)->size());
   for(uint j = 0; j < riData->at(0)->size(); ++j) (*useDataPoint)[j] = true;

   vectorMatrix *dnDcs = new vectorMatrix(numberOfPeaks);
   QList<double> dnDcVals = peakParameterFrame->getRefIndexIncs();
   for(int i = 0; i < numberOfPeaks; ++i)
   {
      (*dnDcs)[i] = new vecD(riData->at(0)->size());
      for(uint j = 0; j < riData->at(0)->size(); ++j)
      {
         (*(dnDcs->at(i)))[j] = dnDcVals.at(i);
      }
   }

   vecD mallsCalibConstants = calibrationFrame->getMallsAngleConstants().toVector().toStdVector();
   vector<bool> useAngleFlags = calibrationFrame->getUsedAngles().toVector().toStdVector();


   /////////////////
   // calculation //
   /////////////////

   FFFSLSEvaluator slsEvaluator;
   slsEvaluator.evalRiMALLS_partZimmplot(
            riData->at(0),
            mallsData->at(0),
            useDataPoint,
            dnDcs->at(0),
            laserWaveLength->value() / 1e7,
            refIndexSolvent->value(),
            calibrationFrame->getRIConstant(), //  riConstant->value(),
            concentrationCut->value(),
            calibrationFrame->getMallsConstant(),
            mallsCalibConstants,
            useAngleFlags
            );

   for ( int i = 0; i < (int) (*dnDcs)[ 0 ]->size(); ++i )
   {
      FFFLog::logText( QString( "dnDcs pos %1 val %2" ).arg( i ).arg( (*(*dnDcs)[ 0 ])[ i ] ) );
   }

   vecD *gyrRadii = slsEvaluator.getRadGyrs(&ok);
   if(!ok){
      FFFLog::logError(tr("Rad of gyr vector does not exist."), true);
      delete riIndices;
      delete mallsIndices;
      for(uint i = 0; i < data->size(); ++i) delete data->at(i);
      delete data;
      delete headLines;
      delete riData;
      for(uint k = 0; k < mallsData->size(); ++k) delete mallsData->at(k);
      delete mallsData;
      delete useDataPoint;
      for(uint i = 0; i < dnDcs->size(); ++i) delete dnDcs->at(i);
      delete dnDcs;
      return;
   }
   vecD *MWs = slsEvaluator.getMolWeights(&ok);
   if(!ok){
      FFFLog::logError(tr("Mol weights vector does not exist."), true);
      delete riIndices;
      delete mallsIndices;
      for(uint i = 0; i < data->size(); ++i) delete data->at(i);
      delete data;
      delete headLines;
      delete riData;
      for(uint k = 0; k < mallsData->size(); ++k) delete mallsData->at(k);
      delete mallsData;
      delete useDataPoint;
      for(uint i = 0; i < dnDcs->size(); ++i) delete dnDcs->at(i);
      delete dnDcs;
      delete gyrRadii;
      return;
   }

   vecD *rhos = nullptr;
   if(calculateShapeFactor){
      if(stokesRadiusChosen){
         slsEvaluator.calcRho(stokesRadii, gyrRadii, &rhos);
      } else {
         FFFLog::logWarning(tr("No column with Stokes radii specified. Shape factor rho cannot be calculated."));
      }
   }

   vector<string> *resultHeadLines = new vector<string>;
   vectorMatrix *resultData = new vectorMatrix;

   resultHeadLines->push_back(headLines->at(timeIndex));
   resultData->push_back(times);
   if(diffCoeffChosen){
      resultHeadLines->push_back(headLines->at(diffCoeffIndex));
      resultData->push_back(diffCoeffs);
   }

   resultHeadLines->push_back(string("Molecular Weight [g/mol]"));
   resultData->push_back(MWs);

   resultHeadLines->push_back(string("Radius of Gyration [nm]"));
   resultData->push_back(gyrRadii);

   if(stokesRadiusChosen){
      resultHeadLines->push_back(headLines->at(rSIndex));
      resultData->push_back(data->at(rSIndex));
      if(calculateShapeFactor->isChecked()){
         resultHeadLines->push_back(string("rho"));
         resultData->push_back(rhos);
      }
   }

   for(int i = 0; i < riIndices->size(); ++i){
      resultHeadLines->push_back(headLines->at(riIndices->at(i).position));
      resultData->push_back(data->at(riIndices->at(i).position));
   }

   for(int k = 0; k < mallsIndices->size(); ++k){
      for(int i = 0; i < mallsIndices->at(k).size(); ++i){
         resultHeadLines->push_back(headLines->at(mallsIndices->at(k).at(i).position));
         resultData->push_back(data->at(mallsIndices->at(k).at(i).position));
      }
   }



   FFFCsvWriter fileWriter(outFileName.toStdString());
   errorCode = fileWriter.writeFile(resultData, resultHeadLines);


   // free memory
   delete riIndices;
   delete mallsIndices;
   for(uint i = 0; i < data->size(); ++i) delete data->at(i);
   delete data;
   delete headLines;
   delete riData;
   for(uint k = 0; k < mallsData->size(); ++k) delete mallsData->at(k);
   delete mallsData;
   delete useDataPoint;
   for(uint i = 0; i < dnDcs->size(); ++i) delete dnDcs->at(i);
   delete dnDcs;
   delete gyrRadii;
   if(rhos) delete rhos;
   delete resultHeadLines;
   delete resultData;
   QApplication::restoreOverrideCursor();



   }




*/




   emit finished();
   return;
}



void FFFJobThread::freeMemory()
{
   //if(headLines) delete headLines;                                 headLines = nullptr;
   //if(data){ for(auto &p : *data) if(p) delete p; delete data; }   data = nullptr;
   //if(diffCoeffs) delete diffCoeffs;                               diffCoeffs = nullptr;
   //if(stokesRadii) delete stokesRadii;                             stokesRadii = nullptr;
   if(resultsRL) delete resultsRL;                                 resultsRL = nullptr;
   if(resultsIR) delete resultsIR;                                 resultsIR = nullptr;

}

uint FFFJobThread::getMaximum(const QList<uint> &l) const
{
   uint r = l.first();
   for( uint e : l)
      if(e > r) r = e;
   return r;
}

void FFFJobThread::evalErrorCodeFromDiffCorr(int errorCode, const QString &algoDescription)
{
   if(errorCode){

      logError(tr("Diffusion correction using %1 could not be finished.").arg(algoDescription));
      switch(errorCode) {
      case 1: logError("1: voidPeak time must not be 0."); break;
      case 2: logError("2: elution flow must not be 0."); break;
      case 3: logError("3: cross flow mmust not be 0."); break;
      case 4: logError("4: channel Width must not be 0."); break;
      case 5: logError("5: channel Length must not be 0."); break;
      case 6: logError("6: RMSmax must not be 0."); break;
      case 7: logError("7: A diffusion coefficient read from file was 0."); break;
      case 8: logError("8: An rMeas value has become 0."); break;
      case 9: logError("9: An intermediate value has become 0 during the calculation of lambda."); break;
      case 10: logError("10: An intermediate value has become 0 during the calculation of lambda."); break;
      case 11: logError("11: An intermediate value has become 0 during the calculation of lambda."); break;
      case 12: logError("12: A resulting lambda has become 0."); break;
      case 13: logError("13: A resulting chi has become 0."); break;
      case 14: logError("14: A resulting sigmaSq has become 0."); break;
      case 20: logError(tr("20: t<sub>void</sub> time must not be smaller than t<sub>0</sub>")); break;
      case 21: logError(tr("21: t<sub>start</sub> time must not be smaller than t<sub>void</sub>")); break;
      case 30: logError("30: Error occured during first offset normalization."); break;
      case 31: logError("31: Error occured during calculation of parameters rMeas.");
      case 32: logError("32: Error occured during calculation of parameters lambda."); break;
      case 33: logError("33: Error occured during calculation of parameters chi."); break;
      case 34: logError("34: Error occured during calculation of parameters sigmaSq."); break;

      default: logError(tr("Error code %1: not specified.").arg(errorCode)); break;
      }
      finished();
      return;
   } else logText(tr("Diffusion Correction finished")); //after iterations.").arg(*progress));


}

FFFDiffCorrParameters FFFJobThread::convertToFFFDiffCorrParameters(const uint algorithm) const
{
  FFFDiffCorrParameters par;
  /*
   //par.leftOffsettime = 0.0;
   //par.voidPeakTime = expFFFParameters.voidPeakTime - expFFFParameters.leftOffsetTime;
   // par.elutionFlow = expFFFParameters.elutionFlow;
   // par.crossFlow = expFFFParameters.crossFlow;
   //par.chWidth = channelCalibParameters.chWidth;
   //par.chLength = channelCalibParameters.chLength;
   //par.b0 = channelCalibParameters.b0;
   //par.bL = channelCalibParameters.bL;
   //par.r0 = 2.0; // const value for IR diffCorrection;
   par.relFocusPoint = expFFFParameters.relFocusPoint;
   switch (algorithm){
   case 1:
      par.startTime = diffCorrParameters.tStartIR - expFFFParameters.leftOffsetTime;
      par.endTime = diffCorrParameters.tEndIR - expFFFParameters.leftOffsetTime;
      par.itMin = diffCorrParameters.minIterationsIR;
      par.itMax = this->getMaximum(diffCorrParameters.maxIterationsIR);
      par.maxRMS = diffCorrParameters.maxRMSIR;
      break;
   case 2:
      par.startTime = diffCorrParameters.tStartRL - expFFFParameters.leftOffsetTime;
      par.endTime = diffCorrParameters.tEndRL - expFFFParameters.leftOffsetTime;
      par.itMin = diffCorrParameters.minIterationsRL;
      par.itMax = this->getMaximum(diffCorrParameters.maxIterationsRL);
      par.maxRMS = diffCorrParameters.maxRMSRL;
      break;
   }*/
   return par;
}

bool FFFJobThread::writeDiffCorrData(const QString &filePrefix,
                                     const uint timeIndex,
                                     vecD &times,
                                     vecD *diffCoeffs,
                                     vecD *stokesRadii,
                                     uint newStartIndex,
                                     uint newEndIndex,
                                     DiffCorrParameters diffCorrParameters,
                                     vector<vectorMatrix*>* results,
                                     uint algorithm)
{
   bool allOk(true);
   bool ok(true);

   // make new Directory
   QString newDirName = filePrefix;
   while(!newDirName.endsWith('/')) newDirName.chop(1);
   switch(algorithm){
   case 1:
      newDirName.append(tr("DeconvIR"));
      break;
   case 2:
      newDirName.append(tr("DeconvRL"));
      break;
   }
   QDir newDir(newDirName);
   if(!(newDir.exists()) && !(newDir.mkpath(newDirName))) return false;

   // prepare headlines for all files
   std::vector<std::string> resultHeadLines;
   resultHeadLines.push_back(std::string("time"));
   resultHeadLines.push_back(std::string("Diffusion Coefficients [cm^2/min]"));
   resultHeadLines.push_back(std::string("Stokes Radius [nm]"));
   for(uint i = 0; i < headLines.size(); ++i){
      if(i != timeIndex) resultHeadLines.push_back(headLines.at(i));
   }
   // make first 3 identical columns to write
   const uint newLength(newEndIndex - newStartIndex);
   vecD *newTimes = new vecD(newLength);
   vecD *newDiffCoeffs = new vecD(newLength);
   vecD *newStokesRadii = new vecD(newLength);
   uint jB = newStartIndex;
   for(uint j = 0; j < newLength; ++j,++jB){
      (*newTimes)[j] = times[jB];
      (*newDiffCoeffs)[j] = diffCoeffs->at(jB);
      (*newStokesRadii)[j] = stokesRadii->at(jB);
   }
   for(uint h = 0; h < results->size(); ++h){
      vectorMatrix* resAtH = results->at(h);
      matD writeResults;
      writeResults.push_back(*newTimes);         // time
      writeResults.push_back(*newDiffCoeffs);    // diffcoeffs
      writeResults.push_back(*newStokesRadii);   // stokesRadii
      for (uint i = 0; i < resAtH->size(); ++i) // data
         writeResults.push_back(*(resAtH->at(i)));


      QString outFile;
      switch(algorithm){
      case 1:
         outFile = newDirName + tr("/It%1.csv").arg(diffCorrParameters.maxIterationsIR.at(h));
         break;
      case 2:
         outFile = newDirName + tr("/It%1.csv").arg(diffCorrParameters.maxIterationsRL.at(h));
         break;
      }
      FFFCsvWriter writer(outFile.toStdString());
      ok = writer.writeFile(writeResults, resultHeadLines);
      if(!ok){
         emit logError(tr("output file could not be written."));
         allOk = false;
      }
   }
   delete newTimes;       newTimes = nullptr;
   delete newDiffCoeffs;  newDiffCoeffs = nullptr;
   delete newStokesRadii; newStokesRadii = nullptr;
   return allOk;
}

void FFFJobThread::emitNumberedFinish()
{

   emit finished(this->threadId);
}

void FFFJobThread::emitLogError(QString message)
{
   emit logError(message);
}



void FFFJobThread::emitLogText(QString message)
{
   emit logText(message);
}

void FFFJobThread::emitLogWarning(QString message)
{
   emit logWarning(message);
}

FFFJobThread::~FFFJobThread()
{
   freeMemory();
}
