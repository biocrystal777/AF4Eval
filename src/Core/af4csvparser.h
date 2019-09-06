#ifndef AF4CSVPARSER_H
#define AF4CSVPARSER_H
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <exception>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "./af4constants.h"
#include "./af4datatypes.h"
#include <algorithm>
#include <qdebug.h>
#include "../Gui/GeneralWidgets/af4log.h"

/*! ***********************************************************
***
***  \class     AF4CSVParser "src/Core/af4csvparser.h"
***  \brief     Small simple parser for complete .csv files
***             No empty entries are allowed!
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2019
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

   /*!
    * \brief ~FFFCsvParser Default Destructor
    */
   ~AF4CsvParser(){}

   /*!
    * \brief parseFile is used to the parse the file and create the data structures
    * \brief errorInLine indicates the line in the file where a possible error occured
    * \return integer indicates errorvalue
    */
   int parseFile(uint *badLineNo = nullptr);

   int parseFileASCII(std::ifstream &inpFile, uint *badLineNo);


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

private:

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

   NO_COPY_MOVE_CTORS(AF4CsvParser)
};
#endif // AF4CSVPARSER_H
