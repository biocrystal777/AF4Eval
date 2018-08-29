#ifndef FFFNUMBEREDCHECKBOX_H
#define FFFNUMBEREDCHECKBOX_H

#include <QCheckBox>
#include <QSettings>
#include "../Core/fffconstants.h"

class FFFNumberedCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit FFFNumberedCheckBox(int id, QWidget *parent = nullptr);
    ~FFFNumberedCheckBox();

    FFFNumberedCheckBox(const FFFNumberedCheckBox& src) = delete;
    FFFNumberedCheckBox& operator= (FFFNumberedCheckBox& src) = delete;
    FFFNumberedCheckBox(FFFNumberedCheckBox&& src) = delete;
    FFFNumberedCheckBox& operator= (FFFNumberedCheckBox&& src) = delete;

protected:
    int id;
signals:
    void checkedID(int id, bool enabled);
public slots:
    void callcheckedID(bool checked);
};

#endif // FFFNUMBEREDCHECKBOX_H
