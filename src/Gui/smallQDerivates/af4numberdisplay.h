#ifndef AF4NUMBERDISPLAY_H
#define AF4NUMBERDISPLAY_H

#include <QLineEdit>
#include "../Core/af4evalmacros.h"

enum class SignDisplayMode{
   Always = 0,
   OnlyNegative = 1,
   NegativeAligned = 2
};

class AF4NumberDisplay : public QLineEdit
{
   Q_OBJECT
public:
   AF4NumberDisplay(uint value,   QWidget *parent = nullptr);
   AF4NumberDisplay(int value,    QWidget *parent = nullptr);
   AF4NumberDisplay(double value, char format = 'f', QWidget *parent = nullptr);
   AF4NumberDisplay(               QWidget *parent = nullptr);


   void setValue(uint value);
   void setValue(int value);
   bool setValue(double value, char format = 'f');
   QVariant value() const;

   char getFormat() const { return formatFlag; }
   bool setFormat(char format);
   void setSignDisplayMode(SignDisplayMode s);
   SignDisplayMode signDisplayMode() const { return this->signDisplay;}
   uint getPrecision() const {return this->precision;}
   bool setPrecision(uint prec);

private:

   bool isDoubleFormatFlag( char c ) const {
      if (c == 'e' || c == 'E' || c == 'f' || c == 'F' || c == 'g' || c == 'G')
         return true;
      else return false;
   }

   bool isFormatFlag( char c ) const {
      if ( c == 'i' || c == 'u' || isDoubleFormatFlag(c))
         return true;
      else return false;
   }

   union ndValue {
      double d;
      uint   u;
      int    i;
   };
   ndValue val;
   // 'u' => uint , 'i' => int
   char formatFlag             = 'f';
   uint precision;
   SignDisplayMode signDisplay = SignDisplayMode::Always;

   void updateDisplay();

   NO_COPY_MOVE_CTORS(AF4NumberDisplay)
};

#endif // AF4NUMBERDISPLAY_H
