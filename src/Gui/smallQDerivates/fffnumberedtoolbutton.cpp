#include "fffnumberedtoolbutton.h"


void FFFNumberedToolButton::callClickedID()
{
   emit clicked(id);
}


FFFNumberedToolButton::FFFNumberedToolButton(int id, QWidget *parent) :
   QToolButton(parent), id(id)
{
   connect(this, (&QToolButton::clicked), this, [this](){emit clicked(this->id);} );
}

FFFNumberedToolButton::~FFFNumberedToolButton()
{
}

void FFFNumberedToolButton::changeIdTo(int newId)
{
   this->id = newId;
}
