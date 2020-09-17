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

   makeSpinBox(b1, QString("b_0 / cm"), QString("Maximal trapezoidal width"), 0, 11, 1, 4);
   configSpinBox(b1, 3, 0.1, 0.0, 1000.0);

   makeSpinBox(b2, QString("b_L / cm"), QString("Minimal trapezoidal width"), 0, 16, 1, 4);
   configSpinBox(b2, 3, 0.1, 0.0, 1000.0);

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

   loadSettings();
}

AF4ChannelDimsWidget::~AF4ChannelDimsWidget()
{
   writeSettings();
}

void AF4ChannelDimsWidget::loadSettings()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");




   auto loadSetting = [this, &settings](QString paramKey, std::function< bool (double) > setVal, double defaultVal) {
      bool ok;
      QString settingsKey = tr("channels/%1/%2").arg(channelId).arg(paramKey);
      double settingsVal = settings.value(settingsKey).toDouble(&ok);
      if(!ok)
         AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2")
                            .arg(paramKey).arg(defaultVal)
                            );
      if(! setVal(settingsVal) )
         AF4Log::logWarning(tr("Error while setting channel dimensions.")
                            .arg(channelName));
   };

   loadSetting(QString("L1"), [this](double v) -> bool { return this->setL1(v);}, 1.0 );
   loadSetting(QString("L2"), [this](double v) -> bool { return this->setL2(v);}, 1.0 );
   loadSetting(QString("L3"), [this](double v) -> bool { return this->setL3(v);}, 1.0 );
   loadSetting(QString("b1"), [this](double v) -> bool { return this->setB1(v);}, 1.0 );
   loadSetting(QString("b2"), [this](double v) -> bool { return this->setB2(v);}, 1.0 );
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
   settings.setValue(tr("channels/%1/b1").arg(channelId), this->getB1());
   settings.setValue(tr("channels/%1/b2").arg(channelId), this->getB2());
}

ChannelDims AF4ChannelDimsWidget::getChannelDimensions(){
   return ChannelDims {
      getL1(),
            getL2(),
            getL3(),
            getChLength(),
            getB1(),
            getB2()
   };
}
