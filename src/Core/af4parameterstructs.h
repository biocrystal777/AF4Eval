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

enum struct SLSConcentrationMode{
   FromRI,
   FromUVVis
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

/*
struct Peakproperties{

   Peakproperties(double startTimeP,
                  double endTimeP,
                  bool   secondVirUseP,
                  double secondVirCoeffP,
                  bool   thirdVirUseP,
                  double thirdVirCoeffP,
                  double refIndexIncP,
                  double waveLengthP,
                  double epsilonP)
                : startTime     (startTimeP),
                  endTime       (endTimeP),
                  secondVirUse  (secondVirUseP),
                  secondVirCoeff(secondVirCoeffP),
                  thirdVirUse   (thirdVirUseP),
                  thirdVirCoeff (thirdVirCoeffP),
                  refIndexInc   (refIndexIncP),
                  waveLength    (waveLengthP),
                  epsilon       (epsilonP) {}


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
*/

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
   QList<double> startTimes;
   QList<double> endTimes;
   QList<bool> secondVirUses;
   QList<double> secondVirCoeffs;
   QList<bool> thirdVirUses;
   QList<double> thirdVirCoeffs;
   QList<double> refIndexIncs;
   QList<double> waveLengths;
   QList<double> epsilons;
};
#endif // AF4PARAMETERSTRUCTS_H
