#ifndef AF4NUMBEREDCHECKBOX_H
#define AF4NUMBEREDCHECKBOX_H

#include <QCheckBox>
#include <QSettings>
#include "../Core/af4constants.h"

class AF4NumberedCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit AF4NumberedCheckBox(int id, QWidget *parent = nullptr);
    ~AF4NumberedCheckBox();

    AF4NumberedCheckBox(const AF4NumberedCheckBox& src) = delete;
    AF4NumberedCheckBox& operator= (AF4NumberedCheckBox& src) = delete;
    AF4NumberedCheckBox(AF4NumberedCheckBox&& src) = delete;
    AF4NumberedCheckBox& operator= (AF4NumberedCheckBox&& src) = delete;

protected:
    int id;
signals:
    void checkedID(int id, bool enabled);
public slots:
    void callcheckedID(bool checked);
};

#endif // AF4NUMBEREDCHECKBOX_H
