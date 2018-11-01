#include "af4log.h"

static AF4Log * global_LogPtr = nullptr;

//-/////////////////////////////
// public stuff
//-/////////////////////////////

AF4Log* AF4Log::createSingleInstance(QWidget *parent)
{
   if(!global_LogPtr){
      global_LogPtr = new AF4Log(parent);
      return global_LogPtr;
   }
   else
      return nullptr;
}

void AF4Log::logText(std::string logMessage, bool qDebugToo )
{
   if(global_LogPtr){
      global_LogPtr->logTextInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << logMessage.c_str();
   }
}

void AF4Log::logWarning(std::string logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logWarningInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << "WARNING:" << logMessage.c_str();
   }
}

void AF4Log::logError(std::string logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logErrorInInstance(QString(logMessage.c_str()));
      if(qDebugToo) qDebug() << "ERROR:" << logMessage.c_str();
   }
}

void AF4Log::logText(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logTextInInstance(logMessage);
      if(qDebugToo) qDebug() << logMessage;
   }
}

void AF4Log::logWarning(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logWarningInInstance(logMessage);
      if(qDebugToo) qDebug() << "WARNING:" << logMessage;
   }
}


void AF4Log::logError(QString logMessage, bool qDebugToo)
{
   if(global_LogPtr){
      global_LogPtr->logErrorInInstance(logMessage);
      if(qDebugToo) qDebug() << "ERROR:" << logMessage;
   }
}

void AF4Log::clearLog()
{
   if(global_LogPtr) global_LogPtr->clearLogInInstance();
}

//-/////////////////////////////
// Private stuff
//-/////////////////////////////

AF4Log::AF4Log(QWidget *parent) :
   QWidget(parent)
{
   layout =      new QGridLayout(this);
   logWidget =   new QTextEdit(this);
   logWidget->setReadOnly(true);
   layout->addWidget(logWidget, 0, 0, 5, 10);
   clearButton = new QPushButton("Clear", this);
   QObject::connect(clearButton, &QPushButton::pressed, this, &AF4Log::clearLog) ;
   layout->addWidget(clearButton, 2, 10, 1,1);
}

void AF4Log::logTextInInstance(QString logMessage) const
{
   logWidget->append(tr("<b>%1\n").arg(logMessage));   
   logWidget->verticalScrollBar()->setValue(logWidget->verticalScrollBar()->maximum());
}

void AF4Log::logWarningInInstance(QString logMessage) const
{
   logWidget->append(tr("<font color=\"#DAA520\"><b>WARNING: %1\n</b></font>").arg(logMessage));
   logWidget->verticalScrollBar()->setValue(logWidget->verticalScrollBar()->maximum());
}

void AF4Log::logErrorInInstance(QString logMessage) const
{
   logWidget->append(tr("<font color=\"#CC0033\"><b>ERROR: %1\n</b></font>").arg(logMessage));
   logWidget->verticalScrollBar()->setValue(logWidget->verticalScrollBar()->maximum());
}

void AF4Log::clearLogInInstance()
{
   logWidget->clear();
}
