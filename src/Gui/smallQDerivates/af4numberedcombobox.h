#ifndef AF4NUMBEREDCOMBOBOX_H
#define AF4NUMBEREDCOMBOBOX_H

#include <QComboBox>

/*! **********************************************************************************************
***
***  \class     AF4NumberedCheckBox "src/Gui/smallQDerivates/af4numberedcheckbox.h"
***  \brief     Implements a simple version of a QSpinBox which can be identified quickly if used
***             eg.in a container
***  \details
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-09-24
***  \copyright LGPL 3.0
***
*************************************************************************************************/

class AF4NumberedComboBox : public QComboBox
{
   Q_OBJECT
public:
   explicit AF4NumberedComboBox(int id, QWidget *parent = nullptr);
   ~AF4NumberedComboBox(){}

protected:
   int id;

signals:
   void currentIndexChanged(int index, int id);

private:
   AF4NumberedComboBox(const AF4NumberedComboBox& src) = delete;
   AF4NumberedComboBox& operator= (AF4NumberedComboBox& src) = delete;
   AF4NumberedComboBox(AF4NumberedComboBox&& src) = delete;
   AF4NumberedComboBox& operator= (AF4NumberedComboBox&& src) = delete;
};

#endif // AF4NUMBEREDCOMBOBOX_H
