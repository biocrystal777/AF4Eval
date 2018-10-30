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
***  \brief
***  \details   FFChannelCalConfWidget contains space for a fffchannelconfigwidget and
***             a fffchannelcalibwidget; Each fffchannelconfigwidget represents a FFF channel profile
***             and its specific properties. A comboBox is used to switch between the different channels.
***             Each channel can have a various number of calibration profiles depending on the measurements,
***             choosable by a comboBox as well. The information about all existing channels and calibrations
***             is stored on QMaps. Pointers to those QMaps are used to update widgets in other tabs when
***             the user adds, renames or deletes channels and calibration profiles.
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

   //\////////////////
   // channel Frame //
   //\////////////////

private:

   QGridLayout *layout                                          = nullptr;
   AF4ChannelDimsOrgFrame *channelDimsOrgFrame = nullptr;
   //QGridLayout *channelConfigFrameLayout                        = nullptr;
   //QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets;//  = nullptr;
   AF4ChannelDimsWidget *currentChConfigWidget                  = nullptr;
   //QSharedPointer<QComboBox> channelSelection;
   //QToolButton *addChButton                                     = nullptr;
   //QToolButton *renameChButton                                  = nullptr;
   //QToolButton *deleteChButton                                  = nullptr;


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
   AF4CalibOrgFrame *calibsOrgFrame    = nullptr;

   QSharedPointer<QPushButton> settingsWriter;
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
