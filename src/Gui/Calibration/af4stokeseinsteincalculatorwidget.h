#ifndef AF4STOKESEINSTEINCALCULATORWIDGET_H
#define AF4STOKESEINSTEINCALCULATORWIDGET_H

#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <qwt_text_label.h>
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../../Core/af4constants.h"

/*! **********************************************************************************************
***
***  \class     AF4StokesEinsteinCalculatorWidget "src/Gui/Calibration/af4channeldimswidget.h"
***  \brief     AF4StokesEinsteinCalculatorWidget Small GUI for quick estimations of D via classical
***                                               Stokes Einstein equation for calibration purposes
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-10-09
***  \copyright LGPL
***
*************************************************************************************************/

class AF4StokesEinsteinCalculatorWidget final : public QDialog
{
   Q_OBJECT
public:
   /*!
    * \brief AF4StokesEinsteinCalculatorWidget
    * \param parent
    */
   explicit AF4StokesEinsteinCalculatorWidget(QWidget *parent);

   /*!
    * \brief
    */
   ~AF4StokesEinsteinCalculatorWidget();

signals:

   void newDiffCoeff(double D);


private slots:

   /*!
    * \brief reCalculate with D with Stokes-Einstein equation
    */
   void reCalculate();

private:
   /*!
    * \brief loadParameters
    */
   void loadParameters();

   /*!
    * \brief saveParameters
    */
   void saveParameters();

   QGridLayout      *lay             = nullptr;
   QDoubleSpinBox   *stokesRad       = nullptr;
   QDoubleSpinBox   *temperature     = nullptr;
   QDoubleSpinBox   *viscosity       = nullptr;
   QFrame           *dFrame          = nullptr;
   QHBoxLayout      *frameLay        = nullptr;
   AF4SciNotSpinBox *diffCoefficient = nullptr;
   QPushButton *accepter             = nullptr;
   QPushButton *rejecter             = nullptr;

   AF4StokesEinsteinCalculatorWidget(const AF4StokesEinsteinCalculatorWidget& src) = delete;
   AF4StokesEinsteinCalculatorWidget& operator= (AF4StokesEinsteinCalculatorWidget& src) = delete;
   AF4StokesEinsteinCalculatorWidget(AF4StokesEinsteinCalculatorWidget&& src) = delete;
   AF4StokesEinsteinCalculatorWidget& operator= (AF4StokesEinsteinCalculatorWidget&& src) = delete;
};

#endif // AF4STOKESEINSTEINCALCULATORWIDGET_H
