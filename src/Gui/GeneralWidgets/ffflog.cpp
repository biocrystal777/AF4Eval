#include "ffflog.h"
FFFLog *global_LogPtr = 0;

FFFLog::FFFLog(QWidget *parent) :
   QWidget(parent)
{
   logWidget = new QTextEdit(this);
   layout = new QGridLayout(this);
   layout->addWidget(logWidget);
   logWidget->setReadOnly(true);
   if(!global_LogPtr)
      global_LogPtr = this;
}

FFFLog::~FFFLog()
{
}

void FFFLog::logText(std::string logMessage, bool qDebugToo )
{

   if(global_LogPtr){
      global_LogPtr->logTextInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << logMessage.c_str();
   }
}

void FFFLog::logWarning(std::string logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logWarningInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << "WARNING:" << logMessage.c_str();
   }
}

void FFFLog::logError(std::string logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logErrorInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << "ERROR:" << logMessage.c_str();
   }
}

void FFFLog::logText(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logTextInInstance(logMessage);
      if(qDebugToo) qDebug() << logMessage;
   }
}

void FFFLog::logWarning(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logWarningInInstance(logMessage);
      if(qDebugToo) qDebug() << "WARNING:" << logMessage;
   }
}


void FFFLog::logError(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logErrorInInstance(logMessage);
      if(qDebugToo) qDebug() << "ERROR:" << logMessage;
   }
}

void FFFLog::clearLog()
{
   if(global_LogPtr){
      global_LogPtr->clearLogInInstance();
   }
}

void FFFLog::logTextInInstance(QString logMessage) const
{
   logWidget->append(tr("<b>%1\n").arg(logMessage));
}

void FFFLog::logWarningInInstance(QString logMessage) const
{
   logWidget->append(tr("<font color=\"#DAA520\"><b>WARNING: %1\n</b></font>").arg(logMessage));
}

void FFFLog::logErrorInInstance(QString logMessage) const
{
   logWidget->append(tr("<font color=\"#CC0033\"><b>ERROR: %1\n</b></font>").arg(logMessage));
}

void FFFLog::clearLogInInstance() const
{
   logWidget->clear();
}
