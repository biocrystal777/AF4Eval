#include "af4numberedcheckbox.h"

AF4NumberedCheckBox::AF4NumberedCheckBox(int id, QWidget *parent)
   : QCheckBox(parent), id(id)
{
   QObject::connect(this, &QCheckBox::toggled,
                    this, [this](bool checked){emit toggled(checked,this->id);}
   );
}
