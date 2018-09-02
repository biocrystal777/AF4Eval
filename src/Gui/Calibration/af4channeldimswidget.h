#ifndef AF4CHANNELDIMSWIDGET_H
#define AF4CHANNELDIMSWIDGET_H

#include <QSettings>
#include <QFrame>
#include <QGridLayout>
#include <cmath>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>
#include <qwt_text_label.h>
//#include <qwt_mathml_text_engine.h>     renderer too slow, unfortunately
#include "../Core/af4parameterstructs.h"
#include "../GeneralWidgets/ffflog.h"
#include "../smallQDerivates/ffftwoboxwidget.h"

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

class AF4ChannelDimsWidget : public QWidget
{
   Q_OBJECT
public:
   explicit AF4ChannelDimsWidget(const int channelId,
                                   const QString &channelName,
                                   const bool loadParameters,
                                   QWidget *parent = nullptr);

    ~AF4ChannelDimsWidget();

    AF4ChannelDimsWidget(const AF4ChannelDimsWidget& src) = delete;
    AF4ChannelDimsWidget& operator= (AF4ChannelDimsWidget& src) = delete;
    AF4ChannelDimsWidget(AF4ChannelDimsWidget&& src) = delete;
    AF4ChannelDimsWidget& operator= (AF4ChannelDimsWidget&& src) = delete;

    inline double getChLength() const {return (length1->value() + length2->value() + length3->value());}

    /*!
    * \brief getLength1
    * \return
    */
   inline double getLength1() const {return length1->value();}

   /*!
    * \brief getLength2
    * \return
    */
   inline double getLength2() const {return length2->value();}

   /*!
    * \brief getLength3
    * \return
    */
   inline double getLength3() const {return length3->value();}

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
   inline ChannelDims getChannelDimensions(){
      ChannelDims dims;
      dims.length1  = this->getLength1();
      dims.length2  = this->getLength2();
      dims.length3  = this->getLength3();
      dims.chLength = this->getChLength();
      dims.b0       = this->getB0();
      dims.bL       = this->getBL();
      return dims;
   }


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
   * \brief setLength1
   * \param value
   * \return
   */
   SET_MACRO(setLength1, length1)
   /*!
    * \brief setLength2
    * \param value
    * \return
    */
   SET_MACRO(setLength2, length2)
   /*!
   * \brief setLength3
   * \param value
   * \return
   */
   SET_MACRO(setLength3, length3)
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

protected:
   QFrame *widgetFrame       = nullptr;
   QGridLayout *widgetLayout = nullptr;
   QGridLayout *frameLayout  = nullptr;

   QDoubleSpinBox *bL        = nullptr;
   QDoubleSpinBox *b0        = nullptr;

   QDoubleSpinBox *length1   = nullptr;
   QDoubleSpinBox *length2   = nullptr;
   QDoubleSpinBox *length3   = nullptr;

   int channelId;
   QString channelName;

   /*!
    * \brief loadSettings
    */
   void loadSettings();

   /*!
    * \brief defaultInit
    */
   void defaultInit();
};

#endif // AF4CHANNELCONFIGWIDGET_H
