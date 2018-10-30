#ifndef AF4CHANNELCONFIGUARTIONWIDGET_H
#define AF4CHANNELCONFIGUARTIONWIDGET_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include "./af4caliborgframe.h"
#include "../Core/af4evalmacros.h"
#include "./af4channeldimsorgframe.h"

/*! **************************************************************************************************************
***
***  \class     AF4ChannelConfigurationWidget "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief     master class for calibration functions*
***
***  \details   AF4ChannelConfigurationWidget coordinates two frames (channelDimsOrgFrame and calibsOrgFrame)
***             which handle the respective information sets of fixed channel dimensions and calibration
***             profiles. This class is responsible for the proper linkage of the organzied frames.
***             As main class, it interprets the configuration of***
***             In the following, a complete overview of the included classes is given
***
***
***                 .-,(  ),-.
***              .-(          )-.
***             (    External    ) <-----+
***              '-(          ).-'       | provide information about all calibration
***                  '-.( ).-            | stuff and return references
***                                      |
***                                      |
***                                      |
***                    +---------------------------------- +
***                    |   AF4ChannelConfigurationWidget   |◆-------------------------------------+
***                    +---------------------------------- +                                      |
***                      ⧫                             🡑                                         |
***                      |                              \                                         |
***                      |                               \                                        |
***                      |                                \  triggers calibration                 |
***                      |                                 +---------------------+                |
***                      |                                                        \               |
***                      |                                                         \              |
***                      |                                                          \             |
***                      |                                                           \            |
***                      | 1                                                          \           | 1
***  +----------------------------+                                                +----------------------------+
***  |   AF4ChannelDimsOrgFrame   |                                                |   AF4ChannelDimsOrgFrame   |
***  |----------------------------|                                                |----------------------------|
***  |                            | ---------------------------------------------> |                            |
***  |                            |   adjusts number of channel-associated         |                            |
***  |      ............          |                                                |        ............        |
***  |----------------------------|                                                |----------------------------|
***  |                            |                                                |                            |
***  |                            |                                                |                            |
***  |                            |                                                |                            |
***  |      ............          |                                                |         ............       |
***  +----------------------------+                                                +----------------------------+
***    ⧫
***    |
***    | 1
***  +----------------------------+
***  | QMap<AF4ChannelDimsWidget> |
***  |----------------------------+
***  | p entries ()               |
***  | p = numer of channels      |
***  +----------------------------+
***
***
***
***
***
***
***
***
***
***
***
***
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-08-31
***  \todo      Put Frames into single classes (started)
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/

class AF4ChannelConfigurationWidget final : public QWidget {

   Q_OBJECT

public:

   /*!
    * \brief FFFChannelCalConfWidget default constructor
    * \param parent parent Widget of the object
    */
   explicit AF4ChannelConfigurationWidget(QWidget *parent);

   /*!
    * \brief getChannelConfigWidgets returns a pointer to the QMap
    *        that contains the channelConfigWidgets of the program
    * \return map of the channelConfigWidgets
    */
   auto getChannelConfigWidgets() const -> QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> >   {
      return channelDimsOrgFrame->getChannelConfigWidgets();
   }
   /*!
    * \brief getChannelCalibWidgets
    */
   auto getChannelCalibWidgets()  const -> QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > {
      return calibsOrgFrame->getChannelCalibWidgets();
   }

private:




private slots:

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;


   /*!
    * \brief calibrateChannnel
    * \details creates a fffcalibrator object to calculate the channel width, depending
    *          on the parameters in the current channel and calibration profile
    */
   void calibrateChannnel();

private:
   QGridLayout                *layout              = nullptr;
   AF4ChannelDimsOrgFrame     *channelDimsOrgFrame = nullptr;
   AF4CalibOrgFrame           *calibsOrgFrame      = nullptr;
   QSharedPointer<QPushButton> settingsWriter;  // replace shared reference by signal slot later!
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
    * \brief connectCtrlWithPlotWidget
    */
   //void connectCtrlWithPlotWidget();

   /*!
    * \brief writeSettings writes Parameters to QSettings
    */
   void writeSettings() const;

   /*!
    * \brief ~FFFChannelCalConfWidget() default destructor
    */
   ~AF4ChannelConfigurationWidget();

   NO_COPY_ASSIGNMENT_CTORS(AF4ChannelConfigurationWidget)
};
#endif // AF4CHANNELCONFIGUARTIONWIDGET_H
