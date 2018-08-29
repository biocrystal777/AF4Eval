#include "fffpeakparameterframe.h"

#ifndef CHECK_SETTINGS_CONVERSION
#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   FFFLog::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };
#endif // CHECK_SETTINGS_CONVERSION

FFFPeakParameterFrame::FFFPeakParameterFrame(const QString &prefix, int id, QWidget *parent) :
   QFrame(parent), prefix(prefix), id(id)
{
   fractionDescription = nullptr;

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   double value;
   double signValue;
   int expValue;

   bool ok;
   int numberOfPeaks = settings.value(tr("/%1/peakParameters/%2/numberOfPeaks").arg(prefix).arg(id), 1).toInt(&ok);
   if(numberOfPeaks < 1) numberOfPeaks = 1;
   if(!ok) FFFLog::logWarning(tr("Could not read number Of Peaks."));

   this->setFrameStyle(0x1011);
   lay = new QGridLayout(this);

   lay->addWidget(new QLabel(tr("<b>Sample Properties</b>"), this), 0, 0, 1, 6, Qt::AlignLeft);

   peakAdder = new QPushButton("Add Peak", this);
   QObject::connect(peakAdder, SIGNAL(clicked()), this, SLOT(addPeakParWidget()));
   lay->addWidget(peakAdder, 0, 7, 1, 2, Qt::AlignCenter);

   scrollArea = new QScrollArea(this);
   scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   lay->addWidget(scrollArea, 1, 0, 10, 10,Qt::AlignTop);

   areaWidget = new QWidget(scrollArea);
   scrollArea->setWidget(areaWidget);
   scrollArea->setWidgetResizable(true);

   peakParLayout = new QHBoxLayout(areaWidget);
   peakParWidgets = new QList<QWidget*>();
   peakParLayouts = new QList<QGridLayout*>();
   peakParLabels = new QList<QLabel*>();
   startTimes = new QList<QDoubleSpinBox*>();
   endTimes = new QList <QDoubleSpinBox*>();
   useSecondVirCoeffs = new QList<QCheckBox*>();
   secondVirCoeffs = new QList<FFFTwoBoxWidget*>();
   useThirdVirCoeffs = new QList<QCheckBox*>();
   thirdVirCoeffs = new QList<FFFTwoBoxWidget*>();

   refIndexIncrements = new QList<QDoubleSpinBox*>();
   waveLengths = new QList<QDoubleSpinBox*>() ;
   epsilon =  new QList<FFFTwoBoxWidget*>();
   useKnownMW = new QList<QCheckBox*>();
   knownMWs = new QList<FFFTwoBoxWidget*>();
   removeButtons = new QList<FFFNumberedToolButton*>();

   // Initialize content of dn/dc + wl stuff
   for(int i = 0; i < numberOfPeaks; ++i){
      addPeakParWidget();
   }


   if(!peakParWidgets){
      FFFLog::logError(tr("Settings could not be loaded, widgets do not exist.."));
      return;
   }

   for(int i = 0; i < peakParWidgets->size(); ++i){

      value = settings.value(tr("/%1/peakParameters/%2/no%3/startTime").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("/%1/peakParameters/%2/no%3/startTime").arg(i), "1.0");
      startTimes->at(i)->setValue(value);

      value = settings.value(tr("/%1/peakParameters/%2/no%3/endTime").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("/%1/peakParameters/%2/no%3/endTime").arg(prefix).arg(id).arg(i), "1.0");
      endTimes->at(i)->setValue(value);

      value = settings.value(tr("/%1/peakParameters/%2/no%3/dndc").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("/%1/peakParameters/%2/no%3/dndc").arg(prefix).arg(id).arg(i), "1.0");
      refIndexIncrements->at(i)->setValue(value);

      value = settings.value(tr("/%1/peakParameters/%2/no%3/lambda").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("/%1/peakParameters/%2/no%3/lambda").arg(prefix).arg(id).arg(i), "1.0");
      waveLengths->at(i)->setValue(value);

      value = settings.value(tr("/%1/peakParameters/%2/no%3/epsilon").arg(prefix).arg(id).arg(i), "1.0").toDouble(&ok);
      CHECK_SETTINGS_CONVERSION(tr("/%1/peakParameters/%2/no%3/epsilon").arg(prefix).arg(id).arg(i), "1.0");
      //signValue = FFFTwoBoxWidget::calcSignificand(value, &expValue);
      //epsilon->at(i)->setValue(signValue, expValue, 500);
      epsilon->at(i)->setValueM(value, 500);
   }

}

FFFPeakParameterFrame::~FFFPeakParameterFrame()
{
   writeSettings();
   if(peakParWidgets) delete peakParWidgets; peakParWidgets = nullptr;
   if(peakParLayouts) delete peakParLayouts; peakParLayouts = nullptr;
   if(startTimes) delete startTimes; startTimes = nullptr;
   if(endTimes) delete endTimes; endTimes = nullptr;
   if(useSecondVirCoeffs) delete useSecondVirCoeffs; useSecondVirCoeffs = nullptr;
   if(secondVirCoeffs) delete secondVirCoeffs; secondVirCoeffs = nullptr;
   if(useThirdVirCoeffs) delete useThirdVirCoeffs; useThirdVirCoeffs = nullptr;
   if(thirdVirCoeffs) delete thirdVirCoeffs; thirdVirCoeffs = nullptr;
   if(refIndexIncrements) delete refIndexIncrements; refIndexIncrements = nullptr;
   if(waveLengths) delete waveLengths; waveLengths = nullptr;
   if(epsilon) delete epsilon; epsilon = nullptr;
   if(useKnownMW) delete useKnownMW; useKnownMW = nullptr;
   if(knownMWs) delete knownMWs; knownMWs = nullptr;
   if(fractionDescription) delete fractionDescription; fractionDescription = nullptr;
   if(removeButtons) delete removeButtons; removeButtons = nullptr;


}

void FFFPeakParameterFrame::addPeakParWidget()
{

   int i = peakParWidgets->size();
   peakParWidgets->append(new QWidget(areaWidget));
   peakParLayouts->append(new QGridLayout(peakParWidgets->at(i)));

   peakParLabels->append(new QLabel(tr("<b>Peak %1</b>").arg(i+1), peakParWidgets->at(i)));
   peakParLayouts->at(i)->addWidget(peakParLabels->at(i), 0, 0, 1, 3, Qt::AlignLeft);

   removeButtons->append(new FFFNumberedToolButton(i, peakParWidgets->at(i)));
   removeButtons->at(i)->setText("-");
   QObject::connect(removeButtons->at(i), SIGNAL(clickedID(int)), this, SLOT(removePeakParWidget(int)));
   peakParLayouts->at(i)->addWidget(removeButtons->at(i), 0, 3, 1, 1);

   peakParLayouts->at(i)->addWidget(new QLabel(tr("From:"), peakParWidgets->at(i)), 1, 0, 1, 2, Qt::AlignRight);
   startTimes->append(new QDoubleSpinBox(peakParWidgets->at(i)));
   //startTimes->at(i)->setEnabled(false);
   peakParLayouts->at(i)->addWidget(startTimes->at(i), 1, 2, 1, 3);
   peakParLayouts->at(i)->addWidget(new QLabel(tr("To:"), peakParWidgets->at(i)), 2, 0, 1, 2, Qt::AlignRight);
   endTimes->append(new QDoubleSpinBox(peakParWidgets->at(i)));
   //endTimes->at(i)->setEnabled(false);
   peakParLayouts->at(i)->addWidget(endTimes->at(i), 2, 2, 1, 3);
   for(QDoubleSpinBox* box : *startTimes) box->setEnabled(true);
   for(QDoubleSpinBox* box : *endTimes) box->setEnabled(true);

   peakParLayouts->at(i)->addWidget(new QLabel(tr("dn/dc:"), peakParWidgets->at(i)), 3, 0, 1, 2, Qt::AlignRight);
   refIndexIncrements->append(new QDoubleSpinBox(peakParWidgets->at(i)));
   refIndexIncrements->at(i)->setDecimals(5);
   peakParLayouts->at(i)->addWidget(refIndexIncrements->at(i), 3, 2, 1, 4);
   useSecondVirCoeffs->append(new QCheckBox(tr("B_2:"), peakParWidgets->at(i)));
   useSecondVirCoeffs->at(i)->setChecked(false);
   useSecondVirCoeffs->at(i)->setEnabled(false); // preliminary
   peakParLayouts->at(i)->addWidget(useSecondVirCoeffs->at(i), 4, 0, 1, 6);
   secondVirCoeffs->append(new FFFTwoBoxWidget(tr("second Virial Coefficient"),peakParWidgets->at(i)));
   //secondVirCoeffs->at(i)->setMaximum(9.99, 10);
   secondVirCoeffs->at(i)->setMaximum(9.99e10);
   secondVirCoeffs->at(i)->setEnabled(false);
   peakParLayouts->at(i)->addWidget(secondVirCoeffs->at(i), 5, 0, 1, 6);
   QObject::connect(useSecondVirCoeffs->at(i), SIGNAL(toggled(bool)), secondVirCoeffs->at(i), SLOT(setEnabled(bool)));
   useThirdVirCoeffs->append(new QCheckBox(tr("B_3:"), peakParWidgets->at(i)));
   useThirdVirCoeffs->at(i)->setChecked(false);
   useThirdVirCoeffs->at(i)->setEnabled(false); // preliminary
   peakParLayouts->at(i)->addWidget(useThirdVirCoeffs->at(i), 6, 0, 1, 6);
   thirdVirCoeffs->append(new FFFTwoBoxWidget(tr("second Virial Coefficient"),peakParWidgets->at(i)));
   //thirdVirCoeffs->at(i)->setMaximum(9.99, 10);
   thirdVirCoeffs->at(i)->setMaximum(9.99e10);
   thirdVirCoeffs->at(i)->setEnabled(false);
   peakParLayouts->at(i)->addWidget(thirdVirCoeffs->at(i), 7, 0, 1, 6);
   QObject::connect(useThirdVirCoeffs->at(i), SIGNAL(toggled(bool)), thirdVirCoeffs->at(i), SLOT(setEnabled(bool)));


   peakParLayouts->at(i)->addWidget(new QLabel(tr("lam [nm]:"), peakParWidgets->at(i)), 8, 0, 1, 2, Qt::AlignRight);
   waveLengths->append(new QDoubleSpinBox(peakParWidgets->at(i)));
   peakParLayouts->at(i)->addWidget(waveLengths->at(i), 8, 2, 1, 4, Qt::AlignLeft);
   waveLengths->at(i)->setEnabled(false); // prelim version

   peakParLayouts->at(i)->addWidget(new QLabel(tr("ext: [(mg/ml)/mAU ]"), peakParWidgets->at(i)), 9, 0, 1, 2, Qt::AlignRight);
   epsilon->append(new FFFTwoBoxWidget(tr("Molar Extinction Coefficient of Peak %1:").arg(i),  peakParWidgets->at(i)));
   epsilon->at(i)->setSignifandDecimals(5);
   peakParLayouts->at(i)->addWidget(epsilon->at(i), 9, 2, 1, 4, Qt::AlignLeft);
   epsilon->at(i)->setEnabled(false); // prelim version

   useKnownMW->append(new QCheckBox(tr("Use known MW"), peakParWidgets->at(i)));
   useKnownMW->at(i)->setToolTip(tr("\
                                    If this option ist chosen, the enterd molecular weight for \
                                    this peak will be used instead calcuating it from the SLS data.\
                                    "
                                    ));
   peakParLayouts->at(i)->addWidget(useKnownMW->at(i), 10, 0, 1, 6, Qt::AlignLeft);

   knownMWs->append(new FFFTwoBoxWidget("Determines fixed molecular weight."));
   peakParLayouts->at(i)->addWidget(knownMWs->at(i), 11, 0, 1, 6, Qt::AlignLeft);
   QObject::connect(useKnownMW->at(i), SIGNAL(toggled(bool)), knownMWs->at(i), SLOT(setEnabled(bool)));
   useKnownMW->at(i)->setEnabled(false);
   knownMWs->at(i)->setEnabled(false);
   peakParLayout->addWidget(peakParWidgets->at(i), 1);

}

void FFFPeakParameterFrame::removePeakParWidget(const int number)
{

   if(peakParWidgets->size() <= 1){
      FFFLog::logWarning(tr("You cannot remove the last Peak."));
      return;
   }


   delete peakParWidgets->at(number);
   peakParWidgets->removeAt(number);
   peakParLayouts->removeAt(number);
   peakParLabels->removeAt(number);
   startTimes->removeAt(number);
   endTimes->removeAt(number);
   useSecondVirCoeffs->removeAt(number);
   secondVirCoeffs->removeAt(number);
   useThirdVirCoeffs->removeAt(number);
   thirdVirCoeffs->removeAt(number);
   refIndexIncrements->removeAt(number);
   waveLengths->removeAt(number);
   epsilon->removeAt(number);
   useKnownMW->removeAt(number);
   knownMWs->removeAt(number);
   removeButtons->removeAt(number);
   for(int i = 0; i < peakParWidgets->size(); ++i){
      peakParLabels->at(i)->setText(tr("<b>Peak %1</b>").arg(i+1));
      removeButtons->at(i)->changeIdTo(i);
   }
   if(peakParWidgets->size() == 1) {
      startTimes->first()->setEnabled(false);
      endTimes->first()->setEnabled(false);
   }
}

void FFFPeakParameterFrame::writeSettings()
{

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   settings.setValue(tr("/%1/peakParameters/%2/numberOfPeaks").arg(prefix).arg(id), peakParWidgets->size());
   for(int i = 0; i < peakParWidgets->size(); ++i){
      settings.setValue(tr("/%1/peakParameters/%2/no%3/startTime").arg(prefix).arg(id).arg(i), startTimes->at(i)->value());
      settings.setValue(tr("/%1/peakParameters/%2/no%3/endTime").arg(prefix).arg(id).arg(i), endTimes->at(i)->value());
      settings.setValue(tr("/%1/peakParameters/%2/no%3/dndc").arg(prefix).arg(id).arg(i), refIndexIncrements->at(i)->value());
      settings.setValue(tr("/%1/peakParameters/%2/no%3/lambda").arg(prefix).arg(id).arg(i), waveLengths->at(i)->value());
      settings.setValue(tr("/%1/peakParameters/%2/no%3/epsilon").arg(prefix).arg(id).arg(i), epsilon->at(i)->value());
   }
}

void FFFPeakParameterFrame::adoptConcentrationMode(uint mode)
{
   qDebug() << mode;
   if(mode == 0){}
   else if(mode == 1){
      for(auto &e : *epsilon ) e->setEnabled(true);
   }

}




SampleProperties FFFPeakParameterFrame::getSampleProperties()
{
   SampleProperties p;
   p.startTimes = this->getStartTimes();
   p.endTimes = this->getEndTimes();
   p.secondVirCoeffs = this->getSecondVirCoeffs();
   p.secondVirUse = this->getSecondVirUse();
   p.thirdVirCoeffs = this->getThirdVirCoeffs();
   p.refIndexIncs = this->getRefIndexIncs();
   p.waveLengths = this->getWaveLengths();
   p.epsilon = this->getEpsilon();
   return p;
}
