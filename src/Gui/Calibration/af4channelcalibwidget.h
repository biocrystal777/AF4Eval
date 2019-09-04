#ifndef AF4CHANNELCALIBWIDGET_H
#define AF4CHANNELCALIBWIDGET_H

#include <QWidget>
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
#include "./af4calibparametersframe.h"
#include "./af4calibplotwidget.h"
#include "./af4innercalibrationframe.h"
#include "./af4stokeseinsteincalculatorwidget.h"
#include "../Core/af4evalmacros.h"
#include "../Core/af4datatypes.h"
#include "../Core/af4parameterstructs.h"
#include "../smallQDerivates/af4scinotspinbox.h"

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

   int getChannelId() const { return channelId; }

   int getCalibId() const { return channelId; }

   double getClassicalChannelWidth() const { return innerCalibFrame->getChannelWidth(); }

   double getGeometChannelWidth() const { return innerCalibFrame->getChannelWidthGeo(); }
   double getHydrodynChannelWidth() const { return innerCalibFrame->getChannelWidthHydro(); }
   double getClassicalVolume() const { return innerCalibFrame->getClassicalVolume(); }
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


   CalibModeSettings getCalibModes() const { return innerCalibFrame->getCalibModes(); }

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
  // SET_MACRO(setHydrodynVolume, hydrodynVolume)

   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   //SET_MACRO(setGeometVolume, geometVolume)

#undef SET_MACRO


   void setClassicalVolume(double value){ innerCalibFrame->setClassicalVolume(value); }

   void setHydrodynVolume(double value){ innerCalibFrame->setHydrodynVolume(value); }

   void setGeometVolume(double value){ innerCalibFrame->setGeometVolume(value); }
   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidthClassical(double value) { innerCalibFrame->setChannelWidthClassical(value); }

   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidthGeo(double value) { innerCalibFrame->setChannelWidthGeo(value); }

   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidthHydro(double value) { innerCalibFrame->setChannelWidthHydro(value); }


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

   /*!
    * \brief getCtrlBoxRefs
    * \return
    */
   CtrlBoxRefs getCtrlBoxRefs () { return calibParFrame->getCtrlBoxRefs(); }

signals:

   /*!
    * \brief inputFileChosen will be emitted when a new file is chosen by the user
    *
    */

   void inputFileChosen();

   /*!
    * \brief calibrateChannelCalled signal will be emitted when the
    *        calibButton has been pressed
    */
   void calibrateChannelCalled();

   /*!
    * \brief saveButtonClicked
    */
   void saveButtonClicked();

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
   NO_COPY_ASSIGNMENT_CTORS(AF4ChannelCalibWidget)
};

#endif // AF4CHANNELCALIBWIDGET_H
