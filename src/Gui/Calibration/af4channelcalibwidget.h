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

struct CalibModes {
   const bool   checkUncertainty;
   const double uncertRange;
   const uint   uncertGridSize;
   const bool   classical;
   const bool   geometric;
   const bool   hydrodynamic;
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
   AF4CalibParametersFrame(int channelId, int calibId,
                           const QString &channelName,
                           const QString &calibName, QWeakPointer<QPushButton> saveSettings,
                           QWidget *parent);
   ~AF4CalibParametersFrame();

   /*!
    * \brief getCtrlBoxRefs
    * \return returns references to the three controlboxes which
    */
   CtrlBoxRefs getCtrlBoxRefs (){
      return CtrlBoxRefs {
         QWeakPointer<QDoubleSpinBox>(this->leftOffsetTime),
               QWeakPointer<QDoubleSpinBox>(this->voidPeakTime),
               QWeakPointer<QDoubleSpinBox>(this->elutionTime)
      };
   }

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
#undef SET_MACRO

   public slots:
      void saveSettings();
   /*!
    * \brief adaptEnablingStatus Activates/Deactivatees according
    * \param
    * \todo NOT IMPLEMENTED DETAILS, BUT STRUCTURE ONLY; ALREADY CONNECTED;
    */
   void adaptEnablingStatus(CalibModes m);

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

   AF4CalibParametersFrame(const AF4CalibParametersFrame& src)        = delete;
   AF4CalibParametersFrame& operator= (AF4CalibParametersFrame& src)  = delete;
   AF4CalibParametersFrame(AF4CalibParametersFrame&& src)             = delete;
   AF4CalibParametersFrame& operator= (AF4CalibParametersFrame&& src) = delete;

};


























/*! ***************************************************************************************
***
***  \class     AF4InnerCalibrationFrame "src/Gui/Calibration/af4channelcalibwidget.h"
***  \brief     AF4InnerCalibrationFrame
***  \details
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
********************************************************************************************/

class AF4InnerCalibrationFrame final : public QFrame
{
   Q_OBJECT
public:
   AF4InnerCalibrationFrame(const int channelId,
                            const int calibId,
                            const QString channelName,
                            const QString calibName,
                            QWeakPointer<QPushButton> saveButton,
                            QWidget *parent);
   ~AF4InnerCalibrationFrame();


   CalibModes getCalibModes() const;

   /*!
   * \brief getDiffCoefficient returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   * \return channel width
   */
   double getChannelWidth() const { return channelWidth->value(); }

   /*!
    * \brief getChannelWidthGeo
    * \return
    */
   double getChannelWidthGeo() const { return channelWidthGeo->value(); }

   /*!
    * \brief getChannelWidthHydro
    * \return
    */
   double getChannelWidthHydro() const { return channelWidthHydro->value(); }

   /*!
   * \brief getDiffCoefficient returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   * \return hydrodynVolume
   */
   double getClassicalVolume() const { return classicalVolume->value(); }

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
   * \brief hydrodynVolume set value of the channelWidth
   * \return bool if value could be set
   */
   SET_MACRO(setClassicalVolume, classicalVolume)

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
   void setChannelWidth(double value) { channelWidth->setValue(value); }
   /*!
   * \brief setChannelWidthGeo
   * \param value
   */
   void setChannelWidthGeo(double value) { channelWidthGeo->setValue(value); }
   /*!
   * \brief setChannelWidthHydro
   * \param value
   */
   void setChannelWidthHydro(double value) { channelWidthHydro->setValue(value); }

public slots:
   void saveSettings();

signals:
   void calibrateChannelCalled();
   void calibModeSettingsChanged(CalibModes m);

private:
   void loadSettings();

   int channelId;
   int calibId;
   QString channelName;
   QString calibName;

   QCheckBox      *checkUncertainties = nullptr;
   QwtTextLabel   *uncertRangeLabel   = nullptr;
   QDoubleSpinBox *uncertRange        = nullptr;
   QwtTextLabel   *uncertGridLabel    = nullptr;
   QSpinBox       *uncertGrid         = nullptr;

   QGridLayout    *lay               = nullptr;
   QPushButton    *calibButton       = nullptr;
   QCheckBox      *classicMode       = nullptr;
   QCheckBox      *geoMode           = nullptr;
   QCheckBox      *hydMode           = nullptr;

   QwtTextLabel *channelWidthLabel      = nullptr;
   QwtTextLabel *channelWidthHydroLabel = nullptr;
   QwtTextLabel *channelWidthGeoLabel   = nullptr;
   QwtTextLabel *classicalVolumeLabel   = nullptr;
   QwtTextLabel *hydrodynVolumeLabel    = nullptr;
   QwtTextLabel *geometVolumeLabel      = nullptr;

   QDoubleSpinBox *channelWidth      = nullptr;
   QDoubleSpinBox *channelWidthHydro = nullptr;
   QDoubleSpinBox *channelWidthGeo   = nullptr;
   QDoubleSpinBox *classicalVolume   = nullptr;
   QDoubleSpinBox *hydrodynVolume    = nullptr;
   QDoubleSpinBox *geometVolume      = nullptr;

   AF4InnerCalibrationFrame(const AF4InnerCalibrationFrame& src)        = delete;
   AF4InnerCalibrationFrame& operator= (AF4InnerCalibrationFrame& src)  = delete;
   AF4InnerCalibrationFrame(AF4InnerCalibrationFrame&& src)             = delete;
   AF4InnerCalibrationFrame& operator= (AF4InnerCalibrationFrame&& src) = delete;
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
***  \todo      support class split
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
   AF4ChannelCalibWidget(const int channelId,
                         const int calibId,
                         const QString channelName,
                         const QString calibName,
                         QWeakPointer<QPushButton> saveButton,
                         QWidget *parent);

   /*!
    * \brief destructor of this class
    */
   ~AF4ChannelCalibWidget();

   /*!
    * \brief getDiffCoefficient returns the channel width shown
    *        in the corresponding FFFTwoBoxWidget
    * \return channel width
    */
   //double getChannelWidth() const { return channelWidth->value(); }
   double getChannelWidth() const { return innerCalibFrame->getChannelWidth(); }

   /*!
    * \brief getDiffCoefficient returns the channel width shown
    *        in the corresponding FFFTwoBoxWidget
    * \return hydrodynVolume
    */
   //double getHydrodynVolume() const { return hydrodynVolume->value(); }
   double getHydrodynVolume() const { return innerCalibFrame->getHydrodynVolume(); }

   /*!
    * \brief getGeometVolume returns the channel width shown
    *        in the corresponding FFFTwoBoxWidget
    * \return geometVolume
    */
   //double getGeometVolume() const { return geometVolume->value(); }
   double getGeometVolume() const { return innerCalibFrame->getGeometVolume(); }



   CalibModes getCalibModes() const{ return innerCalibFrame->getCalibModes(); }

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
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   //SET_MACRO(setHydrodynVolume, hydrodynVolume)

   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   //SET_MACRO(setGeometVolume, geometVolume)

#undef SET_MACRO
   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidth(double value);

   /*!
    * \brief setInputFileName set the inputFileName
    * \return bool if value could be set
    **/
   bool setInputFileName(QString path, bool quoted = false);

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
   void setAllCalibrationParameters(const AllCalibrationParameters &p);

   CtrlBoxRefs getCtrlBoxRefs () { return calibParFrame->getCtrlBoxRefs(); }

signals:
   /*!
    * \brief calibrateChannelCalled signal will be emitted when the
    *        calibButton has been pressed
    */
   void calibrateChannelCalled();

private:

   void loadSettings();

   QFrame      *widgetFrame                   = nullptr;
   QGridLayout *widgetLayout                  = nullptr;
   QGridLayout *frameLayout                   = nullptr;

   QWidget     *fileWidget                    = nullptr;
   QHBoxLayout *fileLayout                    = nullptr;
   QToolButton *inputFileChooser              = nullptr;
   QLineEdit   *inputFileName                 = nullptr;

   AF4CalibParametersFrame  *calibParFrame    = nullptr;
   AF4InnerCalibrationFrame *innerCalibFrame  = nullptr;

   QLineEdit *dateDescr                       = nullptr;
   QLineEdit *sampleDescr                     = nullptr;
   QLineEdit *bufferDescr                     = nullptr;
   QTextEdit *notesDescr                      = nullptr;

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

private slots:
   /*!
    * \brief chooseInputFile calls a QFileDialog to choose the input file Name
    *        (see callCalibrateChannel()).
    */
   void chooseInputFile();

   /*!
    * \brief saveParameters
    */
   void saveParameters();
private:

   AF4ChannelCalibWidget(const AF4ChannelCalibWidget& src)        = delete;
   AF4ChannelCalibWidget& operator= (AF4ChannelCalibWidget& src)  = delete;
   AF4ChannelCalibWidget(AF4ChannelCalibWidget&& src)             = delete;
   AF4ChannelCalibWidget& operator= (AF4ChannelCalibWidget&& src) = delete;

};

#endif // AF4CHANNELCALIBWIDGET_H
