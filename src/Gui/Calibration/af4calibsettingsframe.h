#ifndef AF4CALIBSETTINGSFRAME_H
#define AF4CALIBSETTINGSFRAME_H

#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include "./af4channeldimswidget.h"
#include "./af4channelcalibwidget.h"
#include "../../Core/af4evalmacros.h"
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
   AF4CalibSettingsFrame(QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets,
                         QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget *> > > channelCalibWidgets,
                         const QString &prefix,
                         QWidget *parent = nullptr);
   /*!
     * \brief saveParameters
     */
   void saveParameters();

   /*!
     * \brief getChLength
     * \return
     */
   double getChLength() const {
      return channelConfigWidgets->value(channelChooser->currentText())->getChLength();
   }

   /*!
     * \brief getL1
     * \return
     */
   double getL1() const {
      return channelConfigWidgets->value(channelChooser->currentText())->getL1();
   }

   /*!
     * \brief getL2
     * \return
     */
   double getL2() const {
      return channelConfigWidgets->value(channelChooser->currentText())->getL2();
   }

   /*!
     * \brief getL3
     * \return
     */
   double getL3() const {
      return channelConfigWidgets->value(channelChooser->currentText())->getL3();
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
      return channelCalibWidgets->value(channelChooser->currentText()).value(currentCalibChooser->currentText())->getClassicalChannelWidth();

   }

   /*!
     * \brief getHydrodynVolume
     * \return
     */
   double getHydrodynVolume() const {
      return channelCalibWidgets->value(channelChooser->currentText()).value(currentCalibChooser->currentText())->getHydrodynVolume();
   }

   /*!
     * \brief getGeometVolume
     * \return
     */
   double getGeometVolume() const {
      return channelCalibWidgets->value(channelChooser->currentText()).value(currentCalibChooser->currentText())->getGeometVolume();
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
      return channelCalibWidgets->value(channelChooser->currentText()).value(currentCalibChooser->currentText())->getChannelDimsFromCalib();
   }

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

private:
   const QString prefix;
   QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets;
   QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > channelCalibWidgets;

   QGridLayout *layout                         = nullptr;
   QList<QString> *channelKeyList              = nullptr;
   QList<QList<QString> > calibKeyList;
   QComboBox *channelChooser                   = nullptr;
   QComboBox *currentCalibChooser              = nullptr;
   QMap<QString, QComboBox*>* allCalibChoosers = nullptr;
   QString currentChannelKey;
   QString currentCalibKey;

   QLabel *LTot                            = nullptr;
   QLabel *L1                             = nullptr;
   QLabel *L2                             = nullptr;
   QLabel *L3                             = nullptr;
   QLabel *b0                                  = nullptr;
   QLabel *bL                                  = nullptr;
   QLabel *channelWidth                        = nullptr;
   QLabel *channelVolumeDef                    = nullptr;
   QLabel *channelVolume                       = nullptr;
   QLabel *channelVolumeGeo                    = nullptr;

   /*!
     * \brief loadParameters
     */
   void loadParameters();

   /*!
    * \brief destructor
    */
   ~AF4CalibSettingsFrame();

   NO_COPY_MOVE_CTORS(AF4CalibSettingsFrame)
};
#endif // AF4CALIBSETTINGSFRAME_H
