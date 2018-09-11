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
#include <QLineEdit>
#include "./af4log.h"

/*! **********************************************************************************************
***
***  \class     AF4FileInOutWidget "src/Gui/Calibration/af4diffevaluationwidget.h"
***  \brief     AF4FileInOutWidget
***  \author    Benedikt Häusele
***  \version   1.0
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
   explicit AF4FileInOutWidget(const QString &identifier, const QString &suffix, QWidget *parent = nullptr);
   ~AF4FileInOutWidget();

    AF4FileInOutWidget(const AF4FileInOutWidget& src) = delete;
    AF4FileInOutWidget& operator= (AF4FileInOutWidget& src) = delete;
    AF4FileInOutWidget(AF4FileInOutWidget&& src) = delete;
    AF4FileInOutWidget& operator= (AF4FileInOutWidget&& src) = delete;

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
   * \param quoted
   * \return
   */
   bool setInputFilePath(QString path, bool quoted = true);

   /*!
    * \brief setOutputFilePath
    * \param path
    * \param quoted
    */
   void setOutputFilePath(QString path, bool quoted);

   /*!
    * \brief writeSettings
    */
   void writeSettings();

private:

   QString identifier;
   QString suffix;

   QPushButton *nameGenButton;
   QCheckBox   *autoGenName;


   QGridLayout *widgetLayout;


   QGridLayout *fileLayout;
   QToolButton *inputFileChooser;
   QLineEdit *inputFileName;
   QToolButton *outputFileChooser;
   QLineEdit *outputFileName;

   /*!
    * \brief chopStringsQuotMarksToOne if a string has more than one Quotation mark, they are merged
    * \example """this string"""   =>    "this string"
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksToOne(QString &string) const;

   /*!
    * \brief chopStringsQuotMarksEntirely removes quotation marks of strings
    * \example """this string"""   =>    this string
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksEntirely(QString &string) const;

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
   void autoGenOutputName();

};

#endif // AF4FILEINOUTWIDGET_H
