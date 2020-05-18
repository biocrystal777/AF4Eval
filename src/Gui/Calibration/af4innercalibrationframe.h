#ifndef AF4INNERCALIBRATIONFRAME_H
#define AF4INNERCALIBRATIONFRAME_H

#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QPushButton>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSettings>
#include <functional>
#include <qwt_text_label.h>
#include "../Core/af4evalmacros.h"
#include "../GeneralWidgets/af4log.h"
#include "../Core/af4parameterstructs.h"

enum struct CalibMode {
   classical,
   approxGeometric,
   geometric,
   hydrodynamic
};

struct CalibModeSettings {
   const bool   checkUncertainty;
   const double uncertRange;
   const uint   uncertGridSize;
   const bool   classical;
   const bool   approxGeometric;
   const bool   geometric;
   const bool   hydrodynamic;
};

/*! ***************************************************************************************
***
***  \class     AF4InnerCalibrationFrame "src/Gui/Calibration/af4channelcalibwidget.h"
***  \brief     AF4InnerCalibrationFrame  displays results (width and volume) from one calibration algorithm
***
******************************************************************************************/

class AF4InnerCalibResultLine final : public QWidget {
   Q_OBJECT
public:
   AF4InnerCalibResultLine(const QString &wLabelStr,
                           const QString &wtoolTipStr,
                           const QString &vLabelStr,
                           const QString &vtoolTipStr,
                           QWidget *parent);
   ~AF4InnerCalibResultLine(){}

   bool isUsed() const { return useBox->isChecked(); }
   double getWidth() const { return width->value() / 1e4; } // display in µm, return in cm
   double getVolume() const { return volume->value(); }
   void setWidth(double value) { width->setValue(value * 1e4); } // set in cm, display in µm   
   bool setVolume(double value) {
      if(value < volume->minimum()){
         volume->setValue(volume->minimum());
         return false;
      }
      else if(value > volume->maximum()){
         volume->setValue(volume->maximum());
         return false;
      }
      else{
         volume->setValue(value);
         return true;
      }
   }

signals:
   void useBoxToggled(bool enable);

private:
   //const CalibMode calibMode;
   QHBoxLayout    *lay         = nullptr;
   QCheckBox      *useBox      = nullptr;
   QwtTextLabel   *widthLabel  = nullptr;
   QwtTextLabel   *volumeLabel = nullptr;
   QDoubleSpinBox *volume      = nullptr;
   QDoubleSpinBox *width       = nullptr;

   NO_COPY_MOVE_CTORS(AF4InnerCalibResultLine)
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
                            QWidget *parent);
   ~AF4InnerCalibrationFrame();


   CalibModeSettings getCalibModes() const;

   /*!
   * \brief getDiffCoefficient returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   *
   * \return channel width
   */   
   double getChannelWidth() const { return classicCalibLine->getWidth(); }

   /*!
    * \brief getChannelWidthGeo
    * \return
    */   
   double getChannelWidthApproxGeo() const { return approxGeoCalibLine->getWidth(); }

   /*!
    * \brief getChannelWidthGeo
    * \return
    */   
   double getChannelWidthGeo() const { return geometCalibLine->getWidth(); }

   /*!
    * \brief getChannelWidthHydro
    * \return
    */   
   double getChannelWidthHydro() const { return hydrodynCalibLine->getWidth(); }

   /*!
   * \brief getDiffCoefficient returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   * \return hydrodynVolume
   */   
   double getClassicalVolume() const { return classicCalibLine->getVolume(); }


   /*!
   * \brief getGeometVolume returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   * \return geometVolume
   */   
   double getApproxGeometVolume() const { return approxGeoCalibLine->getVolume(); }

   /*!
   * \brief getGeometVolume returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   * \return geometVolume
   */
   double getGeometVolume() const { return geometCalibLine->getVolume(); }

   /*!
   * \brief getDiffCoefficient returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   * \return hydrodynVolume
   */
   double getHydrodynVolume() const { return hydrodynCalibLine->getVolume(); }

   /*!
   * \brief getChannelDimsFromCalib
   * \return
   */
   //ChannelDimsFromCalib getChannelDimsFromCalib() const;

   /*!
   * \brief hydrodynVolume set value of the channelWidth
   * \return bool if value could be set
   */   
   bool setClassicalVolume(double value){ return classicCalibLine->setVolume(value); }

   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   bool setApproxGeometVolume(double value){ return approxGeoCalibLine->setVolume(value); }


   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */   
   bool setGeometVolume(double value){ return geometCalibLine->setVolume(value); }

   /*!
    * \brief hydrodynVolume set value of the channelWidth
    * \return bool if value could be set
    */
   bool setHydrodynVolume(double value){ return hydrodynCalibLine->setVolume(value); }


   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */   
   void setChannelWidthClassical(double value) { classicCalibLine->setWidth(value); }

   /*!
   * \brief setChannelWidthGeo
   * \param value
   */
   void setChannelWidthApproxGeo(double value) { approxGeoCalibLine->setWidth(value); }

   /*!
    * \brief setChannelWidthGeo
    * \param value
    */
   void setChannelWidthGeo(double value) { geometCalibLine->setWidth(value); }

   /*!
   * \brief setChannelWidthHydro
   * \param value
   */
   void setChannelWidthHydro(double value) { hydrodynCalibLine->setWidth(value); }

public slots:
   void saveSettings();

signals:
   void calibrateChannelCalled();
   void calibModeSettingsChanged(CalibModeSettings m);
   void saveButtonClicked();
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

   AF4InnerCalibResultLine *classicCalibLine   = nullptr;
   AF4InnerCalibResultLine *approxGeoCalibLine = nullptr;
   AF4InnerCalibResultLine *geometCalibLine    = nullptr;
   AF4InnerCalibResultLine *hydrodynCalibLine  = nullptr;

   NO_COPY_MOVE_CTORS(AF4InnerCalibrationFrame)
};

#endif // AF4INNERCALIBRATIONFRAME_H
