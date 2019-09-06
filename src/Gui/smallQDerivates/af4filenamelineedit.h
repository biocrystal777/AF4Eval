#ifndef AF4FILENAMELINEEDIT_H
#define AF4FILENAMELINEEDIT_H

#include <QFile>
#include <QLineEdit>
#include "../GeneralWidgets/af4log.h"
#include "../Core/af4evalmacros.h"

/*! **********************************************************************************************
***
***  \class     AF4FilenameLineEdit "src/Gui/Calibration/af4diffevaluationwidget.h"
***  \brief     AF4FileInOutWidget is a QLineEdit for the display of file names with or without
***             quotation marks.
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2019-09-5
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4FilenameLineEdit : public QLineEdit
{
   Q_OBJECT
public:
   AF4FilenameLineEdit(QWidget *parent = nullptr) : QLineEdit(parent){}
   ~AF4FilenameLineEdit(){}

   /*!
    * \brief singleQuotMarks embraces the given string with a single pair of
    *                        quotation marks
    * \example """this string"""   =>    "this string"
    *            "this string""    =>    "this string"
    * \param string to be processed
    * \return processed string
    */
   QString singleQuotMarks(const QString &string) const;

   /*!
    * \brief noQuotMarks removes quotation marks of the given string
    * \example """this string"""   =>    this string
    * \param string to be processed
    * \return processed string
    */
   QString noQuotMarks(const QString &string) const;

   /*!
    * \brief singleQuotMarkText
    * \return currently displayed text
    */
   QString singleQuotMarkText() const { return singleQuotMarks(text()); }

   /*!
    * \brief noQuotMarkText
    * \return
    */
   QString noQuotMarkText() const { return noQuotMarks(text()); }

public slots:

   /*!
    * \brief setSingleQuotMarkText sets the text of the with one matched pair of
    *                              embracing quotation marks
    * \param filePath
    * \param checkExistency
    * \return The result of the existency test of the given file. If no existency test was required
    *         the result will always be true.
    */
   bool setSingleQuotMarkText(const QString &filePath, bool checkFileExistency = false, bool silent = false);

   /*!
    * \brief setNoQuotMarkText sets the text of the without any embracing quotation marks
    * \param filePath
    * \param checkExistency
    * \return The result of the existency test of the given file. If no existency test was required
    *         the result will always be true.
    */
   bool setNoQuotMarkText(const QString &filePath, bool checkFileExistency = false, bool silent = false);

private:

   /*!
    * \brief fileExists checks if a file with the given path exists.
    *                   and returns a warning message when no file was found.
    *
    * \param path
    * \return result of the existency test
    */
   bool fileExists(const QString &path, bool silent = false);

   NO_COPY_MOVE_CTORS(AF4FilenameLineEdit)
};
#endif // AF4FILENAMELINEEDIT_H
