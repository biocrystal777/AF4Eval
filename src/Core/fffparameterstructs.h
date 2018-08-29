#ifndef FFFPARAMETERSTRUCTS_H
#define FFFPARAMETERSTRUCTS_H
#include <inttypes.h>
#include <QList>
#include <QString>
struct ExpFFFParameters{
   double temperature;
   double viscosity;
   double voidPeakTime;
   double relFocusPoint;
   double elutionFlow;
   double crossFlow;
   double leftOffsetTime;
};

enum class DeconWriteFreq {
   all,
   each5th,
   each10th,
   each50th,
   each100th,
   each500th,
   each1000th,
   last
};



struct FFFDiffCorrParameters{
   double leftOffsettime;
   bool   recalcVolume;
   double voidPeakTime;
   double elutionFlow;
   double crossFlow;
   double r0;
   double startTime;
   double endTime;
   double relFocusPoint;
   uint itMin;
   uint itMax;
   double maxRMS;
   double leftAutoBaseBorder;
   double rightAutoBaseBorder;
   uint algorithm;
   uint parallelMode;
   uint baseLineCorrMode;

};

struct SLSParameters {
   bool kratky;
   bool zimm;
   bool berry;
   bool guinier;
   bool cFromRefIndex;
   bool cFromWaveLength;
   double laserWL;
   double riSolvent;
   double concCut;
};

struct AllCalibrationParameters {
   double temperature;
   double elutionFlow;
   double crossFlow;
   double relFocusPoint;
   double leftOffsetTime;
   double voidPeakTime;
   double elutionTime;
   double diffCoeff;
   double chWidth;
   double hydrodynVolume;
   double geometVolume;

   QString date;
   QString sample;
   QString buffer;
   QString additionalNotes;
};


struct ChannelDims {
   double chLength;
   double length1;
   double length2;
   double length3;
   double b0;
   double bL;

};

struct ChannelDimsFromCalib {
   double chWidth;
   double hydrodynVolume;
   double geometVolume;
};

struct ParametersForCalibration {
   double elutionFlow;
   double crossFlow;
   double relFocusPoint;
   double leftOffsetTime;
   double voidPeakTime;
   double elutionTime;
   double diffCoeff;
};

struct DiffCorrParameters {
   bool useRL;
   double tStartRL;
   double tEndRL;
   double maxRMSRL;
   uint minIterationsRL;
   QList<uint> maxIterationsRL;
   bool useIR;
   double tStartIR;
   double tEndIR;
   double maxRMSIR;
   uint minIterationsIR;
   QList<uint> maxIterationsIR;
};

struct SLSCalibrationParameters{
   double riConstant;
   double mallsConstant;
   QList<bool> usedAngles;
   QList<double> angleConstants;
};



struct SampleProperties{
   QList<double> startTimes;
   QList<double> endTimes;
   QList<bool> secondVirUse;
   QList<double> secondVirCoeffs;
   QList<bool> thirdVirUse;
   QList<double> thirdVirCoeffs;
   QList<double> refIndexIncs;
   QList<double> waveLengths;
   QList<double> epsilon;
};
#endif // FFFPARAMETERSTRUCTS_H
