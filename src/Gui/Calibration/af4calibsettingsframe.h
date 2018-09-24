#ifndef AF4CALIBSETTINGSFRAME_H
#define AF4CALIBSETTINGSFRAME_H

#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include "./af4channeldimswidget.h"
#include "./af4channelcalibwidget.h"
#include "../../Core/af4parameterstructs.h"

/*! ***************************************************************************************
***
***  \class     AF4CalibSettingsFrame "src/Gui/Calibration/af4calibsettingsframe.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
********************************************************************************************/

class AF4CalibSettingsFrame final : public QFrame {
    Q_OBJECT
public:
    explicit AF4CalibSettingsFrame(QMap<QString, AF4ChannelDimsWidget*>* channelConfigWidgets,
                                   QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* channelCalibWidgets,
                                   const QString &prefix,
                                   QWidget *parent = nullptr);

    ~AF4CalibSettingsFrame();

    AF4CalibSettingsFrame(const AF4CalibSettingsFrame& src) = delete;
    AF4CalibSettingsFrame& operator= (AF4CalibSettingsFrame& src) = delete;
    AF4CalibSettingsFrame(AF4CalibSettingsFrame&& src) = delete;
    AF4CalibSettingsFrame& operator= (AF4CalibSettingsFrame&& src) = delete;

    /*!
     * \brief getChLength
     * \return
     */
    double getChLength() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getChLength();
    }

    /*!
     * \brief getLength1
     * \return
     */
    double getLength1() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getLength1();
    }

    /*!
     * \brief getLength2
     * \return
     */
    double getLength2() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getLength2();
    }

    /*!
     * \brief getLength3
     * \return
     */
    double getLength3() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getLength3();
    }

    /*!
     * \brief getB0
     * \return
     */
    double getB0() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getB0();
    }

    /*!
     * \brief getBL
     * \return
     */
    double getBL() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getBL();
    }

    /*!
     * \brief getChWidth
     * \return
     */
    double getChWidth() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getChannelWidth();
    }

    /*!
     * \brief getHydrodynVolume
     * \return
     */
    double getHydrodynVolume() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getHydrodynVolume();
    }

    /*!
     * \brief getGeometVolume
     * \return
     */
    double getGeometVolume() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getGeometVolume();
    }

    /*!
     * \brief getChannelDimensions
     * \return
     */
    ChannelDims getChannelDimensions() const {  return channelConfigWidgets->value(channelChooser->currentText())->getChannelDimensions(); }

    /*!
     * \brief getChannelDimsFromCalib
     * \return
     */
    ChannelDimsFromCalib getChannelDimsFromCalib() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getChannelDimsFromCalib();
    }

    /*!
     * \brief saveParameters
     */
    void saveParameters();

private:
    QMap<QString, AF4ChannelDimsWidget*>* const channelConfigWidgets;
    QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* const channelCalibWidgets;

    QGridLayout *layout                         = nullptr;
    QList<QString> *channelKeyList              = nullptr;
    QList<QList<QString>*> *calibKeyList        = nullptr;
    QComboBox *channelChooser                   = nullptr;
    QComboBox *currentCalibChooser              = nullptr;
    QMap<QString, QComboBox*>* allCalibChoosers = nullptr;
    QString currentChannelKey;
    QString currentCalibKey;

    QLabel *chLength                            = nullptr;
    QLabel *length1                            = nullptr;
    QLabel *length2                            = nullptr;
    QLabel *length3                            = nullptr;
    QLabel *b0                                 = nullptr;
    QLabel *bL                                 = nullptr;
    QLabel *channelWidth                       = nullptr;
    QLabel *channelVolumeDef                   = nullptr;
    QLabel *channelVolume                      = nullptr;
    QLabel *channelVolumeGeo                   = nullptr;

    const QString prefix;

    /*!
     * \brief loadParameters
     */
    void loadParameters();


public slots:
    /*!
     * \brief updateChannelValues updates the displayed channel values according to the
     *        values entered in the FFFChannelConfigWidget
     * \param channelKey the key of the displayed channel (equal to its name)
     */
    void updateChannelValues(QString channelKey);

    /*!
     * \brief updateCalibValues updates the displayed calibration value (i.e. channelwidth) according
     *        to its value in the corresponding FFFChannelCalibWidget
     *        called when new item in the combobox is chosen; CHANGES the key state variable currentCalibValue
     * \param calibKey
     */
    void updateCalibValues(QString calibKey);

   /*!
    * \brief adaptCalibValues updates the displayed calibration value (i.e. channelwidth) according
    *        to its value in the corresponding FFFChannelCalibWidget
    *        Called. when this tab widget is chosen and does NOT change the key state variable currentCalibValue
    * \param calibKey
    */
   void adaptCalibValues(QString calibKey);

   /*!
    * \brief adaptChannelParameters adapts the comboboxes according
    *        to the internal structures (QMaps) that contain
    *        the information about all existing channels and
    *        calibration profiles. Additionally, displayed
    *        values of the chosen values are updated
    */
   void adaptChannelParameters();

   /*!
    * \brief enableVolume
    * \param enable
    */
   void enableVolume(bool enable);
};
#endif // AF4CALIBSETTINGSFRAME_H
