#include "af4filenamelineedit.h"

bool AF4FilenameLineEdit::setSingleQuotMarkText(const QString &filePath, bool checkFileExistency, bool silent)
{
   if(checkFileExistency && !fileExists(filePath, silent))
      return false;
   setText(singleQuotMarks(filePath));
   return true;
}

bool AF4FilenameLineEdit::setNoQuotMarkText(const QString &filePath, bool checkFileExistency, bool silent)
{
   if(checkFileExistency && !fileExists(filePath, silent))
      return false;
   setText(noQuotMarks(filePath));
   return true;
}

bool AF4FilenameLineEdit::fileExists(const QString &path, bool silent)
{
   QString noQuotPath = noQuotMarks(path);
   if(QFile::exists(noQuotPath)) return true;
   if(!silent)
      if (noQuotPath.isEmpty())
         AF4Log::logWarning(tr("No file path."));
      else
         AF4Log::logWarning(tr("%1 does not exist.").arg(singleQuotMarks(noQuotPath)));
   return false;
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
