#include "af4calibsettingsframe.h"

//-/////////////////////////////////
//
//   Public stuff
//
//-/////////////////////////////////

AF4CalibSettingsFrame::AF4CalibSettingsFrame(QSharedPointer<QMap<QString, AF4ChannelDimsWidget *> >channelConfigWidgets,
                                             QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget *> > >channelCalibWidgets,
                                             const QString &prefix,
                                             QWidget *parent) :
   QFrame(parent),
   prefix(prefix),
   channelConfigWidgets(channelConfigWidgets),
   channelCalibWidgets(channelCalibWidgets)
{
   channelKeyList = new QList<QString>(channelConfigWidgets->keys());
   calibKeyList =  QList<QList<QString> >();
   for(int i=0; i < channelKeyList->size(); ++i)
      calibKeyList.append( QList<QString>(channelCalibWidgets->value(channelKeyList->at(i)).keys()));

   layout = new QGridLayout(this);
   channelChooser = new QComboBox(this);
   for (int i=0; i < channelKeyList->size(); ++i)
      channelChooser->addItem(channelKeyList->at(i));
   layout->addWidget(channelChooser, 0, 0, 1, 3);

   auto makeLabel = [this](QLabel *&label, QString labelText, QString toolTip, int row, int column){
      QwtTextLabel *qwtLabel = new QwtTextLabel(this);
      qwtLabel->setText(labelText, QwtText::PlainText);
      qwtLabel->setToolTip(toolTip);
      layout->addWidget(qwtLabel, row, column-1, 1, 1, Qt::AlignLeft);
      label = new QLabel(this);
      label->setToolTip(toolTip);
      layout->addWidget(label, row, column, 1, 2, Qt::AlignLeft);
   };

   makeLabel(b0,   "b_0 / cm", "Maximal trapezoidal width", 1, 1);
   makeLabel(bL,   "b_L / cm", "Minimal trapezoidal width", 1, 6);
   makeLabel(LTot, "L_Σ / cm", "Total channel length",      1, 11);
   makeLabel(L1,   "L1  / cm", "Length 1",                  2, 1);
   makeLabel(L2,   "L2  / cm", "Length 2",                  2, 6);
   makeLabel(L3,   "L3  / cm", "Length 3",                  2, 11);

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
      numberOfChannelCalibs = channelCalibWidgets->value(channelKey).size();
      for(int j=0; j < numberOfChannelCalibs; j++){
         calibKey = calibKeyList.at(i).at(j);
         currentCalibChooser->addItem(calibKey);
      }
      connect(allCalibChoosers->value(channelKey), qOverload<const QString &>(&QComboBox::currentIndexChanged),
              this, &AF4CalibSettingsFrame::updateCalibValues);
   }
   connect(channelChooser, qOverload<const QString &>(&QComboBox::currentIndexChanged),
           this, &AF4CalibSettingsFrame::updateChannelValues);

   layout->addWidget(currentCalibChooser, 7, 0, 1, 3);
   currentCalibChooser->show();

   QwtTextLabel *qwtLabel = new QwtTextLabel(this);
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
   L3 = new QLabel(this);
   layout->addWidget(L3, 10, 1, 1, 2, Qt::AlignRight);

   updateChannelValues(channelKeyList->at(channelChooser->currentIndex()));
   this->setFrameStyle(0x1011);
   loadParameters();
}

void AF4CalibSettingsFrame::saveParameters()
{
   QSettings settings("AgCoelfen", "AF4Eval");
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
   LTot->setText(QString::number(configWidget->getChLength(), 'E'));
   L1->setText(QString::number(configWidget->getL1(), 'E') );
   L2->setText(QString::number(configWidget->getL2(), 'E') );
   L3->setText(QString::number(configWidget->getL3(), 'E') );
   b0->setText(QString::number(configWidget->getB0(), 'E') );
   bL->setText(QString::number(configWidget->getBL(), 'E') );
}

void AF4CalibSettingsFrame::updateCalibValues(QString calibKey)
{
   currentCalibKey = calibKey;
   AF4ChannelCalibWidget* calibWidget = channelCalibWidgets->value(channelChooser->currentText()).value(calibKey);
   channelWidth->setText(QString::number(calibWidget->getClassicalChannelWidth(), 'E'));
   channelVolume->setText(QString::number(calibWidget->getHydrodynVolume(), 'E'));
}

void AF4CalibSettingsFrame::adaptCalibValues(QString calibKey)
{
   AF4ChannelCalibWidget* calibWidget = channelCalibWidgets->value(channelChooser->currentText()).value(calibKey);
   channelWidth->setText(QString::number(calibWidget->getClassicalChannelWidth(), 'E'));
   channelVolume->setText(QString::number(calibWidget->getHydrodynVolume(), 'E'));
}

void AF4CalibSettingsFrame::adaptChannelParameters()
{
   // adapt key Lists
   delete channelKeyList;
   channelKeyList = new QList<QString>(channelConfigWidgets->keys());

   //delete calibKeyList;
   //calibKeyList = new QList<QList<QString> >();
   calibKeyList.clear();
   for(int i=0; i < channelKeyList->size(); i++)
      calibKeyList.append(QList<QString>(channelCalibWidgets->value(channelKeyList->at(i)).keys()));
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
      numberOfChannelCalibs = channelCalibWidgets->value(channelKey).size();
      for(int j=0; j < numberOfChannelCalibs; j++){
         calibKey = calibKeyList.at(i).at(j);
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
   //if(!calibKeyList.isEmpty()){
//      for(int i = 0; i < calibKeyList->size(); ++i) delete calibKeyList->at(i);
   //delete calibKeyList; calibKeyList = nullptr;
   //}
   calibKeyList.clear();
}

void AF4CalibSettingsFrame::loadParameters()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   bool ok(false);
   int index = settings.value(tr("%1/channelCalibs/channelIndex").arg(prefix), 0).toInt(&ok);

   if(ok && index < channelChooser->count()) channelChooser->setCurrentIndex(index);
   else AF4Log::logWarning(tr("Channel profile could not found, set to default entry 0."));
   index = settings.value(tr("%1/channelCalibs/calibIndex").arg(prefix), 0).toInt(&ok);

   if(ok && index < currentCalibChooser->count()) currentCalibChooser->setCurrentIndex(index);
   else AF4Log::logWarning(tr("Calibration profile could not found, set to default entry."));
}
