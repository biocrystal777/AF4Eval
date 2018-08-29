#ifndef FFFCALIBSETTINGSFRAME_H
#define FFFCALIBSETTINGSFRAME_H

#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include "./fffchannelconfigwidget.h"
#include "./fffchannelcalibwidget.h"
#include "../../Core/fffparameterstructs.h"
#include <QFrame>

class FFFCalibSettingsFrame final : public QFrame {
    Q_OBJECT
public:
    explicit FFFCalibSettingsFrame(QMap<QString, FFFChannelConfigWidget*>* channelConfigWidgets,
                                   QMap<QString, QMap<QString, FFFChannelCalibWidget*>*>* channelCalibWidgets,
                                   const QString &prefix,
                                   QWidget *parent = nullptr);

    ~FFFCalibSettingsFrame();

    FFFCalibSettingsFrame(const FFFCalibSettingsFrame& src) = delete;
    FFFCalibSettingsFrame& operator= (FFFCalibSettingsFrame& src) = delete;
    FFFCalibSettingsFrame(FFFCalibSettingsFrame&& src) = delete;
    FFFCalibSettingsFrame& operator= (FFFCalibSettingsFrame&& src) = delete;

    inline double getChLength() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getChLength();
    }
    inline double getLength1() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getLength1();
    }
    inline double getLength2() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getLength2();
    }
    inline double getLength3() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getLength3();
    }



    inline double getB0() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getB0();
    }
    inline double getBL() const {
       return channelConfigWidgets->value(channelChooser->currentText())->getBL();
    }
    inline double getChWidth() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getChannelWidth();
    }
    inline double getHydrodynVolume() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getHydrodynVolume();
    }
    inline double getGeometVolume() const {
       return channelCalibWidgets->value(channelChooser->currentText())->value(currentCalibChooser->currentText())->getGeometVolume();
    }
    ChannelDims getChannelDimensions() const;
    ChannelDimsFromCalib getChannelDimsFromCalib() const;

    void saveParameters();

private:
    QMap<QString, FFFChannelConfigWidget*>* const channelConfigWidgets;
    QMap<QString, QMap<QString, FFFChannelCalibWidget*>*>* const channelCalibWidgets;

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
    QLabel *channelVolumeGeo                      = nullptr;

    const QString prefix;

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

   void enableVolume(bool enable);


};
#endif // FFFCALIBSETTINGSFRAME_H
