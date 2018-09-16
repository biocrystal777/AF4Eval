#ifndef AF4CHANNELCONFIGUARTIONWIDGET_H
#define AF4CHANNELCONFIGUARTIONWIDGET_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include "af4channelcalibwidget.h"
#include "./af4channeldimswidget.h"
#include "../../Core/af4calibrator.h"

/*! **************************************************************************************************************
***
***  \class     AF4CalibNameDialog "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief     AF4CalibNameDialog isused to delete the current channel
***             as well as its assigned calibration profiles and adapt the internal data structures
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/
class AF4CalibNameDialog final : public QDialog
{
   Q_OBJECT

public:

   /*!
    * \brief FFFCalibNameDialog constructor of this class
    * \param name pointer that has to be passed that assigns the place,
    *        where the name entered by user will be written
    * \param first if the dialog is called repeatedly, this flag can
    *        the text of the dialog
    * \param nameSuggestion default name that is shown before the user enters something
    * \param rename has to be set true if the dialog is called by the rename function. Changes
    *        the text of the dialog
    */
   explicit AF4CalibNameDialog(QString *name, bool first, const QString nameSuggestion = "", bool rename = false);

   /*!
    * \brief destructor of the class
    */
   ~AF4CalibNameDialog(){}

   AF4CalibNameDialog(const AF4CalibNameDialog& src)        = delete;
   AF4CalibNameDialog& operator= (AF4CalibNameDialog& src ) = delete;
   AF4CalibNameDialog(AF4CalibNameDialog&& src)             = delete;
   AF4CalibNameDialog& operator= (AF4CalibNameDialog&& src) = delete;

private:
   QString *calibName        = nullptr;
   QGridLayout *layout       = nullptr;
   QLineEdit *calibNameInput = nullptr;
   QPushButton *accepter     = nullptr;
   QPushButton *decliner     = nullptr;

private slots:
   /*!
    * \brief acceptName writes the name to the pointer passed for the constructor
    *        and calls accept which delivers "true" to exec()
    */
   void acceptName();
};

/*! **************************************************************************************************************
***
***  \class     AF4DeleteCalibDialog "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief     AF4DeleteCalibDialog isused to delete the current channel
***             as well as its assigned calibration profiles and adapt the internal data structures
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/
class AF4DeleteCalibDialog final : public QDialog
{
   Q_OBJECT
public:
   explicit AF4DeleteCalibDialog();
   ~AF4DeleteCalibDialog(){}

   AF4DeleteCalibDialog(const AF4DeleteCalibDialog& src)        = delete;
   AF4DeleteCalibDialog& operator= (AF4DeleteCalibDialog& src ) = delete;
   AF4DeleteCalibDialog(AF4DeleteCalibDialog&& src)             = delete;
   AF4DeleteCalibDialog& operator= (AF4DeleteCalibDialog&& src) = delete;

protected:
   QGridLayout *layout   = nullptr;
   QPushButton *accepter = nullptr;
   QPushButton *decliner = nullptr;
};


/*! **************************************************************************************************************
***
***  \class     AF4DeleteChannelDialog "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief     AF4DeleteChannelDialog isused do delete the current channel with its dimension profile
***             as well as its assigned calibration profiles and adapt the internal data structures
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/

class AF4DeleteChannelDialog final : public QDialog{

   Q_OBJECT

public:

   /*!
    * \brief FFFDeleteChannelDialog constructor of this class
    */
   explicit AF4DeleteChannelDialog();
   /*!
    * \brief FFFDeleteChannelDialog
    */
   ~AF4DeleteChannelDialog(){}
    AF4DeleteChannelDialog(const AF4DeleteChannelDialog& src) = delete;
    AF4DeleteChannelDialog& operator= (AF4DeleteChannelDialog& src) = delete;
    AF4DeleteChannelDialog(AF4DeleteChannelDialog&& src) = delete;
    AF4DeleteChannelDialog& operator= (AF4DeleteChannelDialog&& src) = delete;

protected:
   QGridLayout *layout   = nullptr;
   QPushButton *accepter = nullptr;
   QPushButton *decliner = nullptr;
};

/*! **************************************************************************************************************
***
***  \class     AF4ChannelNameDialog "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief     AF4ChannelNameDialog
***  \details   AF4ChannelNameDialog offers to enter a name for a channel***
***             If the dialog has been accepted, the name will be written to the pointer that has to be
***             passed to the constructor
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/

class AF4ChannelNameDialog : public QDialog
{
   Q_OBJECT
public:
   /*!
    * \brief FFFChannelNameDialog constructor of this class
    * \param name pointer that has to be passed that assigns the place,
    *             where the name entered by user will be written
    * \param first if the dialog is called repeatedly, this flag can
    *              the text of the dialog
    * \param nameSuggestion default name that is shown before the user enters something
    * \param rename has to be set true if the dialog is called by the rename function. Changes
    *        the text of the dialog
    */
   explicit AF4ChannelNameDialog(QString *name, bool first, const QString nameSuggestion = "", bool rename = false);

   /*!
    * \brief destructor of this class
    */
   ~AF4ChannelNameDialog(){}
   AF4ChannelNameDialog(const AF4ChannelNameDialog& src)        = delete;
   AF4ChannelNameDialog& operator= (AF4ChannelNameDialog& src ) = delete;
   AF4ChannelNameDialog(AF4ChannelNameDialog&& src)             = delete;
   AF4ChannelNameDialog& operator= (AF4ChannelNameDialog&& src) = delete;

protected:
   QString *channelName         = nullptr;
   QGridLayout *layout          = nullptr;
   QLineEdit *channelNameInput  = nullptr;
   QPushButton *accepter        = nullptr;
   QPushButton *decliner        = nullptr;

protected slots:

   /*!
    * \brief acceptName writes the name to the pointer passed for the constructor
    *        and calls accept which delivers "true" to exec()
    */
   void acceptName();
};

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
***  \version   1.0
***  \date      2018-08-31
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
    * \brief ~FFFChannelCalConfWidget() default destructor
    */
   ~AF4ChannelConfigurationWidget();

    AF4ChannelConfigurationWidget(const AF4ChannelConfigurationWidget& src)         = delete;
    AF4ChannelConfigurationWidget& operator= (AF4ChannelConfigurationWidget& src)   = delete;
    AF4ChannelConfigurationWidget(AF4ChannelConfigurationWidget&& src)             = delete;
    AF4ChannelConfigurationWidget& operator= (AF4ChannelConfigurationWidget&& src) = delete;


   /*!
    * \brief getChannelConfigWidgets returns a pointer to the QMap
    *        that contains the channelConfigWidgets of the program
    * \return map of the channelConfigWidgets
    */
   QMap<QString, AF4ChannelDimsWidget*>* getChannelConfigWidgets() const;
   QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* getChannelCalibWidgets() const;

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

private:

   QFrame *calibrationFrame                                                    = nullptr;
   QGridLayout *calibrationFrameLayout                                         = nullptr;
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
