#ifndef FFFSLSPARAMETERFRAME_H
#define FFFSLSPARAMETERFRAME_H

#include <QSettings>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../../Core/fffparameterstructs.h"

class FFFSLSParameterFrame : public QFrame
{
   Q_OBJECT
public:
   explicit FFFSLSParameterFrame(const QString &prefix, int id, QWidget *parent = nullptr);

   FFFSLSParameterFrame(const FFFSLSParameterFrame& src) = delete;
   FFFSLSParameterFrame& operator= (FFFSLSParameterFrame& src) = delete;
   FFFSLSParameterFrame(FFFSLSParameterFrame&& src) = delete;
   FFFSLSParameterFrame& operator= (FFFSLSParameterFrame&& src) = delete;

   SLSParameters getSLSParameters();

   inline bool getZimmUse() const { return zimmPlot->isChecked(); }
   inline bool getBerryUse() const { return berryPlot->isChecked(); }
   inline bool getKratkyUse() const { return kratkyPlot->isChecked(); }
   inline bool getGuinierUse() const { return guinierPlot->isChecked(); }
   inline bool getRefIndexUse() const { return cFromRefIndex->isChecked(); }
   inline bool getWaveLengthUse() const { return cFromWaveLength->isChecked(); }

   inline double getWaveLength() const { return laserWaveLength->value(); }
   inline double getRefIndexSolvent() const { return refIndexSolvent->value(); }

   void writeSettings() const;

signals:

public slots:

protected:
   QString prefix;
   int id;

   QGridLayout* lay;
   QGroupBox* evalGroup;
   QHBoxLayout* evalLayout;
   QCheckBox* zimmPlot;
   QCheckBox* berryPlot;
   QCheckBox* kratkyPlot;
   QCheckBox* guinierPlot;


   QHBoxLayout* concLayout;
   QCheckBox* cFromRefIndex;
   QCheckBox* cFromWaveLength;

   FFFTwoBoxWidget *concentrationCut;
   QDoubleSpinBox *laserWaveLength;
   QDoubleSpinBox* refIndexSolvent;

protected slots:

   void enableZimmPlotParams(bool enable) const;
   void enableBerryPlotParams(bool enable) const;
   void enableKratkyPlotParams(bool enable) const;
   void enableGuinierPlotParams(bool enable) const;

   void enableRefIndex (bool enable) const;
   void enableWaveLength (bool enable) const;
};

#endif // FFFSLSPARAMETERFRAME_H
