#include "af4csvparser.h"
#include <locale>

using std::string;
using std::wstring;
using std::stringstream;
using std::ifstream;
using std::vector;

AF4CsvParser::AF4CsvParser(const string& filePath, const char sep, const char dec) :
   filePath(filePath), sep(sep), dec(dec)
{
}

int AF4CsvParser::parseFile(uint *errorInLine)
{
   headLineEntries.clear();
   dataVectorList.clear();
   ifstream inpFile;
   inpFile.open(filePath.c_str());
   if(!inpFile.is_open()){
      FFFLog::logError(string("File could not be opened."), true);
      return 1;
   }

   string firstLine;
   std::getline(inpFile, firstLine);
   inpFile.seekg(0);   

   if(firstLine.size() == 0 ){
      return 5;
   }

   return parseFileUTF8(inpFile);

}

int AF4CsvParser::parseFileUTF8( ifstream &inpFile)
{
   string dataLine;
   std::getline(inpFile, dataLine);
   dataLine.erase(std::remove(dataLine.begin(), dataLine.end(), '\n'), dataLine.end());
   dataLine.erase(std::remove(dataLine.begin(), dataLine.end(), '\r'), dataLine.end());
   headLineEntries = split(dataLine, sep); // first line = header



   dataVectorList.resize(headLineEntries.size());

   while(std::getline(inpFile, dataLine))
   {
      vector<string> dataSplit = split(dataLine, sep);
      for(uint i = 0; i < dataSplit.size(); ++i){
         double d = std::strtod(dataSplit[i].c_str(), nullptr);
         dataVectorList[i].push_back(d);
      }
   }

   return 0;
}

vector<string> AF4CsvParser::getHeadLines() const
{
   return headLineEntries;
}

matD AF4CsvParser::getData() const
{
   return dataVectorList;
}

bool AF4CsvParser::dataIsComplete(matD &data)
{
   if (data.size() <= 1) return false;
   const uint length = data[0].size();
   for(uint i = 1; i < data.size(); ++i)
      if (data[i].size() != length) return false;
   return true;
}

vector<string> AF4CsvParser::split(const string& s,
                                   const char delim,
                                   const bool keepEmpty) const {
   vector<string> result;
   string::const_iterator substart = s.begin();
   string::const_iterator subend = substart;
   while (subend != s.end()) {
      subend = findCharPos(substart, s.end(), delim);
      string subElem(substart, subend);
      if (keepEmpty || !subElem.empty())
         result.push_back(std::move(subElem));
      substart = subend + 1;
   }
   return result;
}

auto AF4CsvParser::findCharPos(std::string::const_iterator start,
                               std::string::const_iterator end,
                               const char c) const -> std::string::const_iterator
{
   do {
      if(c == *start) return start;
      ++start;
   } while(start != end);
   return end;
}
