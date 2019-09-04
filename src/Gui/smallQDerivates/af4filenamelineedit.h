#ifndef AF4FILENAMELINEEDIT_H
#define AF4FILENAMELINEEDIT_H

#include <QFile>
#include <QLineEdit>
#include "../GeneralWidgets/af4log.h"
#include "../Core/af4evalmacros.h"

class AF4FilenameLineEdit : public QLineEdit
{
   Q_OBJECT
public:
   AF4FilenameLineEdit(QWidget *parent = nullptr) : QLineEdit(parent){}
   ~AF4FilenameLineEdit(){}

   /*!
    * \brief chopStringsQuotMarksToOne if a string has more than one Quotation mark, they are merged
    * \example """this string"""   =>    "this string"
    * \param string to be processed
    * \return processed string
    */
   QString singleQuotMarks(const QString &string) const;

   /*!
    * \brief chopStringsQuotMarksEntirely removes quotation marks of strings
    * \example """this string"""   =>    this string
    * \param string to be processed
    * \return processed string
    */
   QString noQuotMarks(const QString &string) const;

public slots:

   QString singleQuotMarkText() const { return singleQuotMarks(text()); }
   QString noQuotMarkText() const { return noQuotMarks(text()); }

   bool setSingleQuotMarkText(const QString &string, bool checkExistency = false);
   bool setNoQuotMarkText(const QString &string, bool checkExistency = false);

private:

   bool fileExists(const QString &path);
   NO_COPY_ASSIGNMENT_CTORS(AF4FilenameLineEdit)
};

#endif // AF4FILENAMELINEEDIT_H
