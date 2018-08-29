#ifndef FFFCHANNELCALCONFWIDGET_H
#define FFFCHANNELCALCONFWIDGET_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include "fffchannelcalibwidget.h"
#include "./fffchannelconfigwidget.h"
#include "../../Core/fffcalibrator.h"

/**************************************************************
***
***  FFFChannelCalConfWidget, module consisting of 5 classes
***                  (using Qt 4.8.2)
***
**************************************************************/

// ================================ Utilities ==============================

/**************************************************************
***
***  FFFCalibNameDialog (utility dialog)
***           (using Qt 4.8.2)
**************************************************************/
/*!
 * \brief The FFFCalibNameDialog class is used to create a dialog to enter calibration names
 * \details The Dialog offers to enter a name for a
 *          If the dialog has been accepted, the name will be written to the pointer that has to be
 *          passed to the constructor.
 *
 * \author Benedikt Haeusele
 * \version 1.0
 * \
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */
class FFFCalibNameDialog : public QDialog
{
   Q_OBJECT

public:

   /*!
    * \brief FFFCalibNameDialog constructor of this class
    * \param name pointer that has to be passed that assigns the place,
    *             where the name entered by user will be written
    * \param first if the dialog is called repeatedly, this flag can
    *              the text of the dialog
    * \param nameSuggestion default name that is shown before the user enters something
    * \param rename has to be set true if the dialog is called by the rename function. Changes
    *        the text of the dialog
    */
   explicit FFFCalibNameDialog(QString *name, bool first, const QString nameSuggestion = "", bool rename = false);

   /*!
    * \brief destructor of the class
    */
   ~FFFCalibNameDialog(){}

   FFFCalibNameDialog(const FFFCalibNameDialog& src)        = delete;
   FFFCalibNameDialog& operator= (FFFCalibNameDialog& src ) = delete;
   FFFCalibNameDialog(FFFCalibNameDialog&& src)             = delete;
   FFFCalibNameDialog& operator= (FFFCalibNameDialog&& src) = delete;

protected:
   QString *calibName        = nullptr;
   QGridLayout *layout       = nullptr;
   QLineEdit *calibNameInput = nullptr;
   QPushButton *accepter     = nullptr;
   QPushButton *decliner     = nullptr;

protected slots:
   /*!
    * \brief acceptName writes the name to the pointer passed for the constructor
    *        and calls accept which delivers "true" to exec()
    */
   void acceptName();
};


/**************************************************************
***
***  FFFDeleteCalibDialog (utility dialog)
***
**************************************************************/
/*!
 * \brief The FFFDeleteCalibDialog is used do delete the current calibration
 *        profile and adapt the internal data structures
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */
class FFFDeleteCalibDialog : public QDialog
{
   Q_OBJECT
public:
   explicit FFFDeleteCalibDialog();
   ~FFFDeleteCalibDialog(){}

   FFFDeleteCalibDialog(const FFFDeleteCalibDialog& src)        = delete;
   FFFDeleteCalibDialog& operator= (FFFDeleteCalibDialog& src ) = delete;
   FFFDeleteCalibDialog(FFFDeleteCalibDialog&& src)             = delete;
   FFFDeleteCalibDialog& operator= (FFFDeleteCalibDialog&& src) = delete;

protected:
   QGridLayout *layout   = nullptr;
   QPushButton *accepter = nullptr;
   QPushButton *decliner = nullptr;
};

/**************************************************************
***
***  FFFChannelNameDialog (utility dialog)
***
**************************************************************/
/*!
 * \brief The FFFChannelNameDialog class is used to create a dialog to enter channel names
 * \details The Dialog offers to enter a name for a channel
 *          If the dialog has been accepted, the name will be written to the pointer that has to be
 *          passed to the constructor
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */
class FFFChannelNameDialog : public QDialog
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
   explicit FFFChannelNameDialog(QString *name, bool first, const QString nameSuggestion = "", bool rename = false);

   /*!
    * \brief destructor of this class
    */
   ~FFFChannelNameDialog(){}
   FFFChannelNameDialog(const FFFChannelNameDialog& src)        = delete;
   FFFChannelNameDialog& operator= (FFFChannelNameDialog& src ) = delete;
   FFFChannelNameDialog(FFFChannelNameDialog&& src)             = delete;
   FFFChannelNameDialog& operator= (FFFChannelNameDialog&& src) = delete;

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


/**************************************************************
***
***  FFFDeleteChannelDialog (utility dialog)
***
**************************************************************/
/*!
 * \brief The FFFDeleteChannelDialog is used do delete the current channel
 *        as well as its assigned calibration profiles and adapt the internal data structures
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */
class FFFDeleteChannelDialog : public QDialog{

   Q_OBJECT

public:

   /*!
    * \brief FFFDeleteChannelDialog constructor of this class
    */
   explicit FFFDeleteChannelDialog();
   /*!
    * \brief FFFDeleteChannelDialog
    */
   ~FFFDeleteChannelDialog(){}
    FFFDeleteChannelDialog(const FFFDeleteChannelDialog& src) = delete;
    FFFDeleteChannelDialog& operator= (FFFDeleteChannelDialog& src) = delete;
    FFFDeleteChannelDialog(FFFDeleteChannelDialog&& src) = delete;
    FFFDeleteChannelDialog& operator= (FFFDeleteChannelDialog&& src) = delete;



protected:
   QGridLayout *layout   = nullptr;
   QPushButton *accepter = nullptr;
   QPushButton *decliner = nullptr;
};

// ================================ Main class ==============================

/**************************************************************
***
***  FFFChannelCalConfWidget (main widget of this module)
***
**************************************************************/
/*!
 * \brief The FFFChannelCalConfWidget class
 * \details The FFFChannelCalConfWidget contains space for a fffchannelconfigwidget and
 *          a fffchannelcalibwidget; Each fffchannelconfigwidget represents a FFF channel profile
 *          and its specific properties. A comboBox is used to switch between the different channels.
 *          Each channel can have a various number of calibration profiles depending on the measurements,
 *          choosable by a comboBox as well. The information about all existing channels and calibrations
 *          is stored on QMaps. Pointers to those QMaps are used to update widgets in other tabs when
 *          the user adds, renames or deletes channels and calibration profiles.
 *
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFChannelCalConfWidget : public QWidget{

   Q_OBJECT

public:

   /*!
    * \brief FFFChannelCalConfWidget default constructor
    * \param parent parent Widget of the object
    */
   explicit FFFChannelCalConfWidget(QWidget *parent);

   /*!
    * \brief ~FFFChannelCalConfWidget() default destructor
    */
   ~FFFChannelCalConfWidget();

    FFFChannelCalConfWidget(const FFFDeleteChannelDialog& src)         = delete;
    FFFChannelCalConfWidget& operator= (FFFDeleteChannelDialog& src)   = delete;
    FFFChannelCalConfWidget(FFFChannelCalConfWidget&& src)             = delete;
    FFFChannelCalConfWidget& operator= (FFFChannelCalConfWidget&& src) = delete;


   /*!
    * \brief getChannelConfigWidgets returns a pointer to the QMap
    *        that contains the channelConfigWidgets of the program
    * \return map of the channelConfigWidgets
    */
   QMap<QString, FFFChannelConfigWidget*>* getChannelConfigWidgets() const;
   QMap<QString, QMap<QString, FFFChannelCalibWidget*>*>* getChannelCalibWidgets() const;

   ///////////////////
   // channel Frame //
   ///////////////////

private:

   QGridLayout *layout                                          = nullptr;
   QFrame *channelConfigFrame                                   = nullptr;
   QGridLayout *channelConfigFrameLayout                        = nullptr;
   QMap<QString, FFFChannelConfigWidget*> *channelConfigWidgets = nullptr;
   FFFChannelConfigWidget *currentChConfigWidget                = nullptr;
   QComboBox *channelSelection                                  = nullptr;
   QToolButton *addChButton                                     = nullptr;
   QToolButton *renameChButton                                  = nullptr;
   QToolButton *deleteChButton                                  = nullptr;
   void adaptConfigWidgetIds();
   void adaptConfigWidgetNames();
   bool askChannelRenaming(QString &newName, const QString &oldName);
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

   ///////////////////////
   // calibration Frame //
   ///////////////////////

private:

   QFrame *calibrationFrame                                                    = nullptr;
   QGridLayout *calibrationFrameLayout                                         = nullptr;
   QMap<QString, QMap<QString, FFFChannelCalibWidget*>*>* channelCalibWidgets  = nullptr;
   FFFChannelCalibWidget *currentCalibWidget                                   = nullptr;
   QMap<QString, QComboBox*>* allCalibSelections                               = nullptr;
   QComboBox *currentCalibSelection                                            = nullptr;
   QToolButton *addCalibButton                                                 = nullptr;
   QToolButton *renameCalibButton                                              = nullptr;
   QToolButton *deleteCalibButton                                              = nullptr;
   QPushButton *settingsWriter                                                 = nullptr;
   void adaptCalibWidgetIds(const QString &channelName, int newChannelId = -1, const QString caller = "no");
   void adaptCalibWidgetNames(const QString &channelName);

   bool askCalibRenaming(QString &newName, const QString &oldName);
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

   void saveParameters() const;
   ///////////////
   // utilities //
   ///////////////

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
#endif // FFFChannelCalConfWidget_H
