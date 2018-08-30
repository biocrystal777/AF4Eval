#ifndef AF4CSVPARSER_H
#define AF4CSVPARSER_H
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
#include "./af4constants.h"
#include "./af4datatypes.h"
#include <algorithm>
#include <qdebug.h>

/*! ***********************************************************
***
***  \class     AF4CSVParser "src/Core/af4csvparser.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
**************************************************************/


class AF4CsvParser final
{

public:
   /*!
    * \brief AF4CsvParser
    * \param filePath absolute path of the file to be read
    */
   explicit AF4CsvParser(const std::string& filePath,
                         const char sep,
                         const char dec
                         );

   AF4CsvParser() = delete;
   AF4CsvParser(const AF4CsvParser& src) = delete;
   AF4CsvParser& operator= (const AF4CsvParser& src) = delete;
   AF4CsvParser(AF4CsvParser&& src) = delete;
   AF4CsvParser& operator= (AF4CsvParser&& src) = delete;


   /*!
    * \brief ~FFFCsvParser Default Destructor
    */
   ~AF4CsvParser(){}

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
   std::vector<std::string> getHeadLines() const;

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

#endif // AF4CSVPARSER_H
