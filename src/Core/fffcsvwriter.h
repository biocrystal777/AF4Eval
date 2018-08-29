#ifndef FFFCSVWRITER_H
#define FFFCSVWRITER_H
#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <vector>
#include "../Gui/GeneralWidgets/ffflog.h"
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include "./fffconstants.h"
#include "./fffdatatypes.h"

/**************************************************************
***
***   FFFCsvWriter
***
**************************************************************/


/*!
 * \class FFFCsvWriter
 * \brief The FFFCsvWriter class is used to write a file in csv format with headLines
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFCsvWriter final
{

public:

   /*!
    * \brief FFFCsvWriter is the constructor of the class.
    * \param filePath path of the file. A file that already exists will be overwritten
    */
   explicit FFFCsvWriter(const std::string& filePath);


   /*!
    * \brief FFFCsvWriter Default destructor
    */
   ~FFFCsvWriter();

    FFFCsvWriter(const FFFCsvWriter& src) = delete;
    FFFCsvWriter& operator= (const FFFCsvWriter& src) = delete;
    FFFCsvWriter(FFFCsvWriter&& src) = delete;
    FFFCsvWriter& operator= (FFFCsvWriter&& src) = delete;

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

   /*!
    * \brief to_string converts an int to a std::string using a stringstream
    * \param number integer to be converted
    * \return std::string
    */
   std::string to_string(int number) const;

   /*!
    * \brief to_string converts a double to a std::string using a stringstream
    * \param number
    * \return std::string
    */
   std::string to_string(double number) const;

};

#endif // FFFCSVWRITER_H
