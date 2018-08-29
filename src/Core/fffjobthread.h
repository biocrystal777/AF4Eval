#ifndef FFFJOBTHREAD_H
#define FFFJOBTHREAD_H

#include <thread>
#include <QThread>
#include <QFile>
#include <QDir>
#include "./fffdiffcorrectionthread.h"
#include "./fffparameterstructs.h"
#include "./fffdiffevaluator.h"
#include "./fffdiffcorrector.h"
#include "./fffslsevaluator.h"
#include "./fffcsvparser.h"
#include "./fffcsvwriter.h"
#include <unistd.h>
//#include "./fffjobdiffcorrthread.h"
//#include "./fffdiffcorrectionthread.h"



/*

                                        /////////////////////////////
                                        ////                     ////
                                        //// Evaluation overview ////
                                        ////                     ////
                                        /////////////////////////////

                                                ===========
                                                || Start ||
                                                ===========
                                                     |
                                                     |
                                                     V
                                       ============================================
                                       | Check and parse SLS RI File    [1]       |
                                       ============================================
                                                     |
                                                     |
                                                     V
                                       ============================================
                                       | Det. of diffusion coefficients [2]       |
                                       ============================================
                                                     |
                                                     | Use Deconvolution ?
                                                     |
                                       +-------------+-------------------+
                                       |                                 |                  It. relax.?      ***************************
                                       | No                              | Yes            +-------+--------->* It. relaxation [4]      *-+
                                       |                                 |                |       |    Yes   *************************** |
                                       | SLS Evaluation?                 |                |    No |                                      |
                                       |                                 V                |       |                                      |
                       +---------------+--------------+       =====================       |       +--------------------------------------+
                       |                              |       | Deconv [3]        |-------+                                              |
                       | Yes                          | No    =====================       |                                              |
                       |                              |                                   |                                              |
                       V                              |                                   |   Rich Lucy?    **********************       |
           =============================              |                                   +-------+---------* Rich Lucy [5]      *       |
           | SLS Evaluation [6]        |              |                                           |   Yes   **********************       |
           |  for original data set    |              |                                        No |                    |                 |
           =============================              |                                           +--------------------+                 |
                       |                              |                                                                |                 |
                       |                              |                                                                +---------+-------+ Both deconvolutions
                       V                              V                                                                          |         finished
           ============================     =========================                                                            |
           | write D, Rs, Rg, Mw, rho |     | Write D, RS           |                                                            | SLS Evaluation?
           ============================     =========================                                                  ----------+
                                                                                                                                 |
                                                                                                                                 V
                                                                                                                   =============================
                                                                                                                   | SLS Evaluation            |
                                                                                                                   |  for original data set    |
                                                                                                                   |  + each deconvoluted set  |
                                                                                                                   =============================

*/

/*!
 * \brief The FFFDiffCorrectionThread class represents a seperate thread for calculating the
 *        diffusion correction
 * \author Benedikt Haeusele, Emre Brookes
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFJobThread : public QObject
{
   Q_OBJECT

public:

   explicit FFFJobThread(int threadId, QObject *parent = nullptr);

   FFFJobThread(const FFFJobThread& src) = delete;
   FFFJobThread& operator= (FFFJobThread& src) = delete;
   FFFJobThread(FFFJobThread&& src) = delete;
   FFFJobThread& operator= (FFFJobThread&& src) = delete;

   ~FFFJobThread();

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

   void setValues(const QString& inputFilePath,
                  const ExpFFFParameters& expFFFParameters,
                  const ChannelDims& channelCalibParameters,
                  const DiffCorrParameters& diffCorrParameters,
                  const SLSParameters& slsParameters,
                  const SLSCalibrationParameters& slsCalibrationParameters,
                  const SampleProperties& sampleProperties
                  );

   /*!
    * \brief emitUpdatePlot provides an entry for a non-QObject class to emit updatePlot
    * \param position the position in the plot data vectors for the data to plot
    */
   //void emitUpdatePlot( int position, int curve );

   //void emitClearPlot();

   //bool isFinished() const {return threadFinished;}

private:

   const int threadId;
   QString inputFileName;
   ExpFFFParameters expFFFParameters;
   ChannelDims channelCalibParameters;
   DiffCorrParameters diffCorrParameters;
   SLSParameters slsParameters;
   SampleProperties sampleProperties;
   SLSCalibrationParameters slsCalibrationParameters;

   // data to be parsed and calculated
   std::vector<std::string> headLines;
   //vectorMatrix* data;
   matD data;
   vecD diffCoeffs;
   vecD stokesRadii;
   volatile bool rlFinished;
   volatile bool irFinished;

   std::vector<vectorMatrix*>* resultsRL;
   std::vector<vectorMatrix*>* resultsIR;

/*
   QThread* irThreadWrapper;
   FFFJobDiffCorrThread *irThread;
   QThread* rlThreadWrapper;
   FFFJobDiffCorrThread *rlThread;

*/
   //bool threadFinished;

   void freeMemory();

   uint getMaximum(const QList<uint> &l) const;


   void evalErrorCodeFromDiffCorr(int errorCode, const QString & algoDescription);
   FFFDiffCorrParameters convertToFFFDiffCorrParameters(const uint algorithm) const;

   bool writeDiffCorrData(const QString &filePrefix,
                          const uint timeIndex,
                          vecD &times,
                          vecD *diffCoeffs,
                          vecD *stokesRadii,
                          uint newStartIndex,
                          uint newEndIndex,
                          DiffCorrParameters diffCorrParameters,
                          std::vector<vectorMatrix*>* results, uint algorithm);


signals:

   void finished();
   void finished(int threadId);


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


   /*!
    * \brief updatePlot signals when a new dataset is ready to plot at position
    * \param position position in plot vectors with data ready to plot
    */
   // void updatePlot(int position, int curve);

   // void clearPlot();

public slots:
   void runThread();


protected slots:


   void emitNumberedFinish();


   void emitLogError(QString message);
   void emitLogText(QString message);
   void emitLogWarning(QString message);

   /*!
    * \brief logText logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   //void logTextSlot(QString message);

   /*!
    * \brief logWarning logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   //void logWarningSlot(QString message);

   /*!
    * \brief logError logging function implemented as signal to log into the main thread
    * \param message logMessage
    */
   // void logErrorSlot(QString message);

};

#endif // FFFJOBTHREAD_H
