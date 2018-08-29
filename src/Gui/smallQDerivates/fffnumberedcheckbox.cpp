#include "fffnumberedcheckbox.h"

FFFNumberedCheckBox::FFFNumberedCheckBox(int id, QWidget *parent) : QCheckBox(parent), id(id)
{
   QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(callcheckedID(bool)));
}

FFFNumberedCheckBox::~FFFNumberedCheckBox()
{
}

void FFFNumberedCheckBox::callcheckedID(bool checked)
{
   emit checkedID(id, checked);
}
