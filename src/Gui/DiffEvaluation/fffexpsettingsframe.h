#ifndef FFFEXPSETTINGSFRAME_H
#define FFFEXPSETTINGSFRAME_H

#include <QFrame>
#include <QSettings>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include "../GeneralWidgets/ffflog.h"
#include "../../Core/af4parameterstructs.h"

class FFFExpSettingsFrame final : public QFrame
{
    Q_OBJECT
public:
    explicit FFFExpSettingsFrame(const QString& prefix,int id, QWidget* parent = nullptr);
    ~FFFExpSettingsFrame();

    FFFExpSettingsFrame(const FFFExpSettingsFrame& src) = delete;
    FFFExpSettingsFrame& operator= (FFFExpSettingsFrame& src) = delete;
    FFFExpSettingsFrame(FFFExpSettingsFrame&& src) = delete;
    FFFExpSettingsFrame& operator= (FFFExpSettingsFrame&& src) = delete;

    ExpFFFParameters getExpFFFParameters();


    inline void setId(int newId) { id = newId; }

    /*!
     * \brief getTemperatureVal returns the displayed temmperature
     * \return temperature
     */
     inline double getTemperatureVal() const { return temperature->value(); }

     bool setTemperatureVal(double val) const;

     /*!
     * \brief getViscosityVal returns the displayed temmperature
     * \return viscosity
     */
     inline double getViscosityVal() const { return viscosity->value(); }

     bool setViscosity(double val) const;

    /*!
     * \brief getVoidPeakTimeVal returns the displayed voidPeakTime
     * \return voidPeakTime
     */
    inline double getVoidPeakTimeVal() const {return voidPeakTime->value(); }

    bool setVoidPeakTimeVal(double val) const;

    /*!
     * \brief getRelFocusPointVal returns the displayed voidPeakTime
     * \return relFocusPoint
     */
    inline double getRelFocusPointVal() const { return relFocusPoint->value(); }

    bool setRelFocusPointVal(double val) const;

    /*!
     * \brief getElutionFlowVal returns the displayed elutionFlow
     * \return elutionFlow
     */
    inline double getElutionFlowVal() const { return elutionFlow->value(); }

    bool setElutionFlowVal(double val) const;

    /*!
     * \brief getCrossFlowVal returns the displayed crossFlow
     * \return crossFlow
     */
    inline double getCrossFlowVal() const { return crossFlow->value(); }
    bool setCrossFlowVal(double val) const;


    /*!
     * \brief getLeftOffsetTimeVal returns the displayed leftOffsetTime (t0)
     * \return leftOffsetTime
     */
    inline double getLeftOffsetTimeVal() const {return leftOffsetTime->value(); }
    bool setLeftOffsetTimeVal(double val) const;

    inline bool getRecalcVolume() const { return useVoidPeakBox->isChecked(); }
    void setRecalcVolume(bool setValue);


    void loadSettings();
    void writeSettings();

private:

    const QString prefix;
    int id;

    QGridLayout    *lay              = nullptr;
    QDoubleSpinBox *elutionFlow      = nullptr;
    QDoubleSpinBox *crossFlow        = nullptr;
    QDoubleSpinBox *leftOffsetTime   = nullptr;
    QCheckBox      *useVoidPeakBox   = nullptr;
    QLabel         *voidPeakLabel    = nullptr;
    QDoubleSpinBox *voidPeakTime     = nullptr;
    QDoubleSpinBox *relFocusPoint    = nullptr;

    QDoubleSpinBox *viscosity        = nullptr;
    QDoubleSpinBox *temperature      = nullptr;


signals:
    void callEnableVolume(bool enable);

private slots:

    void enableVoidPeakTime(bool checked);
};

#endif // FFFEXPSETTINGSFRAME_H
