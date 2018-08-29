#ifndef FFFLOGWIDGET_H
#define FFFLOGWIDGET_H

#include <QTextEdit>
#include <QGridLayout>
#include <QDebug>
#include <string>

/**************************************************************
***
***  FFFLog (using Qt 4.8.2)
***
**************************************************************/

/*!
 * \brief The FFFLog class allows global logging into a central logWidget
 * \author Benedikt Haeusele
 * \details The class is implemented as singleton. For this reason, all loggging
 * functions are globally accessible.
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFLog: public QWidget
{
   Q_OBJECT
public:

   /*!
     * \brief FFFLog the constructor of this class
     * \param parent parent widget
     */
   explicit FFFLog(QWidget *parent = nullptr);

    FFFLog(const FFFLog& src) = delete;
    FFFLog& operator= (FFFLog& src) = delete;
    FFFLog(FFFLog&& src) = delete;
    FFFLog& operator= (FFFLog&& src) = delete;

   /*!
    * \brief destructor of this
    */
   ~FFFLog();

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

   /*!
     * \brief clearLog clears the logWidget from existing text, calls the
     *        internal logging function
     */
   static void clearLog();

protected:
   QGridLayout *layout;
   QTextEdit *logWidget;

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
   void clearLogInInstance() const;
};

#endif // FFFLOGWIDGET_H
