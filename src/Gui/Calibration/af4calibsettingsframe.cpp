#include "af4calibsettingsframe.h"

//-/////////////////////////////////
//
//   Public stuff
//
//-/////////////////////////////////

AF4CalibSettingsFrame::AF4CalibSettingsFrame(QSharedPointer<QMap<QString, AF4ChannelDimsWidget *> >channelConfigWidgets,
                                             QMap<QString, QMap<QString, AF4ChannelCalibWidget *> *> *channelCalibWidgets,
                                             const QString &prefix,
                                             QWidget *parent) :
   QFrame(parent),
   prefix(prefix),
   channelConfigWidgets(channelConfigWidgets),
   channelCalibWidgets(channelCalibWidgets)
{
   channelKeyList = new QList<QString>(channelConfigWidgets->keys());
   calibKeyList = new QList<QList<QString>*>();
   for(int i=0; i < channelKeyList->size(); ++i)
      calibKeyList->append(new QList<QString>(channelCalibWidgets->value(channelKeyList->at(i))->keys()));

   layout = new QGridLayout(this);
   channelChooser = new QComboBox(this);
   for (int i=0; i < channelKeyList->size(); ++i)
      channelChooser->addItem(channelKeyList->at(i));
   layout->addWidget(channelChooser, 0, 0, 1, 3);

   QwtTextLabel *qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("b</mi><mtext>0</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("b_0 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Minimal trapezoidal width"));
   layout->addWidget(qwtLabel, 1, 0, 1, 1, Qt::AlignLeft);
   b0 = new QLabel(this);
   layout->addWidget(b0, 1, 1, 1, 2, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(this);
   //qwtLabel->setText(QString("b</mi><mtext>L</mtext></msub><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("b_L / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Maximal trapezoidal width"));
   layout->addWidget(qwtLabel, 1, 5, 1, 1, Qt::AlignLeft);
   bL = new QLabel(this);
   layout->addWidget(bL, 1, 6, 1, 2, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(this);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L_Σ / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 3"));
   layout->addWidget(qwtLabel, 1, 10, 1, 1, Qt::AlignLeft);
   chLength = new QLabel(this);
   layout->addWidget(chLength, 1, 11, 1, 2, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(this);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L1 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 1"));
   layout->addWidget(qwtLabel, 2, 0, 1, 1, Qt::AlignLeft);
   length1 = new QLabel(this);
   layout->addWidget(length1, 2, 1, 1, 2, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(this);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L2 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 2"));
   layout->addWidget(qwtLabel, 2, 5, 1, 1, Qt::AlignLeft);
   length2 = new QLabel(this);
   layout->addWidget(length2, 2, 6, 1, 2, Qt::AlignLeft);

   qwtLabel = new QwtTextLabel(this);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(QString("L3 / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("Channel Length 3"));
   layout->addWidget(qwtLabel, 2, 10, 1, 1, Qt::AlignLeft);
   length3 = new QLabel(this);
   layout->addWidget(length3, 2, 11, 1, 2, Qt::AlignLeft);

   updateChannelValues(channelKeyList->at(channelChooser->currentIndex()));
   allCalibChoosers = new QMap<QString, QComboBox*>();
   QString channelKey;
   int numberOfChannelCalibs;
   QString calibKey;

   for (int i=0; i < channelKeyList->size(); i++){
      channelKey = channelKeyList->at(i);
      currentCalibChooser = new QComboBox(this);
      allCalibChoosers->insert(channelKey, currentCalibChooser);
      currentCalibChooser->hide();
      //fill comboBoxes;
      numberOfChannelCalibs = channelCalibWidgets->value(channelKey)->size();
      for(int j=0; j < numberOfChannelCalibs; j++){
         calibKey = calibKeyList->at(i)->at(j);
         currentCalibChooser->addItem(calibKey);
      }
      connect(allCalibChoosers->value(channelKey), qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4CalibSettingsFrame::updateCalibValues);
   }
   connect(channelChooser, qOverload<const QString &>(&QComboBox::currentIndexChanged),
           this, &AF4CalibSettingsFrame::updateChannelValues);

   layout->addWidget(currentCalibChooser, 7, 0, 1, 3);
   currentCalibChooser->show();

   qwtLabel = new QwtTextLabel(this);
   qwtLabel->setText(QString("w / cm"), QwtText::PlainText);
   qwtLabel->setToolTip(QString("channel height"));
   layout->addWidget(qwtLabel, 8, 0, 1, 1, Qt::AlignRight);


   channelWidth = new QLabel(this);
   layout->addWidget(channelWidth, 8, 1, 1, 2, Qt::AlignRight);


   channelVolumeDef = new QLabel(QString("V<sup> 0</sup>"), this);
   channelVolumeDef->setToolTip("hydrodynamic volume");
   layout->addWidget(channelVolumeDef, 9, 0, 1, 1, Qt::AlignRight);
   channelVolume = new QLabel(this);
   layout->addWidget(channelVolume, 9, 1, 1, 2, Qt::AlignRight);

   qwtLabel = new QwtTextLabel(this);
   // qwtLabel->setText(QString(" <math><mi>L</mi><mtext>&nbsp;/&nbsp;cm</mtext></math>"), QwtText::MathMLText);
   qwtLabel->setText(("Vg / cm"), QwtText::TeXText);
   qwtLabel->setToolTip(QString("Geometrical Volume"));
   layout->addWidget(qwtLabel, 10, 0, 1, 1, Qt::AlignRight);
   length3 = new QLabel(this);
   layout->addWidget(length3, 10, 1, 1, 2, Qt::AlignRight);

   updateChannelValues(channelKeyList->at(channelChooser->currentIndex()));
   this->setFrameStyle(0x1011);
   loadParameters();
}

void AF4CalibSettingsFrame::saveParameters()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("%1/channelCalibs/channelIndex").arg(prefix), channelChooser->currentIndex());
   settings.setValue(tr("%1/channelCalibs/calibIndex").arg(prefix), currentCalibChooser->currentIndex());
}

//-/////////////////////////////////
//
//   Public slots
//
//-/////////////////////////////////

void AF4CalibSettingsFrame::updateChannelValues(QString channelKey)
{
   // choose new calibChooser:
   currentChannelKey = channelKey;
   if(currentCalibChooser){
      currentCalibChooser->hide();
      channelKey = channelChooser->currentText();
      currentCalibChooser = allCalibChoosers->value(channelKey);
      layout->addWidget(currentCalibChooser, 7, 0, 1, 3);
      adaptCalibValues(currentCalibChooser->currentText());
      currentCalibChooser->show();
   }
   // update values:
   AF4ChannelDimsWidget* configWidget = channelConfigWidgets->value(channelKey);
   chLength->setText(QString::number(configWidget->getChLength(), 'E'));
   length1->setText( QString::number(configWidget->getLength1(), 'E')  );
   length2->setText( QString::number(configWidget->getLength2(), 'E')  );
   length3->setText( QString::number(configWidget->getLength3(), 'E')  );
   b0->setText(QString::number(configWidget->getB0(), 'E'));
   bL->setText(QString::number(configWidget->getBL(), 'E'));
}

void AF4CalibSettingsFrame::updateCalibValues(QString calibKey)
{
   currentCalibKey = calibKey;
   AF4ChannelCalibWidget* calibWidget = channelCalibWidgets->value(channelChooser->currentText())->value(calibKey);
   channelWidth->setText(QString::number(calibWidget->getClassicalChannelWidth(), 'E'));
   channelVolume->setText(QString::number(calibWidget->getHydrodynVolume(), 'E'));
}

void AF4CalibSettingsFrame::adaptCalibValues(QString calibKey)
{
   AF4ChannelCalibWidget* calibWidget = channelCalibWidgets->value(channelChooser->currentText())->value(calibKey);
   channelWidth->setText(QString::number(calibWidget->getClassicalChannelWidth(), 'E'));
   channelVolume->setText(QString::number(calibWidget->getHydrodynVolume(), 'E'));
}

void AF4CalibSettingsFrame::adaptChannelParameters()
{
   // adapt key Lists
   delete channelKeyList;
   channelKeyList = new QList<QString>(channelConfigWidgets->keys());

   delete calibKeyList;
   calibKeyList = new QList<QList<QString>*>();
   for(int i=0; i < channelKeyList->size(); i++)
      calibKeyList->append(new QList<QString>(channelCalibWidgets->value(channelKeyList->at(i))->keys()));
   // adapt channelChooser

   channelChooser->blockSignals(true);
   channelChooser->clear();
   for (int i=0; i < channelKeyList->size(); i++)
      channelChooser->addItem(channelKeyList->at(i));
   channelChooser->blockSignals(false);
   delete allCalibChoosers;
   allCalibChoosers = new QMap<QString, QComboBox*>();
   int numberOfChannelCalibs;
   QString channelKey;
   QString calibKey;
   for (int i=0; i < channelKeyList->size(); i++){
      channelKey = channelKeyList->at(i);
      currentCalibChooser = new QComboBox(this);
      allCalibChoosers->insert(channelKey, currentCalibChooser);
      currentCalibChooser->hide();
      //fill comboBoxes;
      numberOfChannelCalibs = channelCalibWidgets->value(channelKey)->size();
      for(int j=0; j < numberOfChannelCalibs; j++){
         calibKey = calibKeyList->at(i)->at(j);
         currentCalibChooser->addItem(calibKey);
      }
   }

   for (int i=0; i < allCalibChoosers->size(); i++){
      channelKey = channelKeyList->at(i);
      connect(allCalibChoosers->value(channelKey), qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4CalibSettingsFrame::updateCalibValues);
   }

   int channelIndex = channelChooser->findText(currentChannelKey);
   if (channelIndex != -1) channelChooser->setCurrentIndex(channelIndex);

   int calibIndex = currentCalibChooser->findText(currentCalibKey);
   if (calibIndex != -1) currentCalibChooser->setCurrentIndex(calibIndex);
   QString currentChannelKey = channelChooser->currentText();
   updateChannelValues(currentChannelKey);
}

void AF4CalibSettingsFrame::enableVolume(bool enable)
{
   this->channelVolumeDef->setEnabled(enable);
   this->channelVolume->setEnabled(enable);
}

//-/////////////////////////////////
//
//   Private stuff
//
//-/////////////////////////////////

AF4CalibSettingsFrame::~AF4CalibSettingsFrame()
{
   saveParameters();
   if(channelKeyList) {delete channelKeyList; channelKeyList = nullptr;}
   if(calibKeyList){
      for(int i = 0; i < calibKeyList->size(); ++i) delete calibKeyList->at(i);
      delete calibKeyList; calibKeyList = nullptr;
   }
}

void AF4CalibSettingsFrame::loadParameters()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok(false);
   int index = settings.value(tr("%1/channelCalibs/channelIndex").arg(prefix), 0).toInt(&ok);

   if(ok && index < channelChooser->count()) channelChooser->setCurrentIndex(index);
   else AF4Log::logWarning(tr("Channel profile could not found, set to default entry 0."));
   index = settings.value(tr("%1/channelCalibs/calibIndex").arg(prefix), 0).toInt(&ok);

   if(ok && index < currentCalibChooser->count()) currentCalibChooser->setCurrentIndex(index);
   else AF4Log::logWarning(tr("Calibration profile could not found, set to default entry."));
}
