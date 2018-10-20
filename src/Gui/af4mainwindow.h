#ifndef AF4MAINWINDOW_H
#define AF4MAINWINDOW_H

#include <QtGui>
#include <QDesktopWidget>
#include "./GeneralWidgets/af4log.h"
#include "./DiffEvaluation/af4diffevaluationwidget.h"
#include "./LightScattering/af4slsevaluationwidget.h"
#include "./Calibration/af4channeldimswidget.h"
#include "./Calibration/af4channelconfigurationwidget.h"
#include "../Core/af4csvwriter.h"
#include "../Core/af4evalmacros.h"

/*! **********************************************************************************************
***
***  \class     AF4MainWindow "src/Gui/smallQDerivates/af4mainwindow.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-10-02
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4MainWindow : public QWidget
{
   Q_OBJECT
public:

   /*!
    * \brief FFFMainWindow constructor of this class
    * \param parent parent widget
    */
   explicit AF4MainWindow(QWidget *parent = nullptr);

   /*!
    * ~FFFMainWindow destructor of this class
    */
   ~AF4MainWindow();

private:

   //-//////////////////////
   // General class tools //
   //-//////////////////////
   /*!
    * \brief writeSettings write parameter to QSettings
    */
   void writeSettings() const;

   QGridLayout *layout         = nullptr;

   //-/////////
   // Frames //
   //-/////////

   QFrame      *logFrame       = nullptr;
   QGridLayout *logFrameLayout = nullptr;
   AF4Log      *logDevice      = nullptr;

   //-/////////////////////////
   // Functionality Widgets: //
   //-/////////////////////////

   QTabWidget                    *funcTabWidget        = nullptr;
   QGridLayout                   *funcLayout           = nullptr;
   AF4ChannelConfigurationWidget *channelCalConfWidget = nullptr;
   AF4DiffEvaluationWidget       *diffEvaluationWidget = nullptr;
   AF4SLSEvaluationWidget        *slsEvaluationWidget  = nullptr;
   //int lastChosenTab;

   NO_COPY_ASSIGNMENT_CTORS(AF4MainWindow)
};

#endif // AF4MAINWINDOW_H
