#ifndef AF4SLSCALIBRATIONFRAME_H
#define AF4SLSCALIBRATIONFRAME_H

#include <QGroupBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QSettings>
#include "../Core/af4parameterstructs.h"
#include "../Core/af4constants.h"
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../smallQDerivates/af4numberedcheckbox.h"
#include "../Core/af4evalmacros.h"

/*! **********************************************************************************************
***
***  \class   AF4SLSCalibrationFrame "src/Gui/Calibration/af4slscalibrationframe.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   0.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4SLSCalibrationFrame : public QFrame{
    Q_OBJECT
public:
    explicit AF4SLSCalibrationFrame(const QString &prefix, int id, QWidget *parent);
    ~AF4SLSCalibrationFrame();

    AF4SLSCalibrationFrame(const AF4SLSCalibrationFrame& src) = delete;
    AF4SLSCalibrationFrame& operator= (AF4SLSCalibrationFrame& src) = delete;
    AF4SLSCalibrationFrame(AF4SLSCalibrationFrame&& src) = delete;
    AF4SLSCalibrationFrame& operator= (AF4SLSCalibrationFrame&& src) = delete;

    /*!
     * \brief getCalibParamters
     * \return
     */
    SLSCalibrationParameters getCalibParamters();
    /*!
     * \brief getRIConstant
     * \return
     */
    inline double getRIConstant() const      { return riConstant->value(); }
    /*!
     * \brief getMallsConstant
     * \return
     */
    inline double getMallsConstant() const   { return mallsConstant->value(); }
    /*!
     * \brief getVoltSignalChecked
     * \return
     */
    inline bool getVoltSignalChecked() const { return voltButton->isChecked(); }
    /*!
     * \brief getUsedAngles
     * \return
     */
    QList<bool> getUsedAngles();
    /*!
     * \brief getMallsAngleConstants
     * \return
     */
    QList<double> getMallsAngleConstants();
    /*!
     * \brief writeSettings
     */
    void writeSettings();

private:
    QString prefix;
    int id;
    QList<AF4NumberedCheckBox*> useAngles;
    QList<QLabel*>              angleLabels;
    QList<QDoubleSpinBox*>      mallsAngleConstants;
    QGridLayout     *lay                         = nullptr;
    AF4SciNotSpinBox *riConstant                  = nullptr;
    AF4SciNotSpinBox *mallsConstant               = nullptr;
    QGroupBox *rayleighGroup                     = nullptr;
    QGridLayout *rayleighLayout                  = nullptr;
    QRadioButton *rayleighButton                 = nullptr;
    QRadioButton *voltButton                     = nullptr;

private slots:
    /*!
     * \brief enableAngle
     * \param i
     * \param enable
     */
    void enableAngle(bool enable, int i) const;
    /*!
     * \brief setToRayleigh
     */
    void setToRayleigh();
    /*!
     * \brief setToVolt
     */
    void setToVolt();
};

#endif // AF4SLSCALIBRATIONFRAME_H
