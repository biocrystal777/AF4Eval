#include "af4csvwriter.h"

using std::string;
using std::stringstream;
using std::ofstream;
using std::vector;
using std::setprecision;
using std::ios_base;


bool AF4CsvWriter::writeFile(const matD& data, const vector<string>& headLineEntries)
{
   // check inputs
   if(data.size() != headLineEntries.size()){
      return false;
   }

   ofstream outFile;
   outFile.open(filePath.c_str());
   if(!outFile.is_open()){
      return false;
   }

   // write headLine;

   uint headSize = headLineEntries.size();

   for(uint i=0; i < headSize - 1; i++){
      outFile << headLineEntries.at(i) << sep;
   }
   outFile << headLineEntries.at(headSize-1) << lineSep;

   // find longest column;

   uint dataSize = data.size();
   vector<uint> dataVectorSizes;
   for (uint j = 0; j < dataSize; j++)
      dataVectorSizes.push_back(data.at(j).size());

   uint maxSize = 0;
   maxElementIndex(dataVectorSizes, 0, dataVectorSizes.size(), &maxSize);
   for (uint i=0; i < maxSize; i++){
      for(uint j=0; j < dataSize - 1; j++){
         if (i < dataVectorSizes.at(j))
            outFile << data.at(j).at(i);
         if (i < dataVectorSizes.at(j+1))
            outFile << ",";
      }
      if (i < dataVectorSizes.at(dataSize-1))
         outFile << setiosflags(ios_base::scientific) << setprecision(15) << data.at(dataSize-1).at(i);
      outFile << lineSep;
   }
   outFile.close();
   return true;
}

uint AF4CsvWriter::maxElementIndex(const vector<uint>& searchData, uint startIndex, uint EndIndex, uint *maxElementValue) const
{
   uint resultingIndex = startIndex;
   uint elementValue = searchData.at(startIndex);
   for (uint i = startIndex; i < EndIndex; i++){
      if(elementValue > searchData.at(i)){
         resultingIndex = i;
         elementValue = searchData.at(i);
      }
   }
   if(maxElementValue) *maxElementValue = elementValue;
   return resultingIndex;
}

