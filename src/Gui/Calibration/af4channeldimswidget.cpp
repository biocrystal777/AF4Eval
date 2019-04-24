#include "af4channeldimswidget.h"

AF4ChannelDimsWidget::AF4ChannelDimsWidget(const int channelId,
                                               const QString &channelName,
                                               const bool loadParameters,
                                               QWidget *parent) :
   QWidget(parent),
   channelId(channelId),
   channelName(channelName)
{
   widgetLayout = new QGridLayout(this);
   widgetFrame = new QFrame(this);
   widgetFrame->setFrameStyle(0x0023);
   widgetLayout->addWidget(widgetFrame, 0, 0, 1, 1);
   lay = new QGridLayout(widgetFrame);

   auto makeSpinBox = [this](QDoubleSpinBox *&spinBox, QString labelString, QString toolTip, int row, int column, int rowSpan, int columnSpan )  {
      QwtTextLabel *label = new QwtTextLabel(widgetFrame);
      label->setText(labelString, QwtText::PlainText);
      label->setToolTip(toolTip);
      lay->addWidget(label, row, column-1, Qt::AlignRight);
      spinBox = new QDoubleSpinBox(widgetFrame);
      spinBox->setToolTip(toolTip);
      lay->addWidget(spinBox, row, column, rowSpan, columnSpan,Qt::AlignLeft);
   };

   auto configSpinBox = [this](QDoubleSpinBox *spinBox, int decimals, double singleStep, double minimum, double maximum){
      spinBox->setDecimals(decimals);
      spinBox->setSingleStep(singleStep);
      spinBox->setMinimum(minimum);
      spinBox->setMaximum(maximum);
   };

   makeSpinBox(b0, QString("b_0 / cm"), QString("Maximal trapezoidal width"), 0, 11, 1, 4);
   configSpinBox(b0, 3, 0.1, 0.0, 1000.0);

   makeSpinBox(bL, QString("b_L / cm"), QString("Minimal trapezoidal width"), 0, 16, 1, 4);
   configSpinBox(bL, 3, 0.1, 0.0, 1000.0);

   makeSpinBox(L1, QString("L1 / cm"), QString("Length of channel section 1"), 1, 6, 1, 4);
   configSpinBox(L1, 3, 0.1, 0.0, 1000.0);

   makeSpinBox(L2, QString("L2 / cm"), QString("Length of channel section 2"), 1, 11, 1, 4);
   configSpinBox(L2, 3, 0.1, 0.0, 1000.0);

   makeSpinBox(L3, QString("L3 / cm"), QString("Length of channel section 3"), 1, 16, 1, 4);
   configSpinBox(L3, 3, 0.1, 0.0, 1000.0);

   //QSvgRenderer renderer(QString(":/images/ChannelDims.svg"));
   QImage image(500, 100, QImage::Format_ARGB32);
   image.fill(0xFFFFFFFF);
   QPixmap pixMap = QPixmap(":/images/ChannelDims.png");
   QLabel *labelPtr = new QLabel(widgetFrame);
   labelPtr->setPixmap(pixMap);
   labelPtr->setToolTip(QString("Channel plane AF4"));
   lay->addWidget(labelPtr, 0, 0, 2, 5, Qt::AlignLeft);

   if(loadParameters) loadSettings();
   else defaultInit();
}

AF4ChannelDimsWidget::~AF4ChannelDimsWidget()
{
   writeSettings();
}

void AF4ChannelDimsWidget::loadSettings()
{
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
}\
};
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   double channelValue;
   QString newChannelName;
   bool ok;

   channelValue = settings.value(tr("channels/%1/L1").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setL1(channelValue)))
      AF4Log::logWarning(tr("Error while setting L1, 51.")
                         .arg(newChannelName));

   channelValue = settings.value(tr("channels/%1/L2").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setL2(channelValue)))
      AF4Log::logWarning(tr("Error while setting L2, 51.")
                         .arg(newChannelName));

   channelValue = settings.value(tr("channels/%1/L3").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setL3(channelValue)))
      AF4Log::logWarning(tr("Error while setting L3, 51.")
                         .arg(newChannelName));


   channelValue = settings.value(tr("channels/%1/b0").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setB0(channelValue)))
      AF4Log::logWarning(tr("Error while setting chLenght, 51.")
                         .arg(newChannelName));

   channelValue = settings.value(tr("channels/%1/bL").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");   
   if(!(this->setBL(channelValue)))
      AF4Log::logWarning(tr("Error while setting chLenght, 51.")
                         .arg(newChannelName));
#undef CHECK_SETTINGS_CONVERSION
}

void AF4ChannelDimsWidget::defaultInit()
{
}

void AF4ChannelDimsWidget::writeSettings()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   // window parameters

   settings.setValue(tr("channels/%1/name").arg(channelId), channelName);
   settings.setValue(tr("channels/%1/L1").arg(channelId), this->getL1());
   settings.setValue(tr("channels/%1/L2").arg(channelId), this->getL2());
   settings.setValue(tr("channels/%1/L3").arg(channelId), this->getL3());
   settings.setValue(tr("channels/%1/b0").arg(channelId), this->getB0());
   settings.setValue(tr("channels/%1/bL").arg(channelId), this->getBL());

}

ChannelDims AF4ChannelDimsWidget::getChannelDimensions(){
   return ChannelDims {
      getL1(),
            getL2(),
            getL3(),
            getChLength(),
            getB0(),
            getBL()
   };
}
