#ifndef AF4CHANNELDIMSWIDGET_H
#define AF4CHANNELDIMSWIDGET_H

#include <QSettings>
#include <QFrame>
#include <QGridLayout>
#include <cmath>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>
#include <functional>
#include <qwt_text_label.h>
//#include <qwt_mathml_text_engine.h>     renderer too slow, unfortunately
#include "../Core/af4evalmacros.h"
#include "../Core/af4parameterstructs.h"
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"

/*! ***************************************************************************************
***
***  \class     AF4ChannelDimsWidget "src/Gui/Calibration/af4channeldimswidget.h"
***  \brief     AF4ChannelDimsWidget enables the input of channel calibrations
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
********************************************************************************************/

class AF4ChannelDimsWidget final : public QWidget
{
   Q_OBJECT
public:
   explicit AF4ChannelDimsWidget(const int channelId,
                                 const QString &channelName,
                                 const bool loadParameters,
                                 QWidget *parent);
   ~AF4ChannelDimsWidget();

   inline double getChLength() const {return (L1->value() + L2->value() + L3->value());}

   /*!
    * \brief getL1
    * \return
    */
   inline double getL1() const {return L1->value();}

   /*!
    * \brief getL2
    * \return
    */
   inline double getL2() const {return L2->value();}

   /*!
    * \brief getL3
    * \return
    */
   inline double getL3() const {return L3->value();}

   /*!
    * \brief getB0
    * \return
    */
   inline double getB0() const { return b0->value(); }

   /*!
    * \brief getBL
    * \return
    */
   inline double getBL() const { return bL->value(); }

   /*!
    * \brief getChannelName
    * \return
    */
   inline QString getChannelName() const {return channelName;}

   /*!
    * \brief getChannelId
    * \return
    */
   inline int getChannelId() const { return channelId;}

   /*!
    * \brief getChannelDimensions
    * \return
    */
   ChannelDims getChannelDimensions();


#define SET_MACRO(function, boxPtr)\
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
   * \brief setL1
   * \param value
   * \return
   */
   SET_MACRO(setL1, L1)
   /*!
    * \brief setL2
    * \param value
    * \return
    */
   SET_MACRO(setL2, L2)
   /*!
   * \brief setL3
   * \param value
   * \return
   */
   SET_MACRO(setL3, L3)
   /*!
   * \brief setB0
   * \param value
   * \return
   */
   SET_MACRO(setB0, b0)
   /*!
   * \brief setB0
   * \param value
   * \return
   */
   SET_MACRO(setBL, bL)
#undef SET_MACRO

   /*!
    * \brief setChannelName
    * \param newName
    */
   inline void setChannelName(const QString &newName){ channelName = newName; }

   /*!
    * \brief setConfigId
    * \param newId
    */
   inline void setConfigId(int newId){ channelId = newId; }

   /*!
    * \brief writeSettings
    */
   void writeSettings();

private:
   QFrame *widgetFrame       = nullptr;
   QGridLayout *widgetLayout = nullptr;
   QGridLayout *lay  = nullptr;

   QDoubleSpinBox *bL   = nullptr;
   QDoubleSpinBox *b0   = nullptr;

   QDoubleSpinBox *L1   = nullptr;
   QDoubleSpinBox *L2   = nullptr;
   QDoubleSpinBox *L3   = nullptr;

   int     channelId;
   QString channelName;

   /*!
    * \brief loadSettings
    */
   void loadSettings();


   NO_COPY_MOVE_CTORS(AF4ChannelDimsWidget)
};

#endif // AF4CHANNELCONFIGWIDGET_H
