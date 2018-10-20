#ifndef AF4NUMBEREDCHECKBOX_H
#define AF4NUMBEREDCHECKBOX_H

#include <QCheckBox>
#include "../Core/af4evalmacros.h"
//#include <QSettings>
//#include "../Core/af4constants.h"

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

class AF4NumberedCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    AF4NumberedCheckBox(int id, QWidget *parent);
    ~AF4NumberedCheckBox(){}

signals:
    void toggled(bool enabled, int id);

protected:
    int id;

private:
    NO_COPY_ASSIGNMENT_CTORS(AF4NumberedCheckBox)
};

#endif // AF4NUMBEREDCHECKBOX_H
