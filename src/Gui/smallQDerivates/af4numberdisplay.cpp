#include "af4numberdisplay.h"

AF4NumberDisplay::AF4NumberDisplay(QWidget *parent) : QLineEdit(parent)
{
   this->setReadOnly(true);
}

AF4NumberDisplay::AF4NumberDisplay(uint number, QWidget *parent) : AF4NumberDisplay(parent)
{

}

AF4NumberDisplay::AF4NumberDisplay(int number, QWidget *parent) : AF4NumberDisplay(parent)
{

}

AF4NumberDisplay::AF4NumberDisplay(double number, char format, QWidget *parent) : AF4NumberDisplay(parent)
{
   this->formatFlag = format;
}

void AF4NumberDisplay::setValue(int value)
{
   this->val.i = value;
   this->formatFlag = 'i';
   updateDisplay();
}

void AF4NumberDisplay::setValue(uint value)
{
   this->val.u = value;
   this->formatFlag = 'u';
   updateDisplay();
}

bool AF4NumberDisplay::setValue(double value, char format)
{
   if(isDoubleFormatFlag(format)) return false;
   this->val.d = value;
   this->formatFlag = format;
   updateDisplay();
}

void AF4NumberDisplay::setSignDisplayMode(SignDisplayMode s)
{
   this->signDisplay = s;
   updateDisplay();
}

QVariant AF4NumberDisplay::value() const
{
   if(formatFlag == 'u')
      return QVariant(val.u);
   else if(formatFlag == 'i')
      return QVariant(val.i);
   else
      return QVariant(val.d);
}

void AF4NumberDisplay::updateDisplay()
{
}


