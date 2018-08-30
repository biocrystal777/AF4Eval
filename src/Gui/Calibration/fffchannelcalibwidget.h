#ifndef FFFCHANNELCALIBWIDGET_H
#define FFFCHANNELCALIBWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_text_label.h>
#include "../Core/af4datatypes.h"
#include "../Core/af4parameterstructs.h"
#include "./fffdiffcoeffcalcwidget.h"
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../Core/af4csvparser.h"
#include "./fffcalibplotwidget.h"

class FFFCalibPlotWidget;

/**************************************************************
***
***  FFFChannelCalibWidget
***     (using Qt 4.8.2)
***
**************************************************************/

/*!
 * \brief The FFFChannelCalibWidget class is the second parameter input widget
 *
 * \details The FFFChannelCalibWidget contains different input parameters
 *          to enter parameters specific to a certain calibration
 *          profile a button enables a user to calculate the channel width
 *          from these parameters by emitting a signal that is connected to
 *          an exterior calculation slot function. Each calibration has its own
 *          widget
 *
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */
class FFFChannelCalibWidget : public QWidget
{
   Q_OBJECT

public:

   /*!
    * \brief FFFChannelCalibWidget constructor of this class
    * \param parent parent widget
    */
   explicit FFFChannelCalibWidget(
         const int channelId,
         const int calibId,
         const QString channelName,
         const QString calibName,
         const bool loadParameters,
         QWidget *parent = nullptr);

   /*!
    * \brief destructor of this class
    */
   ~FFFChannelCalibWidget();

   FFFChannelCalibWidget(const FFFChannelCalibWidget& src)        = delete;
   FFFChannelCalibWidget& operator= (FFFChannelCalibWidget& src)  = delete;
   FFFChannelCalibWidget(FFFChannelCalibWidget&& src)             = delete;
   FFFChannelCalibWidget& operator= (FFFChannelCalibWidget&& src) = delete;

   /*!
    * \brief getDiffCoefficient Returns the diffusion Coefficient shown in the
    *        corresponding FFFTwoBoxWidget
    * \return diffusion Coefficient
    */
   inline double getDiffCoefficient() const { return diffCoefficient->value();}

   /*!
    * \brief getCrossFlow Returns the crossFlow shown in the
    *        corresponding FFFTwoBoxWidget
    * \return crossFlow
    */
   inline double getCrossFlow() const { return crossFlow->value();}

   /*!
    * \brief getCrossFlow Returns the crossFlow shown in the
    *        corresponding FFFTwoBoxWidget
    * \return crossFlow
    */
   inline double getRelFocusPoint() const {return relFocusPoint->value();}

   /*!
    * \brief getTemperature Returns the temperature shown in the
    *        corresponding FFFTwoBoxWidget
    * \return temperature
    */
   inline double getTemperature() const {return temperature->value();}

   /*!
    * \brief getVoidPeakTime Returns the voidPeakTime shown in the
    *        corresponding FFFTwoBoxWidget
    * \return voidPeakTime
    */
   inline double getVoidPeakTime() const { return voidPeakTime->value(); }

   /*!
    * \brief getLeftOffsetTime Returns the leftOffsetTime (t0) shown in the
    *        corresponding FFFTwoBoxWidget
    * \return leftOffsetTime
    */
   inline double getLeftOffsetTime() const { return leftOffsetTime->value(); }

   /*!
    * \brief getDiffCoefficient Returns the elutionTime (t elutionMaximum)
    *        shown in the corresponding FFFTwoBoxWidget
    * \return voidPeakTime
    */
   inline double getElutionTime() const { return elutionTime->value(); }

   /*!
    * \brief getDiffCoefficient returns the elutionFlow shown
    *        in the corresponding FFFTwoBoxWidget
    * \return elutionFlow
    */
   inline double getElutionFlow() const { return elutionFlow->value(); }

   /*!
    * \brief getDiffCoefficient returns the channel width shown
    *        in the corresponding FFFTwoBoxWidget
    * \return channel width
    */
   double getChannelWidth() const { return channelWidth->value(); }

   /*!
    * \brief getDiffCoefficient returns the channel width shown
    *        in the corresponding FFFTwoBoxWidget
    * \return hydrodynVolume
    */
   double getHydrodynVolume() const { return hydrodynVolume->value(); }

   /*!
    * \brief getGeometVolume returns the channel width shown
    *        in the corresponding FFFTwoBoxWidget
    * \return geometVolume
    */
   double getGeometVolume() const { return geometVolume->value(); }


   /*!
    * \brief getDiffCoefficient returns the input FileName
    * \return channel width
    */
   inline QString getInputFileName() const { return inputFileName->text(); }

   /*!
    * \brief getDiffCoefficient returns the date
    * \return channel width
    */
   inline QString getDateDescr() const { return dateDescr->text(); }

   /*!
    * \brief getDiffCoefficient returns the sample Name
    * \return channel width
    */
   inline QString getSampleDescr() const { return sampleDescr->text(); }

   /*!
    * \brief getDiffCoefficient returns the buffer info
    * \return channel width
    */
   inline QString getBufferDescr() const { return bufferDescr->text(); }

   /*!
    * \brief getDiffCoefficient returns the buffer info
    * \return channel width
    */
   inline QString getNotesDescr() const { return notesDescr->toPlainText(); }


   QString getInputFilePath(bool quoted = false);

   bool setInputFilePath(QString path, bool quoted = true);

   /*!
    * \brief setDiffCoefficient set value of the diffusion Coefficient
    * \return bool if value could be set
    */

   bool setDiffCoefficient(double value);


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
    * \brief setCrossFlow set value of the crossflow
    * \return bool if value could be set
    */
   SET_MACRO(setCrossFlow, crossFlow)
 //  bool setCrossFlow(double value);

   /*!
    * \brief setCrossFlow set value of the crossflow
    * \return bool if value could be set
    */
   SET_MACRO(setRelFocusPoint, relFocusPoint)
   //bool setRelFocusPoint(double relFocusPoint);

   /*!
    * \brief setTemperature set value of the temperature
    * \return bool if value could be set
    */
   SET_MACRO(setTemperature, temperature)
   //bool setTemperature(double value);

   /*!
    * \brief setVoidPeakTime set value of the voidPeakTime
    * \return bool if value could be set
    */
   SET_MACRO(setVoidPeakTime, voidPeakTime)
   //bool setVoidPeakTime(double value);

   /*!
    * \brief setLeftOffsetTime set value of the leftOffsetTime
    * \return bool if value could be set
    */
   SET_MACRO(setLeftOffsetTime, leftOffsetTime)
   //bool setLeftOffsetTime(double value);

   /*!
    * \brief setElutionTime set value of the elutionTime
    * \return bool if value could be set
    */
   SET_MACRO(setElutionTime, elutionTime)
   //bool setElutionTime(double value);

   /*!
    * \brief setElutionFlow set value of the elutionFlow
    * \return bool if value could be set
    */
   SET_MACRO(setElutionFlow, elutionFlow)
   //bool setElutionFlow(double value);


   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   SET_MACRO(setHydrodynVolume, hydrodynVolume)
   //bool setHydrodynVolume(double value);

   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   SET_MACRO(setGeometVolume, geometVolume)
   //bool setGeometVolume(double value);

   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   bool setChannelWidth(double value);


   /*!
    * \brief setInputFileName set the inputFileName
    * \return bool if value could be set
    **/
   bool setInputFileName(QString value, bool quoted = false);



   inline void setDateDescr(const QString   &text)   {dateDescr->setText(text); }
   inline void setBufferDescr(const QString &text) {bufferDescr->setText(text);}
   inline void setSampleDescr(const QString &text) {sampleDescr->setText(text);}
   inline void setNotesDescr(const QString  &text)  {notesDescr->setText(text);}


   void writeSettings();
   inline void setChannelName(const QString &newName){ channelName = newName; }
   inline void setChannelId(int newId){ channelId = newId; }
   inline void setCalibName(const QString &newName){ calibName = newName; }
   inline void setCalibId(int newId){ calibId = newId; }




   inline ParametersForCalibration getParametersForCalibration () {
      ParametersForCalibration p;
      p.elutionFlow    = this->getElutionFlow();
      p.crossFlow      = this->getCrossFlow();
      p.relFocusPoint  = this->getRelFocusPoint();
      p.leftOffsetTime = this->getLeftOffsetTime();
      p.voidPeakTime   = this->getVoidPeakTime();
      p.elutionTime    = this->getElutionTime();
      p.diffCoeff      = this->getDiffCoefficient();
      return p;
   }

   inline  AllCalibrationParameters getAllCalibrationParameters() {
      AllCalibrationParameters p;
      p.temperature     = this->getTemperature();
      p.elutionFlow     = this->getElutionFlow();
      p.crossFlow       = this->getCrossFlow();
      p.relFocusPoint   = this->getRelFocusPoint();
      p.leftOffsetTime  = this->getLeftOffsetTime();
      p.voidPeakTime    = this->getVoidPeakTime();
      p.elutionTime     = this->getElutionTime();
      p.diffCoeff       = this->getDiffCoefficient();
      p.chWidth         = this->getChannelWidth();
      p.hydrodynVolume  = this->getHydrodynVolume();
      p.geometVolume    = this->getGeometVolume();
      p.date            = this->getDateDescr();
      p.sample          = this->getSampleDescr();
      p.buffer          = this->getBufferDescr();
      p.additionalNotes = this->getNotesDescr();
      return p;
   }

   void setAllCalibrationParameters(const AllCalibrationParameters &p){
      this->setTemperature(p.temperature);
      this->setElutionFlow(p.elutionFlow);
      this->setCrossFlow(p.crossFlow);
      this->setRelFocusPoint(p.relFocusPoint);
      this->setLeftOffsetTime(p.leftOffsetTime);
      this->setVoidPeakTime(p.voidPeakTime);
      this->setElutionTime(p.elutionTime);
      this->setDiffCoefficient(p.diffCoeff);
      this->setChannelWidth(p.chWidth);
      this->setHydrodynVolume(p.hydrodynVolume);
      this->setGeometVolume(p.geometVolume);
      this->setDateDescr(p.date);
      this->setSampleDescr(p.sample);
      this->setBufferDescr(p.buffer);
      this->setNotesDescr(p.additionalNotes);
   }

private:

   //void defaultInit();
   void loadSettings();

   QFrame *widgetFrame                         = nullptr;
   QGridLayout *widgetLayout                   = nullptr;
   QGridLayout *frameLayout                    = nullptr;

   QWidget *fileWidget                         = nullptr;
   QHBoxLayout *fileLayout                     = nullptr;
   QToolButton *inputFileChooser               = nullptr;
   QLineEdit *inputFileName                    = nullptr;

   FFFTwoBoxWidget *diffCoefficient            = nullptr;
   QDoubleSpinBox *crossFlow                   = nullptr;
   QDoubleSpinBox *temperature                 = nullptr;
   QDoubleSpinBox *voidPeakTime                = nullptr;
   QDoubleSpinBox *relFocusPoint               = nullptr;
   QDoubleSpinBox *leftOffsetTime              = nullptr;
   QDoubleSpinBox *elutionTime                 = nullptr;
   QDoubleSpinBox *elutionFlow                 = nullptr;

   QFrame *calibrationFrame                    = nullptr;
   QGridLayout *calibrationFrameLayout         = nullptr;
   QPushButton *calibButton                    = nullptr;
   FFFTwoBoxWidget *channelWidth               = nullptr;
   QDoubleSpinBox *hydrodynVolume              = nullptr;
   QDoubleSpinBox *geometVolume               = nullptr;

   QToolButton *diffCoeffCalculator            = nullptr;
   FFFDiffCoeffCalcWidget *diffCoeffCalcWidget = nullptr;

   QLineEdit *dateDescr                        = nullptr;
   QLineEdit *sampleDescr                      = nullptr;
   QLineEdit *bufferDescr                      = nullptr;
   QTextEdit *notesDescr                       = nullptr;

   FFFCalibPlotWidget *plotWidget              = nullptr;
   FFFCalibPlotWidget *plotWidget2              = nullptr;

   int channelId;
   int calibId;
   QString channelName;
   QString calibName;

   /*!
    * \brief chopStringsQuotMarksToOne if a string has more than one Quotation mark, they are merged
    * \example """this string"""   =>    "this string"
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksToOne(QString &string) const;

   /*!
    * \brief chopStringsQuotMarksEntirely removes quotation marks of strings
    * \example """this string"""   =>    this string
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksEntirely(QString &string) const;

signals:

   /*!
    * \brief calibrateChannelCalled signal will be emitted when the
    *        calibButton has been pressed
    */
   void calibrateChannelCalled();

protected slots:


   void callDiffCoeffDialog();
   void finishDiffCoeffDialog();

   /*!
    * \brief chooseInputFile calls a QFileDialog to choose the input file Name
    *        (see callCalibrateChannel()).
    */
   void chooseInputFile();

   /*!
    * \brief callCalibrateChannel As this class is thought as containment of FFFChannels
    * emits a signal that is connected to a slot method "one level above".
    * => "throw" the signal clicked()
    */
   void callCalibrateChannel();


   void setPlotDataFromFile();




};







#endif // FFFCHANNELCALIBWIDGET_H
