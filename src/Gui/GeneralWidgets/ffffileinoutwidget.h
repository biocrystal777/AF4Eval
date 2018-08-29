#ifndef FFFFILEINOUTWIDGET_H
#define FFFFILEINOUTWIDGET_H

#include <QFile>
#include <QPushButton>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include "ffflog.h"

class FFFFileInOutWidget : public QFrame
{
   Q_OBJECT
public:
   explicit FFFFileInOutWidget(const QString &identifier, const QString &suffix, QWidget *parent = nullptr);
   ~FFFFileInOutWidget();

    FFFFileInOutWidget(const FFFFileInOutWidget& src) = delete;
    FFFFileInOutWidget& operator= (FFFFileInOutWidget& src) = delete;
    FFFFileInOutWidget(FFFFileInOutWidget&& src) = delete;
    FFFFileInOutWidget& operator= (FFFFileInOutWidget&& src) = delete;


   QString getOutputFilePath(bool quoted = false);
   QString getInputFilePath(bool quoted = false);

   bool setInputFilePath(QString path, bool quoted = true);
   void setOutputFilePath(QString path, bool quoted);

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

   void adoptOutputName();
   void autoGenOutputName();

};

#endif // FFFFILEINOUTWIDGET_H
