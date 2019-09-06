#ifndef AF4CHANNELCONFIGUARTIONWIDGET_H
#define AF4CHANNELCONFIGUARTIONWIDGET_H

#include <vector>
#include <string>
#include "./af4caliborgframe.h"
#include "../Core/af4evalmacros.h"
#include "./af4channeldimsorgframe.h"

/*! *********************************************************************************************************************************************
***
***  \class     AF4ChannelConfigurationWidget "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief     master class for calibration functions*
***
***  \details   AF4ChannelConfigurationWidget coordinates two frames (channelDimsOrgFrame and calibsOrgFrame)
***             which handle the respective information sets of fixed channel dimensions and calibration
***             profiles. This class is responsible for the proper linkage of the organzied frames.
***             As main class, it interprets the configuration of***
***             In the following, a complete Pseudo-UML overview of the included classes is given
***
***                                      +-------------------------+
***                      +---------------|  AF4CalibSettingsFrame  | can be used
***                      |               +-------------------------+
***                      |                   🡑
***                      ⧫                  /
***                 .-,(  ),-.             / provide information about all calibration
***              .-(          )-.         /  stuff (and returns references to shared maps
***             (    External    ) <-----+   to enable choosing calibration settings and data
***              '-(          ).-'       |   outside from this class
***                  '-.( ).-            |
***                                      |
***                                      |
***                                      |
***                    +-----------------------------------+
***                    |   AF4ChannelConfigurationWidget   |◆----------------------------------+
***                    +-----------------------------------+                                   |
***                      ⧫                             🡑                                      |
***                      |                              \  triggers calibration                |
***                      |                               +------------------------+            |
***                      | 1                                                       \           | 1
***  +----------------------------+                                             +-------------------------+
***  |   AF4ChannelDimsOrgFrame   |                                             |   AF4CalibOrgFrame      |
***  |----------------------------|                                             |-------------------------|
***  |                            | ------------------------------------------> |                         |
***  |                            |   adjusts number (p) of                     |                         |
***  |      ............          |   channel-associated QMaps                  |        ............     |◆------------+
***  |----------------------------|   with AF4CalibSEttingsFrames               |-------------------------|             | 1
***  |                            |                                             |                         |   +-----------------------+
***  |                            |                                             |                         |   |   AF4CalibPlotWidget  |
***  |                            |                                             |                         |   |-----------------------|
***  |      ............          |                                             |        ............     |   |       ..........      |
***  +----------------------------+                                             +-------------------------+   |-----------------------|
***         ⧫                                                                              ⧫
***         |                                                                              |
***         | 1                                                                            | 1
***  +--------------------------------------+                          +---------------------------------------------------------+
***  |      QMap<AF4ChannelDimsWidget>      |                          |         QMap<QMap <AF4ChannelCalibWidget> >             |
***  |--------------------------------------|                          |---------------------------------------------------------|
***  | p entries                            |                          |  Σc_p  entries                                          |
***  | p = numer of channels                |                          |  c_i = number of calibrations for ith channel           |
***  |                                      |                          |                                                         |
***  |       key   | item                   |                          |  key         | item                                     |
***  |    ---------+----------------        |                          |  ------------+--------------------------------          |
***  | "channel 1" | AF4ChannelDimsWidget 1 |                          |  "channel 1" |    key       | item                      |
***  | "channel 2" | AF4ChannelDimsWidget 2 |                          |              |   -----------+-------------              |
***  |     ...     |        ...             |                          |              |  "calib 1"   | AF4ChannelCalibWidget 1   |
***  | "channel p" | AF4ChannelDimsWidget p |                          |              |  "calib 2"   | AF4ChannelCalibWidget 2   |
***  +--------------------------------------+                          |              |     ...      |         ...               |
***                                                                    |              |  "calic c_1" | AF4ChannelCalibWidget c_1 |
***                                                                    |              |              |                           |
***                                                                    |              |                                          |
***                                                                    |  "channel 2" |    key       | item                      |
***                                                                    |              |   -----------+--------------             |
***                                                                    |              |     ...      |         ...               |
***                                                                    |              |  "calib c_2" | AF4ChannelCalibWidget c_2 |
***                                                                    |              |                                          |
***                                                                    |      ...     |             ...                          |
***                                                                    |              |                                          |
***                                                                    |  "channel p" |    key       | item                      |
***                                                                    |              |   -----------+--------------             |
***                                                                    |              |     ...      |         ...               |
***                                                                    |              |  "calib c_p" |         "calib c_p"       |
***                                                                    +---------------------------------------------------------+
***
***  \author    Benedikt Häusele
***  \version   1.2
***  \date      2018-10-30
***  \todo      Put Frames into single classes (started)
***  \copyright CC CC BY-NC-ND 4.0
***
*******************************************************************************************************************************************/

class AF4ChannelConfigurationWidget final : public QWidget {

   Q_OBJECT

public:

   /*!
    * \brief FFFChannelCalConfWidget default constructor
    * \param parent parent Widget of the object
    */
   explicit AF4ChannelConfigurationWidget(QWidget *parent);

   /*!
    * \brief getChannelConfigWidgets returns a QSharedPointer to the QMap
    *        that contains the channelConfigWidgets
    * \return map of the channelConfigWidgets
    */
   auto getChannelConfigWidgets() const
   -> QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> >;

   /*!
    * \brief getChannelCalibWidgets returns a QSharedPointer to the QMap
    *        that contains the channelConfigWidgets
    */
   auto getChannelCalibWidgets() const
   -> QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > >;

//signals:
   //   void calibStuf();

private slots:

   /*!
    * \brief calibrateChannnel
    * \details creates a fffcalibrator object to calculate the channel width, depending
    *          on the parameters in the current channel and calibration profile
    */
   void calibrateChannnel();

private:
   QGridLayout            *layout              = nullptr;
   AF4ChannelDimsOrgFrame *channelDimsOrgFrame = nullptr;
   AF4CalibOrgFrame       *calibsOrgFrame      = nullptr;
   QPushButton            *settingsWriter      = nullptr;
   /*!
    * \brief calibRealMeaurement calibrates the Channel with the given Parameters and sets the values
    *                            to the widget
    * \param chDims
    * \param params
    * \param cModes
    */
   void calibRealMeaurement(const ChannelDims &chDims, const ParametersForCalibration &params, const CalibModeSettings &cModes);

   /*!
    * \brief calibUncertaintyGrid iterates over the calibration grids
    * \param chDims
    * \param params
    * \param cModes
    */
   void calibUncertaintyGrid(const ChannelDims &chDims, const ParametersForCalibration &params, const CalibModeSettings &cModes);

   /*!
    * \brief calibSingleParamSet conducts a calibration with the given Parameter set
    * \param chDims
    * \param params
    */
   CalibResult calibSingleParamSet(ChannelDims chDims, ParametersForCalibration params, CalibMode mode);

   void logErrorMessage(CalibErrorCode errorCode);

   /*!
    * \brief ~FFFChannelCalConfWidget() default destructor
    */
   ~AF4ChannelConfigurationWidget();

   NO_COPY_MOVE_CTORS(AF4ChannelConfigurationWidget)
};
#endif // AF4CHANNELCONFIGUARTIONWIDGET_H
