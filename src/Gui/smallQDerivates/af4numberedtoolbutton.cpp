#include "af4numberedtoolbutton.h"

AF4NumberedToolButton::AF4NumberedToolButton(int id, QWidget *parent) :
   QToolButton(parent), id(id)
{
   connect(this, (&QToolButton::clicked),
           this, [this](){emit clicked(this->id);} );
}

void AF4NumberedToolButton::changeIdTo(int newId)
{
   this->id = newId;
}
