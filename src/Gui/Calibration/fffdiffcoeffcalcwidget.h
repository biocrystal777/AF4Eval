#ifndef FFFDIFFCOEFFCALCWIDGET_H
#define FFFDIFFCOEFFCALCWIDGET_H

#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <qwt_text_label.h>
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../../Core/fffconstants.h"



class FFFDiffCoeffCalcWidget : public QDialog
{
   Q_OBJECT
public:
   explicit FFFDiffCoeffCalcWidget(QWidget *parent = 0);

   ~FFFDiffCoeffCalcWidget();

   FFFDiffCoeffCalcWidget(const FFFDiffCoeffCalcWidget& src) = delete;
   FFFDiffCoeffCalcWidget& operator= (FFFDiffCoeffCalcWidget& src) = delete;
   FFFDiffCoeffCalcWidget(FFFDiffCoeffCalcWidget&& src) = delete;
   FFFDiffCoeffCalcWidget& operator= (FFFDiffCoeffCalcWidget&& src) = delete;

private:

   QGridLayout *lay              = nullptr;
   QDoubleSpinBox *stokesRad     = nullptr;
   QDoubleSpinBox *temperature   = nullptr;
   QDoubleSpinBox *viscosity     = nullptr;
   QLineEdit *diffCoefficient    = nullptr;
   void loadParameters();
   void saveParameters();

signals:

private slots:

   void reCalculate(double dummy);

};

#endif // FFFDIFFCOEFFCALCWIDGET_H
