#ifndef FFFLOGWIDGET_H
#define FFFLOGWIDGET_H

#include <QTextEdit>
#include <QScrollBar>
#include <QGridLayout>
#include <QDebug>
#include <QPushButton>
#include <QFileDialog>
#include <string>
#include "../Core/af4evalmacros.h"

/*! **********************************************************************************************
***
***  \class   AF4Log "src/Gui/Calibration/af4log.h"
***  \brief   Logging widget
***  \details The class is implemented as singleton and public static functions can be
***           accessed. All functions can be called globally.
***
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4Log final: public QWidget
{
   Q_OBJECT
public:

   static AF4Log* createSingleInstance(QWidget *parent);

   /*!
     * \brief logText global overloaded logging function, calls the
     *        internal logging function
     * \param logMessage message to be logged
     */
   static void logText(std::string logMessage, bool qDebugToo = false);

   /*!
      * \brief logWarning global overloaded logging function, calls the
      *        internal logging function
      * \param logMessage message to be logged
      */
   static void logWarning(std::string logMessage, bool qDebugToo = false);

   /*!
      * \brief logError global overloaded logging function, calls the
      *        internal logging function
      * \param logMessage message to be logged
      */
   static void logError(std::string logMessage, bool qDebugToo = false);

   /*!
      * \brief logText global overloaded logging function, calls the
      *        internal logging function
      * \param logMessage message to be logged
      */
   static void logText(QString logMessage, bool qDebugToo = false);

   /*!
      * \brief logWarning global overloaded logging function, calls the
      *        internal logging function
      * \param logMessage message to be logged
      */
   static void logWarning(QString logMessage, bool qDebugToo = false);

   /*!
      * \brief logError global overloaded logging function, calls the
      *        internal logging function
      * \param logMessage message to be logged
      */
   static void logError(QString logMessage, bool qDebugToo = false);

private slots:
   /*!
     * \brief clearLog clears the logWidget from existing text, calls the
     *                 internal  function
     */
   static void clearLog();

private:


   /*!
     * \brief AF4Log the constructor of this class
     * \param parent parent widget
     */
   explicit AF4Log(QWidget *parent = nullptr);

   QGridLayout *layout       = nullptr;
   QTextEdit   *logWidget    = nullptr;
   QPushButton *clearButton  = nullptr;

   /*!
     * \brief logTextInInstance class internal logging function
     * \param logMessage message to be logged
     */
   void logTextInInstance(QString logMessage) const;

   /*!
     * \brief logWarningInInstance class internal logging function
     * \param logMessage message to be logged
     */
   void logWarningInInstance(QString logMessage) const;

   /*!
     * \brief logWarningInInstance class internal logging function
     * \param logMessage message to be logged
     */
   void logErrorInInstance(QString logMessage) const;

   /*!
     * \brief logWarningInInstance class internal logging function
     * \param logMessage message to be logged
     */
   void clearLogInInstance();

  /*!
   * \brief destructor of this class
   */
  ~AF4Log(){}

   NO_COPY_ASSIGNMENT_CTORS(AF4Log)
};

#endif // FFFLOGWIDGET_H
