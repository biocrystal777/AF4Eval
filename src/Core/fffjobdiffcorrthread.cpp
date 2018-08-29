#include "fffjobdiffcorrthread.h"

using std::string;
using std::vector;

FFFJobDiffCorrThread::FFFJobDiffCorrThread(int threadId, QObject *parent) :
   QObject(parent),
   threadId(threadId)
{
   QObject::connect(this, SIGNAL(finished()), this, SLOT(emitIdFinished()));
}

FFFJobDiffCorrThread::~FFFJobDiffCorrThread()
{
}

void FFFJobDiffCorrThread::setValues(
      vecD* times,
      vecD* diffCoeffs,
      vectorMatrix* data,
      const ExpFFFParameters &e,
      const DiffCorrParameters &d,
      int *errorCode,
      std::vector<vectorMatrix*> *snapShotResults,
      int algorithm
      )
{
   this->times = times;
   this->diffCoeffs = diffCoeffs;
   this->data = data;
   this->expFFFParameters = e;
   this->diffCorrParameters = d;
   this->errorCode = errorCode;
   this->snapShotResults = snapShotResults;
   this->algorithm = algorithm;
}


void FFFJobDiffCorrThread::runThread()
{
   emit finished();
   return;
}

void FFFJobDiffCorrThread::emitIdFinished()
{
   emit finished(threadId);
}
