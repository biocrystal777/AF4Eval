#ifndef FFFJOBDIFFCORRTHREAD_H
#define FFFJOBDIFFCORRTHREAD_H

#include "fffparameterstructs.h"
#include "./fffdiffcorrector.h"
#include <QObject>

class FFFJobDiffCorrThread : public QObject
{
   Q_OBJECT
public:
   explicit FFFJobDiffCorrThread(int threadId, QObject *parent = 0);

   ~FFFJobDiffCorrThread();


   FFFJobDiffCorrThread(const FFFJobDiffCorrThread& src) = delete;
   FFFJobDiffCorrThread& operator= (FFFJobDiffCorrThread& src) = delete;
   FFFJobDiffCorrThread(FFFJobDiffCorrThread&& src) = delete;
   FFFJobDiffCorrThread& operator= (FFFJobDiffCorrThread&& src) = delete;

   void setValues(vecD* times,
                  vecD* diffCoeffs,
                  vectorMatrix* data,
                  const ExpFFFParameters &e,
                  const DiffCorrParameters &d,
                  int *errorCode,
                  std::vector<vectorMatrix*>* snapShotResults,
                  int algorithm
                  );

private:
   const int threadId;

   vecD* times;
   vecD* diffCoeffs;
   vectorMatrix* data;
   ExpFFFParameters expFFFParameters;
   DiffCorrParameters diffCorrParameters;
   int *errorCode;
   std::vector<vectorMatrix*>* snapShotResults;
   int algorithm;

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


public slots:
   void emitIdFinished();
   void runThread();
};

#endif // FFFJOBDIFFCORRTHREAD_H
