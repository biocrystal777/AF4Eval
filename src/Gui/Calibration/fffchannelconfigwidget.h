#ifndef FFFCHANNELCONFIGWIDGET_H
#define FFFCHANNELCONFIGWIDGET_H

#include <QSettings>
#include <QFrame>
#include <QGridLayout>
#include <cmath>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>
#include <qwt_text_label.h>
#include "../Core/af4parameterstructs.h"
#include "../GeneralWidgets/ffflog.h"
#include "../smallQDerivates/ffftwoboxwidget.h"
#include <qwt_mathml_text_engine.h>
/**************************************************************
***
***  FFFChannelConfigWidget
***      (using Qt 4.8.2)
**************************************************************/

/*!
 * \brief The FFFChannelConfigWidget class is the first parameter input widget
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */
class FFFChannelConfigWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FFFChannelConfigWidget(const int channelId,
                                   const QString &channelName,
                                   const bool loadParameters,
                                   QWidget *parent = nullptr);

    ~FFFChannelConfigWidget();

    FFFChannelConfigWidget(const FFFChannelConfigWidget& src) = delete;
    FFFChannelConfigWidget& operator= (FFFChannelConfigWidget& src) = delete;
    FFFChannelConfigWidget(FFFChannelConfigWidget&& src) = delete;
    FFFChannelConfigWidget& operator= (FFFChannelConfigWidget&& src) = delete;

    inline double getChLength() const {return (length1->value() + length2->value() + length3->value());}

   inline double getLength1() const {return length1->value();}
   inline double getLength2() const {return length2->value();}
   inline double getLength3() const {return length3->value();}

   inline double getB0() const { return b0->value(); }
   inline double getBL() const { return bL->value(); }

   inline QString getChannelName() const {return channelName;}
   inline int getChannelId() const { return channelId;}

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

SET_MACRO(setLength1, length1)
SET_MACRO(setLength2, length2)
SET_MACRO(setLength3, length3)
SET_MACRO(setB0, b0)
SET_MACRO(setBL, bL)
#undef SET_MACRO

   inline void setChannelName(const QString &newName){ channelName = newName; }
   inline void setConfigId(int newId){ channelId = newId; }

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

   void loadSettings();
   void defaultInit();
};

#endif // FFFCHANNELCONFIGWIDGET_H
