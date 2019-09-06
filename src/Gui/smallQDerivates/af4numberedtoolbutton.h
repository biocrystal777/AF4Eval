#ifndef AF4NUMBEREDTOOLBUTTON_H
#define AF4NUMBEREDTOOLBUTTON_H

#include <QToolButton>
#include "../Core/af4evalmacros.h"

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
   NO_COPY_MOVE_CTORS(AF4NumberedToolButton)
};
#endif // AF4NUMBEREDTOOLBUTTON_H
