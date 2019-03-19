#ifndef AF4CALIBRATIONDIALOGS_H
#define AF4CALIBRATIONDIALOGS_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include "../Core/af4evalmacros.h"

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
    * \brief AF4CalibNameDialog constructor of this class
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

private:
   QString *calibName        = nullptr;
   QGridLayout *layout       = nullptr;
   QLineEdit *calibNameInput = nullptr;
   QPushButton *accepter     = nullptr;
   QPushButton *decliner     = nullptr;

   NO_COPY_ASSIGNMENT_CTORS(AF4CalibNameDialog)
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

private:
   QGridLayout *layout   = nullptr;
   QPushButton *accepter = nullptr;
   QPushButton *decliner = nullptr;

   NO_COPY_ASSIGNMENT_CTORS(AF4DeleteCalibDialog)
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
   AF4DeleteChannelDialog();
   /*!
    * \brief FFFDeleteChannelDialog
    */
   ~AF4DeleteChannelDialog(){}

private:
   QGridLayout *layout   = nullptr;
   QPushButton *accepter = nullptr;
   QPushButton *decliner = nullptr;

   NO_COPY_ASSIGNMENT_CTORS(AF4DeleteChannelDialog)
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

class AF4ChannelNameDialog final : public QDialog
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
   AF4ChannelNameDialog(QString* const name, bool first, const QString nameSuggestion = "", bool rename = false);

   /*!
    * \brief destructor of this class
    */
   ~AF4ChannelNameDialog(){}

private:
   QString * const channelName;
   QGridLayout *layout          = nullptr;
   QLineEdit *channelNameInput  = nullptr;
   QPushButton *accepter        = nullptr;
   QPushButton *decliner        = nullptr;

   NO_COPY_ASSIGNMENT_CTORS(AF4ChannelNameDialog)
};
#endif // AF4CALIBRATIONDIALOGS_H
