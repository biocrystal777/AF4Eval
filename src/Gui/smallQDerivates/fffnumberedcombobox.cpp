#include "fffnumberedcombobox.h"


FFFNumberedComboBox::FFFNumberedComboBox(int id, QWidget *parent) : QComboBox(parent), id(id)
{
   QObject::connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(callCurrentIndexChangedId(int)));
}

FFFNumberedComboBox::~FFFNumberedComboBox()
{

}

void FFFNumberedComboBox::callCurrentIndexChangedId(int index)
{
   currentIndexChangedId(this->id, index);
}
