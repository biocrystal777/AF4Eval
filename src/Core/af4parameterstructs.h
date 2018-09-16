#ifndef AF4PARAMETERSTRUCTS_H
#define AF4PARAMETERSTRUCTS_H

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

enum struct DeconWriteFreq {
   all,
   each5th,
   each10th,
   each50th,
   each100th,
   each500th,
   each1000th,
   last
};



struct AllCalibrationParameters {
   const double temperature;
   const double elutionFlow;
   const double crossFlow;
   const double relFocusPoint;
   const double leftOffsetTime;
   const double voidPeakTime;
   const double elutionTime;
   const double diffCoeff;
   const double chWidth;
   const double hydrodynVolume;
   const double geometVolume;

   const QString date;
   const QString sample;
   const QString buffer;
   const QString additionalNotes;
};

struct ChannelDims {
   const double length1;
   const double length2;
   const double length3;
   const double chLength;
   const double b0;
   const double bL;
};

struct ChannelDimsFromCalib {
   const double chWidth;
   const double hydrodynVolume;
   const double geometVolume;
};

struct ParametersForCalibration {
   const double elutionFlow;
   const double crossFlow;
   const double relFocusPoint;
   const double leftOffsetTime;
   const double voidPeakTime;
   const double elutionTime;
   const double diffCoeff;
};


struct SLSCalibrationParameters{
   const double riConstant;
   const double mallsConstant;
   const QList<bool> usedAngles;
   const QList<double> angleConstants;
};

struct Peakproperties{
   const double startTime;
   const double endTime;
   const bool   secondVirUse;
   const double secondVirCoeff;
   const bool   thirdVirUse;
   const double thirdVirCoeff;
   const double refIndexInc;
   const double waveLength;
   const double epsilon;
};

struct SampleProperties{
   const QList<double> startTimes;
   const QList<double> endTimes;
   const QList<bool> secondVirUses;
   const QList<double> secondVirCoeffs;
   const QList<bool> thirdVirUses;
   const QList<double> thirdVirCoeffs;
   const QList<double> refIndexIncs;
   const QList<double> waveLengths;
   const QList<double> epsilons;
};




enum struct SLSConcMode{
   FromRI,
   FromUVVis
};

enum struct SLSPlotMode {
   Kratky,
   Zimm,
   Berry,
   Guinier
};

struct posPeakPair{
   uint position; // index
   int peak;      // peakNumber
};


struct SLSParameters {
   //bool cFromRefIndex;
   //bool cFromWaveLength;
   double laserWL;
   double riSolvent;
   //double concCut;
};





// For later modules

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
   uint   itMin;
   uint   itMax;
   double maxRMS;
   double leftAutoBaseBorder;
   double rightAutoBaseBorder;
   uint   algorithm;
   uint   parallelMode;
   uint   baseLineCorrMode;
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


#endif // AF4PARAMETERSTRUCTS_H
