#ifndef FFFNUMBEREDTOOLBUTTON_H
#define FFFNUMBEREDTOOLBUTTON_H

#include <QToolButton>

class FFFNumberedToolButton : public QToolButton
{
   Q_OBJECT
public:
    FFFNumberedToolButton(int id, QWidget *parent = nullptr);
   ~FFFNumberedToolButton();

    FFFNumberedToolButton(const FFFNumberedToolButton& src) = delete;
    FFFNumberedToolButton& operator= (FFFNumberedToolButton& src) = delete;
    FFFNumberedToolButton(FFFNumberedToolButton&& src) = delete;
    FFFNumberedToolButton& operator= (FFFNumberedToolButton&& src) = delete;

   void changeIdTo(int newId);
protected:
   int id;
signals:
   void clickedID(int id);
public slots:
   void callClickedID();
};



#endif // FFFNUMBEREDTOOLBUTTON_H
