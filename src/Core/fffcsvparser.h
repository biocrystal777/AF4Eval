#ifndef FFFCsvParser_H
#define FFFCsvParser_H
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include "../Gui/GeneralWidgets/ffflog.h"
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "./fffconstants.h"
#include "./fffdatatypes.h"
#include <algorithm>
#include <qdebug.h>

/**************************************************************
***
***   FFFCsvParser
***
**************************************************************/

/*! \class FFFCsvParser
*\brief Class can read a csv file and creates data structures
* on heap (=> have to be destroyed externally as soon as they are not needed anymore)
* \author Benedikt Haeusele
* \version 2.0
* \date March 2013
* \copyright GNU General Public License version 3.0
*/


class FFFCsvParser final
{

public:
   /*!
    * \brief FFFCsvParser
    * \param filePath absolute path of the file to be read
    */
   explicit FFFCsvParser(const std::string& filePath,
                         const char sep,
                         const char dec
                         );

   FFFCsvParser() = delete;
   FFFCsvParser(const FFFCsvParser& src) = delete;
   FFFCsvParser& operator= (const FFFCsvParser& src) = delete;
   FFFCsvParser(FFFCsvParser&& src) = delete;
   FFFCsvParser& operator= (FFFCsvParser&& src) = delete;


   /*!
    * \brief ~FFFCsvParser Default Destructor
    */
   ~FFFCsvParser(){}

   /*!
    * \brief parseFile is used to the parse the file and create the data structures
    * \brief errorInLine indicates the line in the file where a possible error occured
    * \return integer indicates errorvalue
    */
   int parseFile(uint *errorInLine = nullptr);

   int parseFileUTF8(std::ifstream &inpFile);


   /*!
    * \brief getHeadLines returns a vector with the entries of the headline of the csv-file.
    *        Has
    * \return Pointer to the data on heap. Use delete on this address
    *         as soon as you don't need data are not need anymore!
    */
   std::vector<std::string> getHeadLines();

   /*!
    * \brief getHeadLines returns a vector with data columns, which are stored in a vector
    *        themselves each. Access: data->at(column)->at(row)
    * \return Pointer to the data on heap. Use delete on this address
    *         as soon as you don't need data are not need anymore!
    */
   matD getData() const;



   bool dataIsComplete(matD &data);

protected:

   /////////////
   // Members //
   /////////////

   const std::string filePath;
   const char sep;
   const char dec;

   std::vector<std::string> headLineEntries;
   matD dataVectorList;


   /////////////////////////
   // Protected functions //
   /////////////////////////



   std::vector<std::string> split(const std::string& s,
                                  const char delim,
                                  const bool keepEmpty = false) const;
   std::vector<std::wstring> split(const std::wstring& str,
                                   const wchar_t delim,
                                   const bool keepEmpty = false) const;


   std::string::const_iterator findCharPos(std::string::const_iterator start,
                                           std::string::const_iterator end,
                                           const char c) const;

};

#endif // FFFCsvParser_H
