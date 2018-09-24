#include "fffnumberedcheckbox.h"

AF4NumberedCheckBox::AF4NumberedCheckBox(int id, QWidget *parent) : QCheckBox(parent), id(id)
{
   QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(callcheckedID(bool)));
}

AF4NumberedCheckBox::~AF4NumberedCheckBox()
{
}

void AF4NumberedCheckBox::callcheckedID(bool checked)
{
   emit checkedID(id, checked);
}
