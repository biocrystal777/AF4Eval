#include "af4filenamelineedit.h"

bool AF4FilenameLineEdit::setSingleQuotMarkText(const QString &string, bool checkExistency)
{
   if(checkExistency && !fileExists(string))
      return false;
   setText(singleQuotMarks(string));
   return true;
}

bool AF4FilenameLineEdit::setNoQuotMarkText(const QString &string, bool checkExistency)
{
   if(checkExistency && !fileExists(string))
      return false;
   setText(noQuotMarks(string));
   return true;
}

bool AF4FilenameLineEdit::fileExists(const QString &path)
{
   if(QFile::exists(noQuotMarks(path)))
      return true;
   else {
      AF4Log::logWarning(tr("Chosen input file does not exist."));
      return false;
   }

}

QString AF4FilenameLineEdit::singleQuotMarks(const QString &string) const
{
   QString s = noQuotMarks(string);
   s.prepend('\"');
   s.append('\"');
   return string;
}

QString AF4FilenameLineEdit::noQuotMarks(const QString &string) const
{
   const QChar quotMark('\"');
   QString s = string;
   while(!s.isEmpty() && s.at(0) == quotMark)
      s.remove(0, 1);
   while(!s.isEmpty() && s.at(s.length()-1) == quotMark)
      s.remove(s.length()-1, 1);
   return s;
}
