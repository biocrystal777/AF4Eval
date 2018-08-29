#ifndef FFFDIFFCORRECTIONTHREAD_H
#define FFFDIFFCORRECTIONTHREAD_H

#include <vector>
#include <string>
#include <thread>
#include <QThread>
#include "./fffdiffcorrector.h"
#include "./fffdiffcorrector.h"
#include "./fffcsvwriter.h"

#include "./qwt_global.h"
//#if QWT_VERSION == 0x060102
//#endif

/*!
 * \brief The FFFDiffCorrectionThread class represents a seperate thread for calculating the
 *        diffusion correction
 * \author Benedikt Haeusele, Emre Brookes
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFDiffCorrectionThread : public QObject {

   Q_OBJECT

public:

   explicit FFFDiffCorrectionThread(QObject *parent = nullptr) : QObject(parent){}

   FFFDiffCorrectionThread(const FFFDiffCorrectionThread& src) = delete;
   FFFDiffCorrectionThread& operator= (FFFDiffCorrectionThread& src) = delete;
   FFFDiffCorrectionThread(FFFDiffCorrectionThread&& src) = delete;
   FFFDiffCorrectionThread& operator= (FFFDiffCorrectionThread&& src) = delete;

   ~FFFDiffCorrectionThread(){}

   /*!
    * \brief setValues sets values that are necessary for the calculation of the diffusion correction
    *        this function has to be called before a calculation becomes possible
    * \param inputFileName
    * \param outputFileName
    * \param adfFileName
    * \param writeAdfFile
    * \param leftOffsetTimeVal
    * \param voidPeakTimeVal
    * \param elutionFlowVal
    * \param crossFlowVal
    * \param chWidthVal
    * \param chLengthVal
    * \param r0Val
    * \param startTimeVal left border of the deconvolution window
    * \param endTimeVal right border of the deconvolution window
    * \param relFocusPoint
    * \param maxRMSVal
    * \param itMin minimum of iterations to be conducted
    * \param itMax maximum of iterations to be conducted
    * \param keepIntegralSize flag that controls the function for resizing the
    *        integral via linear scaling
    * \param progress indicates the progress of the calculation in "iterations"
    * \param usedList indicates which of the data columns have to be processed
    * \param dataToProcess data that shall be processed by the diffusion correction
    * \param inFileData data directly read from the file
    * \param inFileHeadLines headLines of the parsed csv-file
    */
//#if QWT_VERSION == 0x060102
   void setValues(const QString &inputFileName,
                  const QString &outputFileName,
                  const FFFDiffCorrParameters &diffCorrParameters,
                  const ChannelDims &channelP,
                  const ChannelDimsFromCalib &dimsFromCalib,
                  uint timeIndex,
                  uint dCIndex,
                  uint rSIndex,
                  bool rSIndexSet,
                  const QList<uint> &signalIndices,
                  bool keepIntegralSize,
                  uint *progress,
                  std::vector<uint> *progress_par,
                  const bool checkReConv,
                  const matD &inFileData,
                  const QList<bool>& usedList,
                  const std::vector<std::string> &inFileHeadLines,
                  const bool writeCSV,
                  const bool writeGnuplot,
                  const DeconWriteFreq writeFreq,
                  bool displayCalculationWarnings,
                  QVector<double> *const plotTData,
                  QVecMatrix<double> *const plotRefData,
                  QVecMatrix<double> *const plotCorrData,
                  omp_QVector<bool> const*const plotDataToUpdate
                  ){
      this->inputFileName = std::move(inputFileName);
      this->outputFileName = std::move(outputFileName);
      this->diffCorrParameters = std::move(diffCorrParameters);
      this->dims = channelP,
      this->timeIndex = timeIndex;
      this->dCIndex = dCIndex;
      this->rSIndex = rSIndex;
      this->rSIndexSet = rSIndexSet;
      this->signalIndices = signalIndices;
      this->keepIntegralSize = keepIntegralSize;
      this->progress = progress;
      this->progress_par = progress_par;
      this->checkReConv = checkReConv;
      this->inFileData = std::move(inFileData);
      this->usedList = usedList;
      this->inFileHeadLines = inFileHeadLines;
      this->writeCSV = writeCSV;
      this->writeGnuplot = writeGnuplot;
      this->writeFreq = writeFreq;
      this->displayCalculationWarnings = displayCalculationWarnings;
      //plotDataToUpdate
      this->plotTData = plotTData;
      this->plotRefData = plotRefData;
      this->plotCorrData = plotCorrData;
      this->plotDataToUpdate = plotDataToUpdate;
      valuesHaveBeenSet = true;
   }
//#endif




private:

   bool valuesHaveBeenSet;
   QString inputFileName;
   QString outputFileName;
   FFFDiffCorrParameters diffCorrParameters;
   ChannelDims dims;
   ChannelDimsFromCalib dimsFromCalib;
   uint timeIndex;
   uint dCIndex;
   uint rSIndex;
   bool rSIndexSet;
   QList<uint> signalIndices;
   bool keepIntegralSize;
   uint *progress;
   std::vector<uint> *progress_par;

   bool checkReConv;

   QList<bool> usedList;

   matD inFileData;
   bool writeCSV;
   bool writeGnuplot;
   DeconWriteFreq writeFreq;
   std::vector<std::string> inFileHeadLines;



//#if QWT_VERSION == 0x060102
   QVector<double> * plotTData;
   QVecMatrix<double>* plotRefData;
   QVecMatrix<double>* plotCorrData;
   omp_QVector<bool> const* plotDataToUpdate;
//#endif


   bool displayCalculationWarnings;

signals:

   /*!
    * \brief logText logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   void logText(QString message);

   /*!
    * \brief logWarning logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   void logWarning(QString message);

   /*!
    * \brief logError logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   void logError(QString message);

//#if QWT_VERSION == 0x060102
   void updatePlot();
   void initPlot();
//#endif






   void finished();
   void signalsToUpdateChanged(const omp_QVector<bool> &s);

public slots:
   void runThread();

   void emitSignalsToUpdateChanged(const omp_QVector<bool> &s);

protected slots:

//#if QWT_VERSION == 0x060102
   void emitInitPlot();
   void emitUpdatePlot();
//#endif


   /*!
    * \brief logText logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   void logTextSlot(QString message);

   /*!
    * \brief logWarning logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   void logWarningSlot(QString message);

   /*!
    * \brief logError logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   void logErrorSlot(QString message);

};
#endif // FFFDIFFCORRECTIONTHREAD_H
