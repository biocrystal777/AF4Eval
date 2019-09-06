#ifndef AF4FILEINOUTWIDGET_H
#define AF4FILEINOUTWIDGET_H

#include <QFile>
#include <QPushButton>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QToolButton>
#include "../Core/af4evalmacros.h"
#include "af4filenamelineedit.h"
#include "./af4log.h"

/*! **********************************************************************************************
***
***  \class     AF4FileInOutWidget "src/Gui/Calibration/af4diffevaluationwidget.h"
***  \brief     AF4FileInOutWidget
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4FileInOutWidget : public QFrame
{
   Q_OBJECT
public:

   /*!
    * \brief AF4FileInOutWidget
    * \param identifier
    * \param suffix
    * \param parent
    */
   AF4FileInOutWidget(const QString &identifier, const QString &suffix, QWidget *parent);

   /*!
     */
   ~AF4FileInOutWidget();

    /*!
    * \brief getOutputFilePath
    * \param quoted
    * \return
    */
   QString getOutputFilePath(bool quoted = false);
   /*!
   * \brief getInputFilePath
   * \param quoted
   * \return
   */
   QString getInputFilePath(bool quoted = false);

   /*!
   * \brief setInputFilePath
   * \param path
   * \param quoted
   * \param silent
   * \return
   */
   bool setInputFilePath(QString path, bool quoted, bool silent = false);

   /*!
    * \brief setOutputFilePath
    * \param path
    * \param quoted
    * \param silent
    */
   void setOutputFilePath(QString path, bool quoted, bool silent = false);

   /*!
    * \brief writeSettings
    *
    */
   void writeSettings();


private slots:
   /*!
    * \brief chooseInputFile opens a dialog to choose an input data file
    *        (csv-formatted)
    */
   void chooseInputFile();

   /*!
    * \brief chooseOutputFile opens a dialog to choose a folder and filename
    *                         for the output file
    */
   void chooseOutputFile();

   /*!
    * \brief adoptOutputName
    */
   void adoptOutputName();

   /*!
    * \brief autoGenOutputName
    */
   void generateOutputName();

private:

   const QString identifier;
   const QString suffix;

   QPushButton         *nameGenButton     = nullptr;
   QCheckBox           *autoGenName       = nullptr;
   QGridLayout         *widgetLayout      = nullptr;
   QGridLayout         *fileLayout        = nullptr;
   QToolButton         *inputFileChooser  = nullptr;
   AF4FilenameLineEdit *inputFileName     = nullptr;
   QToolButton         *outputFileChooser = nullptr;
   AF4FilenameLineEdit *outputFileName    = nullptr;

   NO_COPY_MOVE_CTORS(AF4FileInOutWidget)
};

#endif // AF4FILEINOUTWIDGET_H
