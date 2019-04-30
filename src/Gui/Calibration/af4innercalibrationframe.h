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
#include <qwt/qwt_text_label.h>
#include "../Core/af4evalmacros.h"
#include "../GeneralWidgets/af4log.h"
#include "../Core/af4parameterstructs.h"

enum struct CalibMode {
   classical,
   geometric,
   hydrodynamic
};

struct CalibModeSettings {
   const bool   checkUncertainty;
   const double uncertRange;
   const uint   uncertGridSize;
   const bool   classical;
   const bool   geometric;
   const bool   hydrodynamic;
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
                            //QWeakPointer<QPushButton> saveButton,
                            QWidget *parent);
   ~AF4InnerCalibrationFrame();


   CalibModeSettings getCalibModes() const;

   /*!
   * \brief getDiffCoefficient returns the channel width shown
   *        in the corresponding FFFTwoBoxWidget
   *
   * \return channel width
   */
   double getChannelWidth() const { return channelWidth->value() / 1e4; }    // display in µm, return in cm

   /*!
    * \brief getChannelWidthGeo
    * \return
    */
   double getChannelWidthGeo() const { return channelWidthGeo->value() / 1e4; }

   /*!
    * \brief getChannelWidthHydro
    * \return
    */
   double getChannelWidthHydro() const { return channelWidthHydro->value() / 1e4; }

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
   //ChannelDimsFromCalib getChannelDimsFromCalib() const;

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
   void setChannelWidthClassical(double value) { channelWidth->setValue(value * 1e4); } // set in cm, display in µm
   /*!
   * \brief setChannelWidthGeo
   * \param value
   */
   void setChannelWidthGeo(double value) { channelWidthGeo->setValue(value * 1e4); }    // set in cm, display in µm
   /*!
   * \brief setChannelWidthHydro
   * \param value
   */
   void setChannelWidthHydro(double value) { channelWidthHydro->setValue(value * 1e4); }// set in cm, display in µm

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

   NO_COPY_ASSIGNMENT_CTORS(AF4InnerCalibrationFrame)
};


#endif // AF4INNERCALIBRATIONFRAME_H
