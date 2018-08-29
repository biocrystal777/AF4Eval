#ifndef FFFSLSCALIBRATIONFRAME_H
#define FFFSLSCALIBRATIONFRAME_H

#include <QGridLayout>
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../smallQDerivates/fffnumberedcheckbox.h"
#include "../../Core/fffparameterstructs.h"
#include <QGroupBox>
#include <QRadioButton>


class FFFSLSCalibrationFrame : public QFrame{
    Q_OBJECT
public:
    explicit FFFSLSCalibrationFrame(const QString &prefix,
                                    int id,
                                    QWidget *parent = nullptr);
    ~FFFSLSCalibrationFrame();

    FFFSLSCalibrationFrame(const FFFSLSCalibrationFrame& src) = delete;
    FFFSLSCalibrationFrame& operator= (FFFSLSCalibrationFrame& src) = delete;
    FFFSLSCalibrationFrame(FFFSLSCalibrationFrame&& src) = delete;
    FFFSLSCalibrationFrame& operator= (FFFSLSCalibrationFrame&& src) = delete;

    SLSCalibrationParameters getCalibParamters();
    inline double getRIConstant() const { return riConstant->value();}
    void setRIConstant(double val) const;
    inline double getMallsConstant() const { return mallsConstant->value();}
    inline void setMallsConstant(double val) const;
    inline bool getVoltSignalChecked() const {return voltButton->isChecked(); }

    QList<bool> getUsedAngles();
    QList<double> getMallsAngleConstants();

    void writeSettings();

protected:
    QString prefix;
    int id;
    QGridLayout *lay;
    FFFTwoBoxWidget *riConstant;
    FFFTwoBoxWidget *mallsConstant;
    QList<FFFNumberedCheckBox*> *useAngles;
    QList<QLabel*> *angleLabels;
    QList<QDoubleSpinBox*> *mallsAngleConstants;

    QGroupBox* rayleighGroup                     = nullptr;
    QGridLayout* rayleighLayout                  = nullptr;
    QRadioButton* rayleighButton                 = nullptr;
    QRadioButton* voltButton                     = nullptr;


protected slots:
    void enableAngle(int i, bool enable) const;
    void setToRayleigh(bool);
    void setToVolt(bool);
};



#endif // FFFSLSCALIBRATIONFRAME_H
