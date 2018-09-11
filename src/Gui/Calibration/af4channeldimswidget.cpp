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
   frameLayout = new QGridLayout(widgetFrame);

   QwtTextLabel *qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("b</mi><mtext>0</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("b_0 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Minimal trapezoidal width"));
   frameLayout->addWidget(qwtLabel, 0, 10, 1, 1, Qt::AlignRight);
   b0 = new QDoubleSpinBox(widgetFrame);   
   b0->setDecimals(3);
   b0->setSingleStep(0.1);
   b0->setMinimum(0.0);
   b0->setMaximum(1000.0);
   frameLayout->addWidget(b0, 0, 11, 1, 4, Qt::AlignLeft);


   qwtLabel = new QwtTextLabel(widgetFrame);
   //qwtLabel->setText(QString("b</mi><mtext>L</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("b_L / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Maximal trapezoidal width"));
   frameLayout->addWidget(qwtLabel, 0, 15, 1, 1, Qt::AlignRight);
   bL = new QDoubleSpinBox(widgetFrame);   
   bL->setDecimals(3);
   bL->setSingleStep(0.1);
   bL->setMinimum(0.0);
   bL->setMaximum(1000.0);
   frameLayout->addWidget(bL, 0, 16, 1, 4, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L1 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 1"));
   frameLayout->addWidget(qwtLabel, 1, 5, 1, 1, Qt::AlignRight);
   length1 = new QDoubleSpinBox(widgetFrame);   
   length1->setDecimals(3);
   length1->setSingleStep(0.1);
   length1->setMinimum(0.0);
   length1->setMaximum(1000.0);
   frameLayout->addWidget(length1, 1, 6, 1, 4, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L2 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 2"));
   frameLayout->addWidget(qwtLabel, 1, 10, 1, 1, Qt::AlignRight);
   length2 = new QDoubleSpinBox(widgetFrame);   
   length2->setDecimals(3);
   length2->setSingleStep(0.1);
   length2->setMinimum(0.0);
   length2->setMaximum(1000.0);
   frameLayout->addWidget(length2, 1, 11, 1, 4, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(widgetFrame);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L3 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 3"));
   frameLayout->addWidget(qwtLabel, 1, 15, 1, 1, Qt::AlignRight);
   length3 = new QDoubleSpinBox(widgetFrame);   
   length3->setDecimals(3);
   length3->setSingleStep(0.1);
   length3->setMinimum(0.0);
   length3->setMaximum(1000.0);
   frameLayout->addWidget(length3, 1, 16, 1, 4, Qt::AlignLeft);

   //QSvgRenderer renderer(QString(":/images/ChannelDims.svg"));
   QImage image(500, 100, QImage::Format_ARGB32);
   image.fill(0xFFFFFFFF);
   QPixmap pixMap = QPixmap(":/images/ChannelDims.png");
   QLabel *labelPtr = new QLabel(widgetFrame);
   labelPtr->setPixmap(pixMap);
   labelPtr->setToolTip(QString("Channel plane AF4"));
   frameLayout->addWidget(labelPtr, 0, 0, 2, 5, Qt::AlignLeft);

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
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   double channelValue;
   QString newChannelName;
   bool ok;

   channelValue = settings.value(tr("channels/%1/length1").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setLength1(channelValue)))
      AF4Log::logWarning(tr("Error while setting length1, 51.")
                         .arg(newChannelName));

   channelValue = settings.value(tr("channels/%1/length2").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setLength2(channelValue)))
      AF4Log::logWarning(tr("Error while setting length2, 51.")
                         .arg(newChannelName));

   channelValue = settings.value(tr("channels/%1/length3").arg(channelId), 2.0).toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("channels/number", "0.0e0");
   if(!(this->setLength3(channelValue)))
      AF4Log::logWarning(tr("Error while setting length3, 51.")
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
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   // window parameters

   settings.setValue(tr("channels/%1/name").arg(channelId), channelName);
   settings.setValue(tr("channels/%1/length1").arg(channelId), this->getLength1());
   settings.setValue(tr("channels/%1/length2").arg(channelId), this->getLength2());
   settings.setValue(tr("channels/%1/length3").arg(channelId), this->getLength3());
   settings.setValue(tr("channels/%1/b0").arg(channelId), this->getB0());
   settings.setValue(tr("channels/%1/bL").arg(channelId), this->getBL());

}
