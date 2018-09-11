#ifndef AF4CSVWRITER_H
#define AF4CSVWRITER_H
#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include "./af4constants.h"
#include "./af4datatypes.h"
#include "../Gui/GeneralWidgets/af4log.h"

/*! ***********************************************************
***
***  \class     AF4CSVWriter "src/Core/af4csvwriter.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/

class AF4CsvWriter final
{
public:

   /*!
    * \brief FFFCsvWriter is the constructor of the class.
    * \param filePath path of the file. A file that already exists will be overwritten
    */
   explicit AF4CsvWriter(const std::string& filePath) :
   filePath(filePath), sep(','), dec('.'), lineSep('\n'){}

   /*!
    * \brief FFFCsvWriter Default destructor
    */
   ~AF4CsvWriter(){}

    AF4CsvWriter(const AF4CsvWriter& src) = delete;
    AF4CsvWriter& operator= (const AF4CsvWriter& src) = delete;
    AF4CsvWriter(AF4CsvWriter&& src) = delete;
    AF4CsvWriter& operator= (AF4CsvWriter&& src) = delete;

   /*!
    * \brief writeFile The function has to be called to write the file to the path stated above
    * \param data dataMatrix to be written. Format has to be data->at(column)->at(row).
    * \param headLineEntries are the entries that will form the headline
    *        headLineEntries-> has to be equal to data->size() !
    * \return bool indicates whether writing the fil was successful or not
    */
   bool writeFile(const matD &data, const std::vector<std::string>& headLineEntries);

   /*!
    * \brief maxElementIndex returns the index of the biggest uint-Element.
    * \param searchData vector
    * \param startIndex start search here
    * \param EndIndex end search here
    * \param maxElementValue can be used to return the actual value of the biggest element
    * \return the index of the biggest element
    */
   uint maxElementIndex(const std::vector<uint>& searchData, uint startIndex, uint EndIndex,
                        uint* maxElementValue = nullptr) const;
protected:
   std::string filePath;
   const char sep;
   const char dec;
   const char lineSep;
   std::vector<std::string>* headLineEntries;
   std::vector<vecD*>* data;
};

#endif // AF4CSVWRITER_H
