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
***  \brief     AF4StokesEinsteinCalculatorWidget Small GUI for quick estimations of Dvia classical
***                                               Stokes Einstein equation
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4StokesEinsteinCalculatorWidget final : public QDialog
{
   Q_OBJECT
public:
   explicit AF4StokesEinsteinCalculatorWidget(QWidget *parent);

   ~AF4StokesEinsteinCalculatorWidget();

private:

   QGridLayout *lay              = nullptr;
   QDoubleSpinBox *stokesRad     = nullptr;
   QDoubleSpinBox *temperature   = nullptr;
   QDoubleSpinBox *viscosity     = nullptr;
   QLineEdit *diffCoefficient    = nullptr;

   /*!
    * \brief loadParameters
    */
   void loadParameters();

   /*!
    * \brief saveParameters
    */
   void saveParameters();

   AF4StokesEinsteinCalculatorWidget(const AF4StokesEinsteinCalculatorWidget& src) = delete;
   AF4StokesEinsteinCalculatorWidget& operator= (AF4StokesEinsteinCalculatorWidget& src) = delete;
   AF4StokesEinsteinCalculatorWidget(AF4StokesEinsteinCalculatorWidget&& src) = delete;
   AF4StokesEinsteinCalculatorWidget& operator= (AF4StokesEinsteinCalculatorWidget&& src) = delete;
private slots:

   /*!
    * \brief reCalculate
    */
   void reCalculate();

};

#endif // AF4STOKESEINSTEINCALCULATORWIDGET_H
