#ifndef AF4CALIBPARAMETERSFRAME_H
#define AF4CALIBPARAMETERSFRAME_H
#include <QFrame>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QToolButton>
#include <QGroupBox>
#include <QRadioButton>
#include <functional>
#include <vector>
#include "af4calibrationdialogs.h"
#include "./af4calibplotwidget.h"
#include "./af4innercalibrationframe.h"
#include "./af4stokeseinsteincalculatorwidget.h"
#include "./af4channeldimswidget.h"
#include "../../Core/af4evalmacros.h"
#include "../../Core/af4calibrator.h"
#include "../../Core/af4csvwriter.h"
#include <../smallQDerivates/af4scinotspinbox.h>

struct CtrlBoxRefs {
   QWeakPointer<QDoubleSpinBox> leftOffset;
   QWeakPointer<QDoubleSpinBox> tVoid;
   QWeakPointer<QDoubleSpinBox> tElution;
};

/*! ***************************************************************************************
***
***  \class     AF4CalibParametersFrame "src/Gui/Calibration/af4channelcalibwidget.h"
***  \brief     AF4CalibParametersFrame contains calibration parameters
***  \details   AF4CalibParametersFrame has 8 spinboxes holding
***             the parameters for the calibration. The Diffusion coefficient can be
***             calculated via Stokes Einstein from hydrodynamic radius.
***             The boxes are enabled when their parameters have to be used by
***             set combination of calibration methods
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
********************************************************************************************/

class AF4CalibParametersFrame final : public QFrame
{
   Q_OBJECT
public:
   /*!
    * \brief AF4CalibParametersFrame
    * \param channelId
    * \param calibId
    * \param channelName
    * \param calibName
    * \param parent
    */
   AF4CalibParametersFrame(int channelId,
                           int calibId,
                           const QString &channelName,
                           const QString &calibName,
                           QWidget *parent);
   /*!
     */
   ~AF4CalibParametersFrame();

   /*!
    * \brief getCtrlBoxRefs
    * \return returns references to the three controlboxes which
    */
   CtrlBoxRefs getCtrlBoxRefs();

   /*!
    * \brief getDiffCoefficient Returns the diffusion Coefficient shown in the
    *        corresponding FFFTwoBoxWidget
    * \return diffusion Coefficient
    */
   double getDiffCoefficient() const { return diffCoefficient->value();}

   /*!
    * \brief getCrossFlow Returns the crossFlow shown in the
    *        corresponding FFFTwoBoxWidget
    * \return crossFlow
    */
   double getCrossFlow() const { return crossFlow->value();}

   /*!
    * \brief getCrossFlow Returns the crossFlow shown in the
    *        corresponding FFFTwoBoxWidget
    * \return crossFlow
    */
   double getRelFocusPoint() const {return relFocusPoint->value();}

   /*!
    * \brief getTemperature Returns the temperature shown in the
    *        corresponding FFFTwoBoxWidget
    * \return temperature
    */
   double getTemperature() const {return temperature->value();}

   /*!
    * \brief getVoidPeakTime Returns the voidPeakTime shown in the
    *        corresponding FFFTwoBoxWidget
    * \return voidPeakTime
    */
   double getVoidPeakTime() const { return voidPeakTime->value() - leftOffsetTime->value(); }

   /*!
    * \brief getLeftOffsetTime Returns the leftOffsetTime (t0) shown in the
    *        corresponding FFFTwoBoxWidget
    * \return leftOffsetTime
    */
   //double getLeftOffsetTime() const { return leftOffsetTime->value(); }

   /*!
    * \brief getDiffCoefficient Returns the elutionTime (t elutionMaximum)
    *        shown in the corresponding FFFTwoBoxWidget
    * \return voidPeakTime
    */
   double getElutionTime() const { return elutionTime->value() - leftOffsetTime->value(); }

   /*!
    * \brief getDiffCoefficient returns the elutionFlow shown
    *        in the corresponding FFFTwoBoxWidget
    * \return elutionFlow
    */
   double getElutionFlow() const { return elutionFlow->value(); }

#define SET_MACRO(function, boxPtr) \
   bool function(double value){\
   if(value < boxPtr->minimum()){\
   boxPtr->setValue(boxPtr->minimum());\
   return false;\
}\
   else if (value > boxPtr->maximum()){\
   boxPtr->setValue(boxPtr->maximum());\
   return false;\
}\
   else {\
   boxPtr->setValue(value);\
   return true;\
}\
};
   /*!
    * \brief setDiffCoefficient set value of the diffusion Coefficient
    * \return bool if value could be set
    */
   SET_MACRO(setDiffCoefficient, diffCoefficient)

   /*!
    * \brief setCrossFlow set value of the crossflow
    * \return bool if value could be set
    */
   SET_MACRO(setCrossFlow, crossFlow)

   /*!
    * \brief setCrossFlow set value of the crossflow
    * \return bool if value could be set
    */
   SET_MACRO(setRelFocusPoint, relFocusPoint)

   /*!
    * \brief setTemperature set value of the temperature
    * \return bool if value could be set
    */
   SET_MACRO(setTemperature, temperature)


   /*!
    * \brief setVoidPeakTime set value of the voidPeakTime
    * \return bool if value could be set
    */
   //SET_MACRO(setVoidPeakTime, voidPeakTime)
   //#define SET_MACRO(function, boxPtr)
   bool setVoidPeakTime(double value){
      value += leftOffsetTime->value();
      if(value  < voidPeakTime->minimum()){
         voidPeakTime->setValue(voidPeakTime->minimum());
         return false;
      }
      else if (value > voidPeakTime->maximum()){
         voidPeakTime->setValue(voidPeakTime->maximum());
         return false;
      }
      else {
         voidPeakTime->setValue(value);
         return true;
      }
   }

   //#define SET_MACRO(function, boxPtr)
   bool setElutionTime(double value){
      value += leftOffsetTime->value();
      if(value  < elutionTime->minimum()){
         elutionTime->setValue(elutionTime->minimum());
         return false;
      }
      else if (value > elutionTime->maximum()){
         elutionTime->setValue(elutionTime->maximum());
         return false;
      }
      else {
         elutionTime->setValue(value);
         return true;
      }
   }

   /*!
    * \brief setElutionTime set value of the elutionTime
    * \return bool if value could be set
    */
   //SET_MACRO(setElutionTime, elutionTime)

   /*!
    * \brief setElutionFlow set value of the elutionFlow
    * \return bool if value could be set
    */
   SET_MACRO(setElutionFlow, elutionFlow)

   /*!
    * \brief setLeftOffsetTime set value of the leftOffsetTime
    * \return bool if value could be set
    */
   SET_MACRO(setLeftOffsetTime, leftOffsetTime)

#undef SET_MACRO

   public slots:
      void saveSettings();
   /*!
    * \brief adaptEnablingStatus Activates/Deactivatees according
    * \param
    * \todo NOT IMPLEMENTED DETAILS, BUT STRUCTURE ONLY; ALREADY CONNECTED;
    */
   void adaptEnablingStatus(CalibModeSettings m);
signals:
   void saveButtonClicked();

private slots:
   void callDiffCoeffDialog();

private:



   void loadSettings();

   int channelId;
   int calibId;
   QString channelName;
   QString calibName;

   QGridLayout    *lay                         = nullptr;
   QDoubleSpinBox *crossFlow                   = nullptr;
   QDoubleSpinBox *elutionFlow                 = nullptr;
   QDoubleSpinBox *temperature                 = nullptr;
   QDoubleSpinBox *relFocusPoint               = nullptr;
   QSharedPointer<QDoubleSpinBox> leftOffsetTime;
   QSharedPointer<QDoubleSpinBox> elutionTime;
   QSharedPointer<QDoubleSpinBox> voidPeakTime;
   AF4SciNotSpinBox *diffCoefficient           = nullptr;
   QToolButton      *diffCoeffCalcButton       = nullptr;
   AF4StokesEinsteinCalculatorWidget *diffCoeffCalcWidget = nullptr;

   NO_COPY_ASSIGNMENT_CTORS(AF4CalibParametersFrame)
};
#endif // AF4CALIBPARAMETERSFRAME_H
