#ifndef FFFNUMBEREDCOMBOBOX_H
#define FFFNUMBEREDCOMBOBOX_H

#include <QComboBox>


class FFFNumberedComboBox : public QComboBox
{
   Q_OBJECT
public:
   explicit FFFNumberedComboBox(int id, QWidget *parent = nullptr);
   ~FFFNumberedComboBox();

    FFFNumberedComboBox(const FFFNumberedComboBox& src) = delete;
    FFFNumberedComboBox& operator= (FFFNumberedComboBox& src) = delete;
    FFFNumberedComboBox(FFFNumberedComboBox&& src) = delete;
    FFFNumberedComboBox& operator= (FFFNumberedComboBox&& src) = delete;


protected:
   int id;

signals:
   void currentIndexChangedId(int id, int index);

protected slots:
   void callCurrentIndexChangedId(int index);

};


#endif // FFFNUMBEREDCOMBOBOX_H
