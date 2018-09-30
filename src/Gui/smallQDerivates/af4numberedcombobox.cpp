#include "af4numberedcombobox.h"

AF4NumberedComboBox::AF4NumberedComboBox(int id, QWidget *parent) : QComboBox(parent), id(id)
{
   connect(this,  qOverload<int>(&QComboBox::currentIndexChanged),
           this, [this](int index){currentIndexChanged(index, this->id);} );
}

