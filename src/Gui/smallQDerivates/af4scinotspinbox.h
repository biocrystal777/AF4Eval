#ifndef FFFTWOBOXWIDGET_H
#define FFFTWOBOXWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <cmath>
#include <QDebug>

/*! **********************************************************************************************
***
***  \class     AF4SciNotSpinBox "src/Gui/smallQDerivates/af4scinotspinbox.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-09-24
***  \copyright LGPL 3.0
***  \todo      Extend for negative values, make widget size constant; documentation incomplete
***
*************************************************************************************************/

class AF4SciNotSpinBox : public QWidget
{
   Q_OBJECT
public:

   /*!
    * \brief FFFTwoBoxWidget constructor of this class
    * \param toolTipString toolTip that shall be displayed for this widget
    * \param parent parent widget
    */
   explicit AF4SciNotSpinBox(const bool signedBox = false, QWidget *parent = nullptr);

   AF4SciNotSpinBox(const AF4SciNotSpinBox& src) = delete;
   AF4SciNotSpinBox& operator= (AF4SciNotSpinBox& src) = delete;
   AF4SciNotSpinBox(AF4SciNotSpinBox&& src) = delete;
   AF4SciNotSpinBox& operator= (AF4SciNotSpinBox&& src) = delete;

   /*!
    * \brief ~FFFTwoBoxWidget destructor of this class
    */
   ~AF4SciNotSpinBox(){}

   /*!
     * \brief value returns the current value of the widget
     * \return value of the box
     */
   double value() const { return valAsDouble(significand->value(), exponent->value());}

   /*!
    * \brief setValue
    * \param value
    * \param ok
    * \param callID
    */
   void setValue(double value, bool *ok = nullptr);

   /*!
    * \brief setMaximum
    * \param value
    */
   void setMaximum(double value, bool *ok = nullptr);
   /*!
    * \brief getMaximum
    * \return
    */
   double maximum() const { return maxSig * pow10(maxExp); }
   /*!
    * \brief setMinimum
    * \param value
    */
   void setMinimum(double value, bool *ok = nullptr);

   /*!
    * \brief getMinimum
    * \return
    */
   double minimum() const { return minSig * pow10(minExp); }

   /*!
     * \brief setSignifandStep set the stepsize of the significand box.
     *        The exponent box step size is always 1.
     * \param stepSize between 1e-9 and 1e9
     */
   void setSignifandSingleStep(double stepSize, bool *ok = nullptr);

   double significandSingleStep() const { return significand->singleStep();}
   /*!
    * \brief setSignifandDecimals
    * \param number; max. Precision = 10
    */
   void setSignifandDecimals(int number);


   int significandDecimals() const { return significand->decimals(); }

   /*!
     * \brief Calculates the signifand of a double p.e. 345.789 results 3.45789
     *        with an exponent of 2.
     *        The exponent can be obtained by passing a pointer to this result
     * \param number the double to be processed
     * \param exponent pass a pointer to the method to obtain the exponent of the number
     * \return the significand of the number
     * \todo  structured bindings for C++17
     **/
   double calcSignificand(double number, int *exponent = nullptr);

signals:

   /*!
     * \brief valueChanged Signal is emitted when either the
     *        significand of exponent has been changed
     */
   void valueChanged();

   /*!
     * \brief valueChanged Signal is emitted when either the
     *        significand of exponent has been changed
    */
   void valueChanged(double val);

public slots:

   /*!
     * \brief setReadOnly set the readOnly flag of both boxes
     * \param r flag
     */
   void setReadOnly(bool r);

protected:

   /*!
     * \brief setValue sets the widget to a value.
     * \param sig significand value for the QDoubleSpinBox
     * \param exp exponent for the QSpinBox
     * \param callID Debugging parameter, can be passed to
     *        identify from where the function has been called
     */
   void setValueIntern(double sig, int exp);

   /*!
     * \brief setMaximum set the maximum value of the function
     *        Be careful to find the right combination
     *        of significand and exponent when determing the range
     *        of the box system
     * \param sig significand
     * \param exp exponent
     */
   void setMaximumIntern(double sig, int exp, bool *ok = nullptr);

   /*!
     * \brief  setMinimum set the minimum value of the function
     * \detail Be careful to find the right combination
     *         of significand and exponent when determing the range
     *         of the box system
     * \param  sig significand
     * \param  exp exponent
     */
   void setMinimumIntern(double sig, int exp, bool *ok = nullptr);

   /*!
    * \brief setMaxExp
    * \param exp
    */
   void setMaxExp(int exp) { this->exponent->setMaximum(exp); }
   /*!
    * \brief setMinExp
    * \param exp
    */
   void setMinExp(int exp) {this->exponent->setMinimum(exp); }

   /*!
    * \brief pow10 returns the power to the base of 10
    * \param exp
    * \return
    */
   double pow10(int exp) const { return std::pow(10.0, static_cast<double>(exp)); }


   double maxSigForDec() const { return 10.0 - pow10(-significandDecimals() ); }

   /*!
    * \brief valAsDouble
    * \param sig
    * \param exp
    * \return
    */
   double valAsDouble(double sig, int exp) const { return sig * pow10(exp); }

   /*!
    * \brief adjustInputToFormat
    */
   void adjustInputToFormat();
   /*!
    * \brief adjustSpinBoxLimits
    */
   void adjustSpinBoxLimits();

   const bool signedBox = false; // always false at the moment
   QHBoxLayout    *layout      = nullptr;
   QDoubleSpinBox *significand = nullptr;
   QSpinBox       *exponent    = nullptr;
   QLabel         *eLabel      = nullptr;
   const int    constMinExpLimit  = -100;
   const int    constMaxExpLimit  = 100;
   const double constMaxDispSig   = 10000.0;
   const int    constMinPrecision  = 10;
   const double constMaxStepSize  = 2.0;

   double minSig = 1.0;
   int    minExp  = -10;
   double maxSig = 9.99999999999999999999999;
   int    maxExp  = 10;
};

#endif // FFFTWOBOXWIDGET_H
