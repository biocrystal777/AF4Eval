#include "fffslsparameterframe.h"

FFFSLSParameterFrame::FFFSLSParameterFrame(const QString &prefix, int id, QWidget *parent) :
   QFrame(parent), prefix(prefix), id(id)
{
   /////// Evaluation method
   this->setFrameStyle(0x1011);
   lay = new QGridLayout(this);

   zimmPlot = new QCheckBox("Zimm", this);
   zimmPlot->setChecked(true);
   QObject::connect(zimmPlot, SIGNAL(toggled(bool)), this, SLOT(enableZimmPlotParams()));
   lay->addWidget(zimmPlot, 0, 0, 1, 2);

   kratkyPlot = new QCheckBox("Kratky", this);
   QObject::connect(kratkyPlot, SIGNAL(toggled(bool)), this, SLOT(enableKratkyPlotParams()));
   lay->addWidget(kratkyPlot, 0, 2, 1, 2);
   kratkyPlot->setEnabled(false);

   berryPlot = new QCheckBox("Berry", this);
   QObject::connect(berryPlot, SIGNAL(toggled(bool)), this, SLOT(enableBerryPlotParams()));
   lay->addWidget(berryPlot, 0, 4, 1, 2);
   berryPlot->setEnabled(false);

   guinierPlot = new QCheckBox("Guinier", this);
   QObject::connect(guinierPlot, SIGNAL(toggled(bool)), this, SLOT(enableGuinierPlotParams()));
   lay->addWidget(guinierPlot, 0, 6, 1, 2);
   guinierPlot->setEnabled(false);

   cFromRefIndex = new QCheckBox("From Refractive Index", this);
   cFromRefIndex->setChecked(true);
   QObject::connect(cFromRefIndex, SIGNAL(toggled(bool)), this, SLOT(enableRefIndex()));
   lay->addWidget(cFromRefIndex, 1, 0, 1, 5);

   cFromWaveLength = new QCheckBox("From Absorption",this);
   cFromWaveLength->setEnabled(false);
   QObject::connect(cFromWaveLength, SIGNAL(toggled(bool)), this, SLOT(enableWaveLength()));
   lay->addWidget(cFromWaveLength, 2, 0, 1, 5);

   lay->addWidget(new QLabel(tr("c<sub>min</sub> [mg/ml]"), this), 3, 0, 1, 1, Qt::AlignRight);
   concentrationCut = new FFFTwoBoxWidget(QString("Minimum concentration to be evaluated"),this);
//   concentrationCut->setMinimum(1.0, -15);
   concentrationCut->setMinimum(1.0e-15);
   lay->addWidget(concentrationCut, 3, 1, 1, 4);

   lay->addWidget(new QLabel(tr("lambda<sub>laser</sub>"), this), 4, 0, 1, 1, Qt::AlignRight);
   laserWaveLength = new QDoubleSpinBox(this);
   laserWaveLength->setDecimals(1);
   laserWaveLength->setMinimum(100.0);
   laserWaveLength->setMaximum(1000.0);
   lay->addWidget(laserWaveLength, 4, 1, 1, 4);

   lay->addWidget(new QLabel(tr("n0:"), this), 5, 0, 1, 1, Qt::AlignLeft);
   refIndexSolvent = new QDoubleSpinBox(this);
   refIndexSolvent->setDecimals(5);
   refIndexSolvent->setToolTip("Refractive Index of the solvent");
   lay->addWidget(refIndexSolvent, 5, 1, 1, 4, Qt::AlignLeft);

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok(false);
   double value(0.0);

#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };

   value = settings.value(tr("/%1/%2/slsparam/n0").arg(prefix).arg(id), "1.3332").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("/%1/%2/slsparam/n0").arg(prefix).arg(id), "1.3332");
   refIndexSolvent->setValue(value);

   value = settings.value(tr("/%1/%2/slsparam/laserWaveLength").arg(prefix).arg(id), "632.8").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("/%1/%2/slsparam/laserWaveLength").arg(prefix).arg(id), "632.8");
   laserWaveLength->setValue(value);

   value = settings.value(tr("/%1/%2/slsparam/concentrationCut").arg(prefix).arg(id), "1.0").toDouble(&ok);
   CHECK_SETTINGS_CONVERSION(tr("/%1/%2/slsparam/concentrationCut").arg(prefix).arg(id), "1.0");
   //signValue = FFFTwoBoxWidget::calcSignificand(value, &expValue);
   //concentrationCut->setValue(signValue, expValue, 520);
   concentrationCut->setValueM(value, 520);

#undef CHECK_SETTINGS_CONVERSION
}




void FFFSLSParameterFrame::writeSettings() const
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   settings.setValue(tr("/%1/%2/slsparam/n0").arg(prefix).arg(id), refIndexSolvent->value());
   settings.setValue(tr("/%1/%2/slsparam/laserWaveLength").arg(prefix).arg(id), laserWaveLength->value());
   settings.setValue(tr("/%1/%2/slsparam/concentrationCut").arg(prefix).arg(id), concentrationCut->value());
}

void FFFSLSParameterFrame::enableZimmPlotParams() const
{
}

void FFFSLSParameterFrame::enableBerryPlotParams() const
{
}

void FFFSLSParameterFrame::enableKratkyPlotParams() const
{
}

void FFFSLSParameterFrame::enableGuinierPlotParams() const
{
}

void FFFSLSParameterFrame::enableRefIndex() const
{
}

void FFFSLSParameterFrame::enableWaveLength() const
{
}

SLSParameters FFFSLSParameterFrame::getSLSParameters()
{
   return SLSParameters {
      getWaveLength(),
            getRefIndexSolvent()
   };
}

SLSPlotMode FFFSLSParameterFrame::getSLSPlotMode()
{
   SLSPlotMode mode{SLSPlotMode::Zimm};
   if     (zimmPlot->isChecked())    mode = SLSPlotMode::Zimm;
   else if(berryPlot->isChecked())   mode = SLSPlotMode::Berry;
   else if(kratkyPlot->isChecked())  mode = SLSPlotMode::Kratky;
   else if(guinierPlot->isChecked()) mode = SLSPlotMode::Guinier;
   return mode;

}

SLSConcMode FFFSLSParameterFrame::getSLSConcMode()
{
   SLSConcMode mode(SLSConcMode::FromRI);
   if      (cFromRefIndex->isChecked())   mode = SLSConcMode::FromRI;
   else if (cFromWaveLength->isChecked()) mode = SLSConcMode::FromUVVis;
   return mode;
}
