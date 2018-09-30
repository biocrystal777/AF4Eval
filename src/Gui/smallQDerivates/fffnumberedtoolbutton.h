#ifndef FFFNUMBEREDTOOLBUTTON_H
#define FFFNUMBEREDTOOLBUTTON_H

#include <QToolButton>

class FFFNumberedToolButton : public QToolButton
{
   Q_OBJECT
public:
    FFFNumberedToolButton(int id, QWidget *parent );
   ~FFFNumberedToolButton();

   void changeIdTo(int newId);

public slots:
   void callClickedID();

protected:
   int id;

signals:
   void clicked(int id);

private:
   FFFNumberedToolButton(const FFFNumberedToolButton& src) = delete;
   FFFNumberedToolButton& operator= (FFFNumberedToolButton& src) = delete;
   FFFNumberedToolButton(FFFNumberedToolButton&& src) = delete;
   FFFNumberedToolButton& operator= (FFFNumberedToolButton&& src) = delete;
};



#endif // FFFNUMBEREDTOOLBUTTON_H
