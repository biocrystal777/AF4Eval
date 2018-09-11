#ifndef FFFTWOBOXWIDGET_H
#define FFFTWOBOXWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <cmath>
#include "../GeneralWidgets/af4log.h"

/**************************************************************
***
***  FFFTwoBoxWidget (using Qt 4.8.2)
***
**************************************************************/

/*!
 * \brief The FFFTwoBoxWidget class Provides user input of real parameters using scientific notation
 * \details The FFFTwoBoxWidget consist of a QDoubleSpinBox to display a signifand and a QSpinBox to
 *          display an exponent.
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFTwoBoxWidget : public QWidget
{
   Q_OBJECT
public:

   /*!
     * \brief FFFTwoBoxWidget constructor of this class
     * \param toolTipString toolTip that shall be displayed for this widget
     * \param parent parent widget
     */
   explicit FFFTwoBoxWidget(const QString &toolTipString, QWidget *parent = nullptr);

    FFFTwoBoxWidget(const FFFTwoBoxWidget& src) = delete;
    FFFTwoBoxWidget& operator= (FFFTwoBoxWidget& src) = delete;
    FFFTwoBoxWidget(FFFTwoBoxWidget&& src) = delete;
    FFFTwoBoxWidget& operator= (FFFTwoBoxWidget&& src) = delete;


   /*!
    * \brief ~FFFTwoBoxWidget destructor of this class
    */
   ~FFFTwoBoxWidget();


   /*!
     * \brief value returns the current value of the widget
     * \return value of the box
     */

   double value() const;

   /*!
     * \brief setValue sets the widget to a value.
     * \param signif significand value for the QDoubleSpinBox
     * \param exp exponent for the QSpinBox
     * \param callID Debugging parameter, can be passed to
     *        identify from where the function has been called
     */
   void setValueS(double signif, int exp, int callID = 0);

   void setValueM(double value, int callID = 0);

   /*!
     * \brief setMaximum set the maximum value of the function
     *        Be careful to find the right combination
     *        of significand and exponent when determing the range
     *        of the box system
     * \param sign significand
     * \param exp exponent
     */
   void setMaximumS(double sign, int exp);

   void setMaximum(double value);

   void setMaxExp(int exp);

   /*!
     * \brief setMinimum set the minimum value of the function
     *        Be careful to find the right combination
     *        of significand and exponent when determing the range
     *        of the box system
     * \param sign significand
     * \param exp exponent
     */
   void setMinimumS(double sign, int exp);

   void setMinimum(double value);

    void setMinExp(int exp);
   /*!
     * \brief setSignifandStep set the stepsize of the significand box.
     *        The exponent box step size is always 1.
     * \param stepSize
     */
   void setSignifandStep(double stepSize);

   void setSignifandDecimals(uint number);
   /*!
     * \brief Calculates the signifand of a double p.e. 345.789 results 3.45789
     *        with an exponent of 2.
     *        The exponent can be obtained by passing a pointer to this result
     * \param number the double to be processed
     * \param exponent pass a pointer to the method to obtain the exponent of the number
     * \return the significand of the number
     **/
   static double calcSignificand(double number, int *exponent = NULL);


signals:

   /*!
     * \brief valueChanged Signal is emitted when either the
     *        significand of exponent has been changed
     */
   void valueChanged();
   void valueChanged(double val);

public slots:

   /*!
     * \brief setReadOnly set the readOnly flag of both boxes
     * \param r flag
     */
   void setReadOnly(bool r);

protected:
   QHBoxLayout *layout;
   QDoubleSpinBox* significand;
   QSpinBox *exponent;

   double minimumSign;
   int minimumExp;
   double maximumSign;
   int maximumExp;



protected slots:

   /*!
     * \brief callValueChanged slot is called when either the
     *        significand of exponent has been changed and throws
     *        the signal via valueChanged()
     * \param dummyArgument
     */
   void callValueChanged(QString dummyArgument);

   /*!
     * \brief reNormValues takes values with significand <1.0 or >=10.0 (>-1.0 or <=-10.0)
     *        and brings them to scientific format
     * \param signInput
     */
   void reNormValues(double signInput);
};

#endif // FFFTWOBOXWIDGET_H
