#ifndef AF4CHANNELCONFIGUARTIONWIDGET_H
#define AF4CHANNELCONFIGUARTIONWIDGET_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include "af4calibrationdialogs.h"
#include "./af4calibplotwidget.h"
#include "./af4channelcalibwidget.h"
#include "./a.out.h"
#include "./af4channeldimswidget.h"
#include "../../Core/af4calibrator.h"


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
***  \todo      Put Frames into single classes
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
   auto getChannelConfigWidgets() const -> QMap<QString, AF4ChannelDimsWidget*>*   {
      return channelConfigWidgets;
   }
   /*!
    * \brief getChannelCalibWidgets
    */
   auto getChannelCalibWidgets()  const -> QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* {
      return channelCalibWidgets;
   }

   //\////////////////
   // channel Frame //
   //\////////////////



private:

   QGridLayout *layout                                          = nullptr;
   QFrame *channelConfigFrame                                   = nullptr;
   QGridLayout *channelConfigFrameLayout                        = nullptr;
   QMap<QString, AF4ChannelDimsWidget*> *channelConfigWidgets   = nullptr;
   AF4ChannelDimsWidget *currentChConfigWidget                  = nullptr;
   QComboBox *channelSelection                                  = nullptr;
   QToolButton *addChButton                                     = nullptr;
   QToolButton *renameChButton                                  = nullptr;
   QToolButton *deleteChButton                                  = nullptr;

   /*!
    * \brief adaptConfigWidgetIds
    */
   void adaptConfigWidgetIds();

   /*!
    * \brief adaptConfigWidgetNames
    */
   void adaptConfigWidgetNames();

   /*!
    * \brief askChannelRenaming
    * \param newName
    * \param oldName
    * \return
    */
   bool askChannelRenaming(QString &newName, const QString &oldName);

   /*!
    * \brief askChannelAdding
    * \param newName
    * \return
    */
   bool askChannelAdding(QString &newName);

private slots:

 // have to be extended with calibration meta settings!
   /*!
    * \brief renameChannel creates a dialog for renaming a channel.
    *        Adapts keys of internal data structures
    */
   void renameChannel();

   /*!
    * \brief addCalibration creates a dialog and a new channel.
    *        Adapts internal data structures.
    */
   bool addChannel();

   /*!
    * \brief deleteCalibration removes the current channel
    *        and its assigned calibration profiles. Adapts
    *        internal data structures.
    */
   void deleteChannel();

   /*!
    * \brief switchCalibWidget switches to another channel and
    *        shows its values. Shows a comboBox with its assigned
    *        calibration profiles as well as the corresponding
    *        FFFChannelcalibWidgets
    * \param newWidgetKey
    */
   void switchChannelWidget(QString newWidgetKey);

   //\////////////////////
   // calibration Frame //
   //\////////////////////

public slots:
   void adaptPlotData();

private:

   QFrame *calibrationFrame                                                    = nullptr;
   QGridLayout *calibrationFrameLayout                                         = nullptr;
   //QFrame calibrationFrame = nullptr;
   AF4CalibPlotWidget *plotWidget                                              = nullptr;
   QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* channelCalibWidgets  = nullptr;   
   AF4ChannelCalibWidget *currentCalibWidget                                   = nullptr;
   QMap<QString, QComboBox*>* allCalibSelections                               = nullptr;   
   QComboBox *currentCalibSelection                                            = nullptr;
   QToolButton *addCalibButton                                                 = nullptr;
   QToolButton *renameCalibButton                                              = nullptr;
   QToolButton *deleteCalibButton                                              = nullptr;
   QPushButton *settingsWriter                                                 = nullptr;

   /*!
    * \brief adaptCalibWidgetIds
    * \param channelName
    * \param newChannelId
    * \param caller
    */
   void adaptCalibWidgetIds(const QString &channelName, int newChannelId = -1);

   /*!
    * \brief adaptCalibWidgetNames
    * \param channelName
    */
   void adaptCalibWidgetNames(const QString &channelName);

   /*!
    * \brief askCalibRenaming
    * \param newName
    * \param oldName
    * \return
    */
   bool askCalibRenaming(QString &newName, const QString &oldName);

   /*!
    * \brief askCalibAdding
    * \param newName
    * \return
    */
   bool askCalibAdding(QString &newName);

   /*!
    * \brief ~FFFChannelCalConfWidget() default destructor
    */
   ~AF4ChannelConfigurationWidget();

    AF4ChannelConfigurationWidget(const AF4ChannelConfigurationWidget& src)        = delete;
    AF4ChannelConfigurationWidget& operator= (AF4ChannelConfigurationWidget& src)  = delete;
    AF4ChannelConfigurationWidget(AF4ChannelConfigurationWidget&& src)             = delete;
    AF4ChannelConfigurationWidget& operator= (AF4ChannelConfigurationWidget&& src) = delete;

private slots:

   /*!
    * \brief renameCalibration creates a dialog for renaming a calibration profile.
    *        Adapts keys of internal data structures
    */
   void renameCalibration();

   /*!
    * \brief addCalibration creates a dialog and a new calibration profile
    *        Adapts internal data structures
    */
   bool addCalibration();

   /*!
    * \brief deleteCalibration removes the current calibration profile and
    *        adapts internal data structures
    */
   void deleteCalibration();

   /*!
    * \brief switchCalibWidget switches to another calibration profile and
    *        shows its values
    * \param newWidgetKey
    */
   void switchCalibWidget(QString newWidgetKey);

   /*!
    * \brief calibrateChannnel
    * \details creates a fffcalibrator object to calculate the channel width, depending
    *          on the parameters in the current channel and calibration profile
    */
   void calibrateChannnel();

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;

   //*************
   // utilities **
   //*************

private:
   /*!
    * \brief connectCtrlWithPlotWidget
    */
   void connectCtrlWithPlotWidget();

   /*!
    * \brief writeSettings writes Parameters to QSettings
    */
   void writeSettings() const;

   /*!
    * \brief chopStringsQuotMarksToOne if a string has more than one Quotation mark, they are merged
    * \example """this string"""   =>    "this string"
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksToOne(QString string);

   /*!
    * \brief chopStringsQuotMarksEntirely removes quotation marks of strings
    * \example """this string"""   =>    this string
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksEntirely(QString string);
};
#endif // AF4CHANNELCONFIGUARTIONWIDGET_H
