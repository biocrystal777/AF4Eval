#ifndef FFFNUMBEREDTOOLBUTTON_H
#define FFFNUMBEREDTOOLBUTTON_H

#include <QToolButton>

/*! **********************************************************************************************
***
***  \class     AF4NumberedToolButton "src/Gui/smallQDerivates/af4numberedtoolbutton.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-10-02
***  \copyright LGPL 3.0
***
*************************************************************************************************/

class AF4NumberedToolButton : public QToolButton
{
   Q_OBJECT
public:
   /*!
    * \brief AF4NumberedToolButton
    * \param id
    * \param parent
    */
   AF4NumberedToolButton(int id, QWidget *parent);

   /*!
    *
    */
   ~AF4NumberedToolButton(){}

   /*!
    * \brief changeIdTo
    * \param newId
    */
   void changeIdTo(int newId);

protected:
   int id;

signals:
   /*!
    * \brief clicked
    * \param id
    */
   void clicked(int id);

private:
   AF4NumberedToolButton(const AF4NumberedToolButton& src) = delete;
   AF4NumberedToolButton& operator= (AF4NumberedToolButton& src) = delete;
   AF4NumberedToolButton(AF4NumberedToolButton&& src) = delete;
   AF4NumberedToolButton& operator= (AF4NumberedToolButton&& src) = delete;
};
#endif // FFFNUMBEREDTOOLBUTTON_H
