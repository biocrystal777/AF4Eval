#include "af4peakparameterframe.h"

/*-*********************************************************
 *
 *
 *    AF4SinglePeakParameterWidget methods
 *
 *
 **********************************************************/

AF4SinglePeakParameterWidget::AF4SinglePeakParameterWidget(int peakNo,
                                                           bool checkSettings,
                                                           const QString &parentPrefix,
                                                           const int parentId,
                                                           QWidget *parent)
   : QFrame(parent), peakNo(peakNo), parentPrefix(parentPrefix), parentId(parentId)// : AF4SingelPeakParameterWidget(parent)
{
   this->setFrameStyle(0x1011);
   lay       = new QGridLayout(this);
   //label     = new QLabel(this);

   removeButton      = new FFFNumberedToolButton(peakNo, this);
   removeButton->setText("-");
   QObject::connect(removeButton, SIGNAL(clickedID(int)), parent, SLOT(removePeakParWidget(int)));
   lay->addWidget(removeButton, 0, 2, 1, 1);

   lay->addWidget(new QLabel("From:", this), 1, 0 , 1, 1, Qt::AlignRight);
   startTime = new QDoubleSpinBox(this);
   lay->addWidget(startTime, 1,1,1,3);

   lay->addWidget(new QLabel("To:", this), 2, 0 , 1, 1, Qt::AlignRight);
   endTime = new QDoubleSpinBox(this);
   lay->addWidget(endTime, 2, 1, 1, 3);

   lay->addWidget(new QLabel(tr("dn/dc:"), this), 3, 0 , 1, 1, Qt::AlignRight);
   refIndexIncrement = new QDoubleSpinBox(this);
   refIndexIncrement->setDecimals(5);
   lay->addWidget(refIndexIncrement, 3, 1, 1, 4);

   useSecondVirCoeff = new QCheckBox( tr("B_2:"), this);
   useSecondVirCoeff->setChecked(false); //prelim
   secondVirCoeff = new AF4SciNotSpinBox(false, this);
   secondVirCoeff->setToolTip("Second Virial Coefficient");
   secondVirCoeff->setMaximum(9.99e10);
   secondVirCoeff->setEnabled(false);
   QObject::connect(useSecondVirCoeff, SIGNAL(toggled(bool)), secondVirCoeff, SLOT(setEnabled(bool)));
   lay->addWidget(useSecondVirCoeff, 4, 0, 1, 6);
   lay->addWidget(secondVirCoeff, 5, 0, 1, 6);

   useThirdVirCoeff = new QCheckBox( tr("B_3:"), this);
   useThirdVirCoeff->setChecked(false); //prelim
   thirdVirCoeff = new AF4SciNotSpinBox(false, this);
   thirdVirCoeff->setToolTip("Third Virial Coefficient" );
   thirdVirCoeff->setMaximum(9.99e10);
   thirdVirCoeff->setEnabled(false);
   QObject::connect(useThirdVirCoeff, SIGNAL(toggled(bool)), thirdVirCoeff, SLOT(setEnabled(bool)));
   lay->addWidget(useThirdVirCoeff, 6, 0, 1, 6);
   lay->addWidget(thirdVirCoeff, 7, 0, 1, 6);

   lay->addWidget(new QLabel(tr("λ<sub>ext</sub>/nm:"), this), 8, 0, 1, 1, Qt::AlignRight);
   waveLength = new QDoubleSpinBox(this);
   lay->addWidget(waveLength, 8, 1, 1, 4, Qt::AlignLeft);
   waveLength->setEnabled(false); // prelim version

   lay->addWidget(new QLabel(tr("ε/<sub>(mg/ml*mAU):</sub>"), this), 9, 0, 1, 1, Qt::AlignRight);
   epsilon = new AF4SciNotSpinBox(false, this);
   epsilon->setToolTip(tr("Molar Extinction Coefficient of Peak %1:").arg(peakNo));
   epsilon->setSignifandDecimals(5);
   lay->addWidget(epsilon, 9, 1, 1, 4, Qt::AlignLeft);
   epsilon->setEnabled(false); // prelim version

   // DISABLED AS NOT IMPLEMENTED YET IN CALCULATOR CLASS!
   //
   //
   useKnownMW = new QCheckBox(tr("Use known MW"), this);
   useKnownMW->setToolTip(tr("\
                             If this option ist chosen, the enterd molecular weight for \
                             this peak will be used instead calcuating it from the SLS data.\
                             "
                             ));
   lay->addWidget(useKnownMW, 10, 0, 1, 6, Qt::AlignLeft);

   knownMW = new AF4SciNotSpinBox(false, this);
   knownMW->setToolTip("Determines fixed molecular weight.");
   lay->addWidget(knownMW, 11, 0, 1, 6, Qt::AlignLeft);
   QObject::connect(useKnownMW, SIGNAL(toggled(bool)), knownMW, SLOT(setEnabled(bool)));
   // DISABLED HERE
   useKnownMW->setEnabled(false);
   knownMW->setEnabled(false);

   if(checkSettings) loadSettings();

   //
   //
   // DISABLED AS NOT IMPLEMENTED YET IN CALCULATOR CLASS!
}

AF4SinglePeakParameterWidget::~AF4SinglePeakParameterWidget()
{
   writeSettings();
}

void AF4SinglePeakParameterWidget::changePeakNo(int newPeakNo)
{
   this->peakNo = newPeakNo;
   removeButton->changeIdTo(newPeakNo);
   epsilon->setToolTip(tr("Molar Extinction Coefficient of Peak %1:").arg(newPeakNo));
}

void AF4SinglePeakParameterWidget::makeSingleSPPW(bool makeSingle)
{
   startTime->setDisabled(makeSingle);
   endTime->setDisabled(makeSingle);
}


void AF4SinglePeakParameterWidget::adaptConcentrationMode(SLSConcMode mode)
{
   switch (mode) {
   case SLSConcMode::FromRI:
      break;
   case SLSConcMode::FromUVVis:
      break;
   default:
      AF4Log::logError(tr("Blame programmer for undefinded behaviour in AF4SinglePeakParameterWidget"));
   }
}

Peakproperties AF4SinglePeakParameterWidget::getPeakProperties() const {
   return Peakproperties {
      this->getStartTime(),
            this->getEndTime(),
            this->getSecondVirUse(),
            this->getSecondVirCoeff(),
            this->getThirdVirUse(),
            this->getThirdVirCoeff(),
            this->getRefIndexInc(),
            this->getWaveLength(),
            this->getEpsilon()
   };
}

void AF4SinglePeakParameterWidget::loadSettings()
{
   bool ok(false);
   double value(0.0);
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   QString key;

   //QString prefix = parent()->getPrefix();

#define CHECK_SETTINGS_CONVERSION(defaultValue) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2.") \
   .arg(key).arg(defaultValue)); \
}\
};

   key = tr("/%1/peakParameters/%2/no%3/startTime").arg(parentPrefix).arg(parentId).arg(peakNo);
   value = settings.value(key, "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("1.0");
   startTime->setValue(value);

   key = tr("/%1/peakParameters/%2/no%3/endTime").arg(parentPrefix).arg(parentId).arg(peakNo);
   value = settings.value(key, "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("1.0");
   endTime->setValue(value);

   key = tr("/%1/peakParameters/%2/no%3/dndc").arg(parentPrefix).arg(parentId).arg(peakNo);
   value = settings.value(key, "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("1.0");
   refIndexIncrement->setValue(value);

   key = tr("/%1/peakParameters/%2/no%3/lambda").arg(parentPrefix).arg(parentId).arg(peakNo);
   value = settings.value(key, "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("1.0");
   waveLength->setValue(value);

   key = tr("/%1/peakParameters/%2/no%3/epsilon").arg(parentPrefix).arg(parentId).arg(peakNo);
   value = settings.value(key, "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION("1.0");
   epsilon->setValue(value);

#undef CHECK_SETTINGS_CONVERSION

}

void AF4SinglePeakParameterWidget::writeSettings()
{

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   settings.setValue(tr("/%1/peakParameters/%2/no%3/startTime").arg(parentPrefix).arg(parentId).arg(peakNo), startTime->value());
   settings.setValue(tr("/%1/peakParameters/%2/no%3/endTime").arg(parentPrefix).arg(parentId).arg(peakNo), endTime->value());
   settings.setValue(tr("/%1/peakParameters/%2/no%3/dndc").arg(parentPrefix).arg(parentId).arg(peakNo), refIndexIncrement->value());
   settings.setValue(tr("/%1/peakParameters/%2/no%3/lambda").arg(parentPrefix).arg(parentId).arg(peakNo), waveLength->value());
   settings.setValue(tr("/%1/peakParameters/%2/no%3/epsilon").arg(parentPrefix).arg(parentId).arg(peakNo), epsilon->value());
}

/*-*********************************************************
 *
 *
 *    AF4PeakParameterFrame methods
 *
 *
 **********************************************************/


AF4PeakParameterFrame::AF4PeakParameterFrame(const QString &prefix, const int id, QWidget *parent) : QFrame(parent), prefix(prefix), id(id)
{

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

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok(false);
   int numberOfPeaks = settings.value(tr("/%1/peakParameters/%2/numberOfPeaks").arg(prefix).arg(id), 1).toInt(&ok);
   if(numberOfPeaks < 1) numberOfPeaks = 1;
   if(!ok) AF4Log::logWarning(tr("Could not read number Of Peaks."));
   peakParWidgets.clear();
   for(int i = 0; i < numberOfPeaks; ++i){
      peakParWidgets.append(new AF4SinglePeakParameterWidget(i, true, prefix, id, this));
      peakParLayout->addWidget(peakParWidgets.last());
   }
}

AF4PeakParameterFrame::~AF4PeakParameterFrame()
{
   writeSettings();
}

SampleProperties AF4PeakParameterFrame::getSampleProperties() const
{
   return SampleProperties {
      getStartTimes(),
            getEndTimes(),
            getSecondVirUses(),
            getSecondVirCoeffs(),
            getThirdVirUses(),
            getThirdVirCoeffs(),
            getRefIndexIncs(),
            getWaveLengths(),
            getEpsilons()
   };
}

QList<double> AF4PeakParameterFrame::getStartTimes() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getStartTime());
   return l;
}

QList<double> AF4PeakParameterFrame::getEndTimes() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getEndTime());
   return l;
}

QList<bool> AF4PeakParameterFrame::getSecondVirUses() const
{
   QList<bool> l;
   for(auto p : peakParWidgets) l.append(p->getSecondVirUse());
   return l;
}

QList<double> AF4PeakParameterFrame::getSecondVirCoeffs() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getSecondVirCoeff());
   return l;
}

QList<bool> AF4PeakParameterFrame::getThirdVirUses() const
{
   QList<bool> l;
   for(auto p : peakParWidgets) l.append(p->getThirdVirUse());
   return l;
}

QList<double> AF4PeakParameterFrame::getThirdVirCoeffs() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getThirdVirCoeff());
   return l;
}

QList<double> AF4PeakParameterFrame::getRefIndexIncs() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getRefIndexInc());
   return l;
}

QList<double> AF4PeakParameterFrame::getWaveLengths() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getWaveLength());
   return l;
}

QList<double> AF4PeakParameterFrame::getEpsilons() const
{
   QList<double> l;
   for(auto p : peakParWidgets) l.append(p->getEpsilon());
   return l;
}

QList<QString> AF4PeakParameterFrame::getFractionDescriptions() const
{
   QList<QString> l;
   for(auto p : peakParWidgets) l.append(p->getFractionDescription());
   return l;
}

void AF4PeakParameterFrame::writeSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("/%1/peakParameters/%2/numberOfPeaks").arg(prefix).arg(id), peakParWidgets.size());
}

void AF4PeakParameterFrame::adoptConcentrationMode(SLSConcMode mode)
{
   for(auto p : peakParWidgets) p->adaptConcentrationMode(mode);
}

void AF4PeakParameterFrame::addPeakParWidget()
{
   if(peakParWidgets.size() == 1 ) peakParWidgets.first()->makeSingleSPPW(false);
   peakParWidgets.append(new AF4SinglePeakParameterWidget(peakParWidgets.size(), false, this->prefix, this->id, this));
   peakParLayout->addWidget(peakParWidgets.last());
}

void AF4PeakParameterFrame::removePeakParWidget(const int peakNo)
{
   if(peakParWidgets.size() <= 1)
      AF4Log::logWarning(tr("You cannot remove the last Peak."));
   else {
      delete peakParWidgets.takeAt(peakNo);
      for(int i = 0; i < peakParWidgets.size(); ++i){
         peakParWidgets[i]->changePeakNo(i);
      }
      if(peakParWidgets.size() == 1) peakParWidgets.first()->makeSingleSPPW(true);
   }
}
