#ifndef FFFMAINWINDOW_H
#define FFFMAINWINDOW_H

#include <QtGui>
#include <QDesktopWidget>
#include "./GeneralWidgets/af4log.h"
#include "./DiffEvaluation/af4diffevaluationwidget.h"
#include "./LightScattering/fffslsevaluationwidget.h"
#include "./Calibration/af4channeldimswidget.h"
#include "./Calibration/af4channelconfigurationwidget.h"
#include "../Core/af4csvwriter.h"

/**************************************************************
***
***  FFFMainWindow (using Qt 4.8.2)
***
**************************************************************/

#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };

/*!
 * \brief The FFFMainWindow class
 * \details The FFFMainWindow consists of a tab widget that contains all relevant
 *          Functionalities and a logWidget that can be accessed globally to log
 *          messages. If the "FFFEvaluationWidget tab" or the "FFFDiffusionCorrection Tab"
 *          is chosen by the user, all displayed parameters and comboBoxes will be updated
 *          in this tab.
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFMainWindow : public QWidget
{
   Q_OBJECT
public:

   /*!
    * \brief FFFMainWindow constructor of this class
    * \param parent parent widget
    */
   explicit FFFMainWindow(QWidget *parent = nullptr);

   /*!
    * ~FFFMainWindow destructor of this class
    */
   ~FFFMainWindow();

    FFFMainWindow(const FFFMainWindow& src) = delete;
    FFFMainWindow& operator= (FFFMainWindow& src) = delete;
    FFFMainWindow(FFFMainWindow&& src) = delete;
    FFFMainWindow& operator= (FFFMainWindow&& src) = delete;

protected:

   QGridLayout *layout;

   ////////////////////////////
   // Functionality Widgets: //
   ////////////////////////////

   QTabWidget *funcTabWidget;
   QGridLayout *funcLayout;

   AF4ChannelConfigurationWidget *channelCalConfWidget;
   AF4DiffEvaluationWidget *diffEvaluationWidget;


   FFFSLSEvaluationWidget *slsEvaluationWidget;


   int lastChosenTab;

   ////////////
   // Frames //
   ////////////

   QFrame *logFrame;
   QGridLayout *logFrameLayout;
   AF4Log *logDevice;

   /////////////////////////
   // General class tools //
   /////////////////////////

protected:

   /*!
    * \brief writeSettings write parameter to QSettings
    */
   void writeSettings() const;
};

#endif // FFFMAINWINDOW_H
