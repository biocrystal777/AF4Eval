#ifndef AF4CHANNELCALIBWIDGET_H
#define AF4CHANNELCALIBWIDGET_H

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
//#include "./af4signalplot.h"
#include "./af4calibplotwidget.h"
#include "./af4stokeseinsteincalculatorwidget.h"
#include "../Core/af4datatypes.h"
#include "../Core/af4parameterstructs.h"
#include "../smallQDerivates/af4scinotspinbox.h"


struct CtrlBoxRefs {
   QWeakPointer<QDoubleSpinBox> leftOffset;
   QWeakPointer<QDoubleSpinBox> tVoid;
   QWeakPointer<QDoubleSpinBox> tElution;
};

/*! ***************************************************************************************
***
***  \class     AF4ChannelCalibWidget "src/Gui/Calibration/af4channelcalibwidget.h"
***  \brief     AF4ChannelCalibWidget enables input of channel dimensions
***  \details   The AF4ChannelCalibWidget contains the physical dimensions of the AF4 channel
***             and enables the input via QDoublespinboxes.
***             Each calibration has its own widget
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
********************************************************************************************/

class AF4ChannelCalibWidget final : public QWidget
{
   Q_OBJECT

public:
   /*!
    * \brief FFFChannelCalibWidget constructor of this class
    * \param parent parent widget
    */
   AF4ChannelCalibWidget(
         const int channelId,
         const int calibId,
         const QString channelName,
         const QString calibName,
         const bool loadParameters,
         QWidget *parent = nullptr);

   /*!
    * \brief destructor of this class
    */
   ~AF4ChannelCalibWidget();

   AF4ChannelCalibWidget(const AF4ChannelCalibWidget& src)        = delete;
   AF4ChannelCalibWidget& operator= (AF4ChannelCalibWidget& src)  = delete;
   AF4ChannelCalibWidget(AF4ChannelCalibWidget&& src)             = delete;
   AF4ChannelCalibWidget& operator= (AF4ChannelCalibWidget&& src) = delete;

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
   double getVoidPeakTime() const { return voidPeakTime->value(); }

   /*!
    * \brief getLeftOffsetTime Returns the leftOffsetTime (t0) shown in the
    *        corresponding FFFTwoBoxWidget
    * \return leftOffsetTime
    */
   double getLeftOffsetTime() const { return leftOffsetTime->value(); }

   /*!
    * \brief getDiffCoefficient Returns the elutionTime (t elutionMaximum)
    *        shown in the corresponding FFFTwoBoxWidget
    * \return voidPeakTime
    */
   double getElutionTime() const { return elutionTime->value(); }

   /*!
    * \brief getDiffCoefficient returns the elutionFlow shown
    *        in the corresponding FFFTwoBoxWidget
    * \return elutionFlow
    */
   double getElutionFlow() const { return elutionFlow->value(); }

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
    * \brief getChannelDimsFromCalib
    * \return
    */
   ChannelDimsFromCalib getChannelDimsFromCalib() const;

   /*!
    * \brief getDiffCoefficient returns the input FileName
    * \return channel width
    */
   QString getInputFileName() const { return inputFileName->text(); }

   /*!
    * \brief getDiffCoefficient returns the date
    * \return channel width
    */
   QString getDateDescr() const { return dateDescr->text(); }

   /*!
    * \brief getDiffCoefficient returns the sample Name
    * \return channel width
    */
   QString getSampleDescr() const { return sampleDescr->text(); }

   /*!
    * \brief getDiffCoefficient returns the buffer info
    * \return channel width
    */
   QString getBufferDescr() const { return bufferDescr->text(); }

   /*!
    * \brief getDiffCoefficient returns the buffer info
    * \return channel width
    */
   QString getNotesDescr() const { return notesDescr->toPlainText(); }

   /*!
    * \brief getInputFilePath
    * \param quoted
    * \return
    */
   QString getInputFilePath(bool quoted = false);

   /*!
    * \brief setInputFilePath
    * \param path
    * \param quoted
    * \return
    */
   bool setInputFilePath(QString path, bool quoted = true);

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
   SET_MACRO(setVoidPeakTime, voidPeakTime)

   /*!
    * \brief setLeftOffsetTime set value of the leftOffsetTime
    * \return bool if value could be set
    */
   SET_MACRO(setLeftOffsetTime, leftOffsetTime)

   /*!
    * \brief setElutionTime set value of the elutionTime
    * \return bool if value could be set
    */
   SET_MACRO(setElutionTime, elutionTime)

   /*!
    * \brief setElutionFlow set value of the elutionFlow
    * \return bool if value could be set
    */
   SET_MACRO(setElutionFlow, elutionFlow)


   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   SET_MACRO(setHydrodynVolume, hydrodynVolume)

   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   SET_MACRO(setGeometVolume, geometVolume)

#undef SET_MACRO
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

   /*!
    * \brief setDateDescr
    * \param text
    */
   void setDateDescr(const QString   &text)   {dateDescr->setText(text); }

   /*!
    * \brief setBufferDescr
    * \param text
    */
   void setBufferDescr(const QString &text) {bufferDescr->setText(text);}

   /*!
    * \brief setSampleDescr
    * \param text
    */
   void setSampleDescr(const QString &text) {sampleDescr->setText(text);}

   /*!
    * \brief setNotesDescr
    * \param text
    */
   void setNotesDescr(const QString  &text)  {notesDescr->setText(text);}

   /*!
    * \brief setChannelName
    * \param newName
    */
   void setChannelName(const QString &newName){ channelName = newName; }

   /*!
    * \brief setChannelId
    * \param newId
    */
   void setChannelId(int newId){ channelId = newId; }

   /*!
    * \brief setCalibName
    * \param newName
    */
   void setCalibName(const QString &newName){ calibName = newName; }

   /*!
    * \brief setCalibId
    * \param newId
    */
   void setCalibId(int newId){ calibId = newId; }

   /*!
    * \brief getParametersForCalibration
    * \return
    */
   ParametersForCalibration getParametersForCalibration ();

   /*!
    * \brief getAllCalibrationParameters
    * \return
    */
  AllCalibrationParameters getAllCalibrationParameters();

   /*!
    * \brief setAllCalibrationParameters
    * \param p
    */
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

   CtrlBoxRefs getCtrlBoxRefs (){
      return CtrlBoxRefs {
         QWeakPointer<QDoubleSpinBox>(this->leftOffsetTime),
               QWeakPointer<QDoubleSpinBox>(this->voidPeakTime),
               QWeakPointer<QDoubleSpinBox>(this->elutionTime)
      };
   }

   /*!
    * \brief saveParameters
    */
   void saveParameters();

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

   AF4SciNotSpinBox *diffCoefficient           = nullptr;
   QDoubleSpinBox *crossFlow                   = nullptr;
   QDoubleSpinBox *temperature                 = nullptr;

   QDoubleSpinBox *relFocusPoint               = nullptr;
   QSharedPointer<QDoubleSpinBox> leftOffsetTime;  //            = nullptr;
   QSharedPointer<QDoubleSpinBox> elutionTime; //                = nullptr;
   QSharedPointer<QDoubleSpinBox> voidPeakTime;//               = nullptr;
   QDoubleSpinBox *  elutionFlow               = nullptr;

   QFrame *calibrationFrame                    = nullptr;
   QGridLayout *calibrationFrameLayout         = nullptr;
   QPushButton *calibButton                    = nullptr;
   AF4SciNotSpinBox *channelWidth              = nullptr;
   QDoubleSpinBox *hydrodynVolume              = nullptr;
   QDoubleSpinBox *geometVolume                = nullptr;

   QToolButton *diffCoeffCalculator            = nullptr;
   AF4StokesEinsteinCalculatorWidget *diffCoeffCalcWidget = nullptr;

   QLineEdit *dateDescr                        = nullptr;
   QLineEdit *sampleDescr                      = nullptr;
   QLineEdit *bufferDescr                      = nullptr;
   QTextEdit *notesDescr                       = nullptr;

   //AF4SignalPlot *plotWidget                   = nullptr;
   //AF4SignalPlot *plotWidget2                  = nullptr;
   //AF4CalibPlotWidget *plotWidget              = nullptr;


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

private slots:

   /*!
    * \brief callDiffCoeffDialog
    */
   void callDiffCoeffDialog();

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

   /*!
    * \brief setPlotDataFromFile
    */
   //int setPlotDataFromFile();
};

#endif // AF4CHANNELCALIBWIDGET_H
