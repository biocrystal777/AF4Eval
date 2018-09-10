#ifndef AF4EXPSETTINGSFRAME_H
#define AF4EXPSETTINGSFRAME_H

#include <QFrame>
#include <QSettings>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include "../GeneralWidgets/ffflog.h"
#include "../../Core/af4parameterstructs.h"

/*! **********************************************************************************************
***
***  \class     AF4ExpSettingsFrame "src/Gui/Calibration/af4expsettingsframe.h"
***  \brief     AF4ExpSettingsFrame
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4ExpSettingsFrame final : public QFrame
{   
    Q_OBJECT

public:
    explicit AF4ExpSettingsFrame(const QString& prefix,int id, QWidget* parent = nullptr);

   ~AF4ExpSettingsFrame(){writeSettings();}

    AF4ExpSettingsFrame(const AF4ExpSettingsFrame& src)        = delete;
    AF4ExpSettingsFrame& operator= (AF4ExpSettingsFrame& src)  = delete;
    AF4ExpSettingsFrame(AF4ExpSettingsFrame&& src)             = delete;
    AF4ExpSettingsFrame& operator= (AF4ExpSettingsFrame&& src) = delete;

    ExpFFFParameters getExpFFFParameters();

    inline void setId(int newId) { id = newId; }

    /*!
     * \brief getTemperatureVal returns the displayed temmperature
     * \return temperature
     */
     inline double getTemperatureVal() const { return temperature->value(); }
     /*!
      * \brief setTemperatureVal
      * \param val
      * \return
      */
     bool setTemperatureVal(double val) const;

     /*!
     * \brief getViscosityVal returns the displayed temmperature
     * \return viscosity
     */
     inline double getViscosityVal() const { return viscosity->value(); }
     /*!
      * \brief setViscosity
      * \param val
      * \return
      */
     bool setViscosity(double val) const;

    /*!
     * \brief getVoidPeakTimeVal returns the displayed voidPeakTime
     * \return voidPeakTime
     */
    inline double getVoidPeakTimeVal() const {return voidPeakTime->value(); }
    /*!
     * \brief setVoidPeakTimeVal
     * \param val
     * \return
     */
    bool setVoidPeakTimeVal(double val) const;

    /*!
     * \brief getRelFocusPointVal returns the displayed voidPeakTime
     * \return relFocusPoint
     */
    inline double getRelFocusPointVal() const { return relFocusPoint->value(); }
    /*!
     * \brief setRelFocusPointVal
     * \param val
     * \return
     */
    bool setRelFocusPointVal(double val) const;

    /*!
     * \brief getElutionFlowVal returns the displayed elutionFlow
     * \return elutionFlow
     */
    inline double getElutionFlowVal() const { return elutionFlow->value(); }
    /*!
     * \brief setElutionFlowVal
     * \param val
     * \return
     */
    bool setElutionFlowVal(double val) const;

    /*!
     * \brief getCrossFlowVal returns the displayed crossFlow
     * \return crossFlow
     */
    inline double getCrossFlowVal() const { return crossFlow->value(); }
    /*!
     * \brief setCrossFlowVal
     * \param val
     * \return
     */
    bool setCrossFlowVal(double val) const;


    /*!
     * \brief getLeftOffsetTimeVal returns the displayed leftOffsetTime (t0)
     * \return leftOffsetTime
     */
    inline double getLeftOffsetTimeVal() const {return leftOffsetTime->value(); }
    /*!
     * \brief setLeftOffsetTimeVal
     * \param val
     * \return
     */
    bool setLeftOffsetTimeVal(double val) const;
    /*!
     * \brief getRecalcVolume
     * \return
     */
    inline bool getRecalcVolume() const { return useVoidPeakBox->isChecked(); }
    /*!
     * \brief setRecalcVolume
     * \param setValue
     */
    void setRecalcVolume(bool setValue);

    /*!
     * \brief loadSettings
     */
    void loadSettings();

    /*!
     * \brief writeSettings
     */
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
    /*!
     * \brief callEnableVolume
     * \param enable
     */
    void callEnableVolume(bool enable);

private slots:
    /*!
     * \brief enableVoidPeakTime
     * \param checked
     */
    void enableVoidPeakTime(bool checked);
};
#endif // AF4EXPSETTINGSFRAME_H
