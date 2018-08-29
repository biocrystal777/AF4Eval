#include "fffnumberedtoolbutton.h"


void FFFNumberedToolButton::callClickedID()
{
   emit clickedID(id);
}


FFFNumberedToolButton::FFFNumberedToolButton(int id, QWidget *parent) :
   QToolButton(parent), id(id)
{
   QObject::connect(this, SIGNAL(clicked()), this, SLOT(callClickedID()));
}

FFFNumberedToolButton::~FFFNumberedToolButton()
{
}

void FFFNumberedToolButton::changeIdTo(int newId)
{
   this->id = newId;
}
