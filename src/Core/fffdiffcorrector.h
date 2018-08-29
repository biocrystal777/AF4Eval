#ifndef FFFDIFFCORRECTOR_H
#define FFFDIFFCORRECTOR_H

#include "fffcalculator.h"
#include "fffparameterstructs.h"
#include <cfloat>
#include <vector>
#include <qdebug.h>
#include <QObject>
#include <bits/basic_string.h>
#include "omp.h"
#include "./omp_qvector.h"
#include  "qwt_global.h"

#ifdef QWT_VERSION
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#endif

/**************************************************************
***
***   FFFDiffCorrector
***
**************************************************************/

/*!
* \class FFFDiffCorrector
* \brief FFFDiffCorrector performs the diffusion correction of the program
* \details FFFDiffCorrector is an abstract factory for diffusion correction
*          by iterative deconvolution with a Gaussian (G). The original raw data set is called F,
*          the deconvoluted data are called W_n. The parameters are determined using
*          experimental parameters. The iterative method to calculate W_{n+1} includes a
*          convolution step:
*          \f[    GWConv = G \otimes W_n    \f]
*          As this calculation has O(n^2) complexity, a cheaper approach using FFT might
*          be considered later. Alternative normalization functions may be added by implementing
*          these functions and adding mode flags for them in the overloaded node functions
*          normalize(...) and offsetNormalize(...).
* \author Benedikt Haeusele, Emre Brookes (Graphical updating)
* \version 2.0
* \date July 2013
* \copyright GNU General Public License version 3.0
*/
class FFFDiffCorrector : public QObject, public FFFCalculator
{
   Q_OBJECT

public:

   /*!
   * \brief FFFDiffCorrector default constructor
   * \param data Data Matrix that shall be processed by the class
   */
   explicit FFFDiffCorrector() {}

   /*!
     * \brief ~FFFDiffCorrector default destructor
     */
   ~FFFDiffCorrector(){}

   FFFDiffCorrector(const FFFDiffCorrector& src) = delete;
   FFFDiffCorrector& operator= (const FFFDiffCorrector& src) = delete;
   FFFDiffCorrector(FFFDiffCorrector&& src) = delete;
   FFFDiffCorrector& operator= (FFFDiffCorrector&& src) = delete;

   /*!
   * \brief diffCorrection performs the actual calculation
   * \param times time axis of the data
   * \param algorithm: 1 = iterative relaxation, 2 = richardson-lucy
   * \param leftOffsetTime offset that has to be substrated by all other time values [min]
   * \param voidPeakTime time of the voidpeak [min]
   * \param elutionFlow integrated flow speed [ml/min] of the elution direction
   * \param crossFlow speed of the crossflow [ml/min]
   * \param diffCoeficient diffusion Coefficient for the substance eluated at elutionTime [cm^2/s]
   * \param chWidth channel Width [cm]
   * \param chLength channel Length [cm]
   * \param r0 constant paramter (2.0)
   * \param startTime left border of the actual deconvolution window [min]
   * \param endTime right border of the actual deconvolution window [min]
   * \param relFocusPoint focus point parameter for the corrected channel volume calculation
   * \param itMin minimal number of iterations to be performed
   * \param itMax maximal number of iterations to be performed
   * \param maxRMS root mean square error determines the condition of abortion
   * \param keepIntegralSize flag that controls the function for resizing the integral via
   *         linear scaling (only for iterative deconvolution)
   * \param diffCoeffs Diffusion Coefficient axis, has to cover the range
   *        corresponding to the data matrix
   * \param newStartIndex if a different start index (= left border)
   *        than the one determined by the start Time
   *        had to be chosen due to the actual data it will indicated here
   * \param newEndIndex if a different end index (=right border) than the
   *        one determined by the start Time had to be chosen due to the
   *        actual data it will indicated here
   * \param normMode
   * \param actIteration indicates the current Iteration and thereby
   *        the progress of the calculation
   * \param actIteration indicates current Iteration for progress bars etc.
   * \param plotT time axis for plot interface
   * \param plotY data axis for plot interface (current deconvoluted data)
   * \param plotColor color of the plotted data
   * \param thread reference to the
   * \return return value indicates if the opereation has been performed successfully
   */


#if QWT_VERSION == 0x060102
   int diffCorrection(vecD &times,
                      const matD &data,
                  //    uint algorithm,
                  //    uint parallelMode,
                  //    uint baseLineCorrMode,
                      FFFDiffCorrParameters p,
                      ChannelDims dims,
                      ChannelDimsFromCalib dimsFromCalib,
                      bool keepIntegralSize,
                      vecD& diffCoeffs,
                      uint* newStartIndex,
                      uint *newEndIndex,
                      bool displayZeroMessages,
                      const bool writeCSV,
                      const bool writeGnuplot,
                      const DeconWriteFreq writeFreq,
                      uint normMode=0,
                      QVector<double> * plotTData=nullptr,
                      QVecMatrix<double>* plotRefData=nullptr,
                      QVecMatrix<double>* plotCorrData=nullptr,
                      omp_QVector<bool> const* plotDataToUpdate=nullptr,
                      uint* actIteration = nullptr,
                      std::vector<uint>* actIterations_par = nullptr,
                      //const bool trackRMS,
                      const bool checkBackConv = false,
                      std::vector<std::vector<lDouble> > *rmsTracker = nullptr
         );
#endif
   /*!
   * \brief getResults returns a pointer to the processed data on heap =>
   *        vector has to be destroyed later manually if not needed anymore
   * \return pointer to the processed data on heap
   */
   matD getResults();

   //bool lock_CalcThread(uint threadID);
   //bool unlock_Thread(uint threadID);



private:

#if QWT_VERSION == 0x060102
   int iterativeRelaxation(double r0,
                           uint itMin,
                           uint itMax,
                           double maxRMS,
                           bool keepIntegralSize,
                           uint normMode = 0,
                           uint* actIteration = nullptr
         );
#endif

   int richardsonLucy(const uint itMax,
                      uint *actIteration,
                      bool checkBackConv,
                      std::vector<std::vector<lDouble> > *trackBackConv
                      );

   int richardsonLucy_par(uint itMax,
                          std::vector<uint> *iterations,
                          bool checkBackConv,
                          std::vector<std::vector<lDouble> > *trackBackConv,
                          QVecMatrix<double> *const plotCorrYData
                          );

   int richardsonLucy_parLogPlot(uint itMax,
                                 std::vector<uint> *iterations,
                                 bool checkBackConv,
                                 std::vector<std::vector<lDouble> > *trackBackConv,
                                 QVecMatrix<double> *const plotCorrYData,
                                 const bool writeCSV,
                                 const bool useGnuPlot,
                                 const DeconWriteFreq writeFreq
                                 );


   vecD backConvolutionRL(const vecD &vecW,
                          const matD &broadMatrix,
                          const double deltaT);

   lDouble sqDiff(const vecD &vec1,
                  const vecD &vec2);


   omp_QVector<bool> signalsToUpdate;

   bool displayZeroMessages;
   vecD workTimes;
   matD workData;
   vecD workDCoeffs;
   matD dataCorr;
   vecD rMeas;
   vecD lambda;
   vecD chi;
   vecD sigmaSq;
   matD GWFolded;

   uint finalLength;
   double newRMS;
   bool calculationHasRun = false;
   bool resultHasBeenPassed = false;

   //////////////////////////////
   // preparative calculations //
   //////////////////////////////

   /*!
   * \brief calcRMeas calculates the rMeas parameter vector
   * \param voidPeak time of the voidpeak [min]
   * \param startIndex new startIndex if adaption is necessary
   * \return bool if function finished successfully
   */
   bool calcRMeas(double voidPeak, uint *startIndex, int *errorCode = nullptr);

   /*!
   * \brief calcLambda calculates the lambda parameter vector
   * \param voidPeak time of the voidpeak [min]
   * \param elutionFlow integrated flow speed [ml/min] of the elution direction
   * \param crossFlow speed of the crossflow [ml/min]
   * \param chWidth channel Width [cm]
   * \param startIndex new startIndex if adaption is necessary
   * \return bool if function finished successfully
   */
   bool calcLambda(//double voidPeakTime,
                   //double elutionFlow,
                   double crossFlow,
                   double chWidth,
                   double chVolume,
                   //double relFocusPoint,
                   //double chLength,
                   uint* startIndex,
                   int *errorCode = nullptr);

   /*!
   * \brief calcChi calculates the chi parameter vector
   * \param startIndex new startIndex if adaption is necessary
   * \return bool if function finished successfully
   */
   bool calcChi(uint *startIndex,int *errorCode = nullptr);

   /*!
   * \brief sigmaSq calculates the sigmaSq parameter vector (=> sigma of the Gaussian)
   * \param startIndex new startIndex if adaption is necessary
   * \param elutionFlow integrated flow speed [ml/min] of the elution direction
   * \param chWidth channel Width [cm]
   * \param chLength channel Length [cm]
   * \return bool if function finished successfully
   */
   bool calcSigmaSq(//double voidPeakTime,
                    double elutionFlow,
                    double crossFlow,
                    double chWidth,
                    double chVolume,
                    double chLength,
                    double b0,
                    double bL,
                    double relFocusPoint,
                    uint *startIndex,
                    int *errorCode = nullptr);

   /*!
   * \brief calcGWFolded calculates the convolution of the current
   *        G (Gaussian, not explicitly calculated) and W (=dataCorr)
   * \param normMode indicates the index of the normalization method
   * \return bool if function finished successfully
   */
   bool calcGWFolded(uint normMode);

   bool isContainedinList(uint i, QList<uint> *l, uint *pos = nullptr);

   inline uint getItsPerParPhase(DeconWriteFreq writeFreq){
      if(writeFreq == DeconWriteFreq::all)             return 1;
      else if(writeFreq == DeconWriteFreq::each5th)    return 5;
      else if(writeFreq == DeconWriteFreq::each10th)   return 10;
      else if(writeFreq == DeconWriteFreq::each50th)   return 50;
      else if(writeFreq == DeconWriteFreq::each100th)  return 100;
      else if(writeFreq == DeconWriteFreq::each500th)  return 500;
      else if(writeFreq == DeconWriteFreq::each1000th) return 1000;
   }

   matD calcRichLucyBroadMatrix(const vecD &workTimes, const vecD &sigmaSq);

   ///////////////////////////
   // Normalization Methods //
   ///////////////////////////

   /*!
   * \brief offsetNormalize overloaded branching function that calls a
   *  OffsetNormalize<x> function depending on the mode flag <x>
   * \param dataToNormalize data matrix to be processed
   *       (normalization will be conducted for each column seperately)
   * \param mode indicates the mode <x> for offsetNormalization

   * \return bool if function finished successfully
   */
   bool offsetNormalize(matD& dataToNormalize, uint mode = 0);

   /*!
   * \brief offsetNormalize overloaded branching function that calls a
   *  OffsetNormalize<x> function depending on the mode flag <x>
   * \param dataToNormalize data vector to be processed
   * \param mode indicates the mode <x> for offsetNormalization
   * \return bool if function finished successfully
   */
   bool offsetNormalize(vecD& dataToNormalize, uint mode = 0);

   /*!
   * \brief normalize overloaded branching function that calls a
   *  normalize<x> function depending on the mode flag <x>
   * \param dataToNormalize data matrix to be processed
   *       (normalization will be conducted for each column seperately)
   * \param mode indicates the mode <x> for normalization

   * \return bool if function finished successfully
   */
   bool normalize(matD& dataToNormalize, uint mode = 0);

   /*!
   * \brief normalize overloaded branching function that calls a
   *  normalize<x> function depending on the mode flag <x>
   * \param dataToNormalize data vector to be processed
   * \param mode indicates the mode <x> for normalization

   * \return bool if function finished successfully
   */
   bool normalize(vecD& dataToNormalize, uint mode = 0);

   /*!
   * \brief offsetNormalizeMode0 standard offsetNormalization method
   * \param dataToNormalize matrix to be processed
   *       (normalization will be conducted for each column seperately)

   * \return bool if function finished successfully
   */
   bool offsetNormalizeMode0(matD& dataToNormalize);

   /*!
   * \brief offsetNormalizeMode0 standard offsetNormalization method
   * \param dataToNormalize data vector to be processed
   * \return bool if function finished successfully
   */
   bool offsetNormalizeMode0(vecD& dataToNormalize);

   /*!
   * \brief normalizeMode0 standard normalization method
   * \param dataToNormalize matrix to be processed
   *       (normalization will be conducted for each column seperately)

   * \return bool if function finished successfully
   */
   bool normalizeMode0(matD& dataToNormalize);

   /*!
   * \brief normalizeMode0 standard normalization method
   * \param dataToNormalize data vector to be processed
   * \return bool if function finished successfully
   */
   bool normalizeMode0(vecD& dataToNormalize);

   ///////////////////////
   // utility functions //
   ///////////////////////

   /*!
   * \brief isZero checks if a double is 0.0 (used to prevent dividing by zero)
   * \param x double to be checked
   * \param silent if stated as true, the method won't log a message if a value becomes zero
   * \return bool if the the double was 0.0
   */
   virtual bool isZero(double x) const;

   /*!
   * \brief minElementIndex returns the index of the smallest element on a vector
   * \param searchData vector to be searched on
   * \param endIndex right border of the search window
   * \param minElementValue value of the smallest element on the vector
   * \return element index
   */
   uint minElementIndex(const vecD &searchData,
                        uint endIndex, double* minElementValue = nullptr) const;

   /*!
   * \brief minElementIndex returns the index of the biggest element on a vector
   * \param searchData vector to be searched on
   * \param endIndex right border of the search window
   * \param maxElementValue value of the biggest element on the vector
   * \return element index   */
   uint maxElementIndex(const vecD& searchData,
                        uint endIndex, double* maxElementValue = nullptr) const;

   void offsetCorrection(matD&v);

   double matrixMinimum(matD &v);

   inline void correctToPositive(double &value){
      if(value <= 0.0) value = 0.0;
   }

   void correctVectorToPositive (vecD &mat);
   void correctMatrixToPositive (matD &mat);

   void constBaseLineCorrection(matD &workData);
   void constBaseLineCorrection_par(matD &workData);

   void linearBaseLineCorrection(matD &workData,
                           double leftAutoBaseBorder,
                           double rightAutoBaseBorder);
   void linearBaseLineCorrection_par(matD &workData,
                               double leftAutoBaseBorder,
                               double rightAutoBaseBorder);

   void writeIntermediateCSV();
   void makeIntermediateGnuplot();

signals:
   void logTextToMainThread(QString message);
   void logWarningToMainThread(QString message);
   void logErrorToMainThread(QString message);\

#if QWT_VERSION == 0x060102
   void initPlot_C();
   void updatePlot();
#endif

public slots:
#if QWT_VERSION == 0x060102
   void setSignalsToUpdate(const omp_QVector<bool>& signalsToUpdate){ this->signalsToUpdate = signalsToUpdate;
                                                                    //  qDebug() << "triggered signalsToUpdate";
                                                                    }
   void emitUpdatePlot();
#endif
};

#endif // FFFDIFFCORRECTOR_H
