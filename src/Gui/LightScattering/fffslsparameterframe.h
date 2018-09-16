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
#include "../../Core/af4parameterstructs.h"

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
   SLSPlotMode getSLSPlotMode();
   SLSConcMode getSLSConcMode();

   /*
   inline bool getZimmUse() const { return zimmPlot->isChecked(); }
   inline bool getBerryUse() const { return berryPlot->isChecked(); }
   inline bool getKratkyUse() const { return kratkyPlot->isChecked(); }
   inline bool getGuinierUse() const { return guinierPlot->isChecked(); }
   */
   //inline bool getRefIndexUse() const { return cFromRefIndex->isChecked(); }
   //inline bool getWaveLengthUse() const { return cFromWaveLength->isChecked(); }

   inline double getWaveLength() const { return laserWaveLength->value(); }
   inline double getRefIndexSolvent() const { return refIndexSolvent->value(); }

   void writeSettings() const;

signals:

public slots:

protected:
   QString prefix;
   int id;

   QGridLayout* lay        = nullptr;
   QGroupBox* evalGroup    = nullptr;
   QHBoxLayout* evalLayout = nullptr;
   QCheckBox* zimmPlot     = nullptr;
   QCheckBox* berryPlot    = nullptr;
   QCheckBox* kratkyPlot   = nullptr;
   QCheckBox* guinierPlot  = nullptr;

   QHBoxLayout* concLayout    = nullptr;
   QCheckBox* cFromRefIndex   = nullptr;
   QCheckBox* cFromWaveLength = nullptr;

   FFFTwoBoxWidget *concentrationCut = nullptr;
   QDoubleSpinBox *laserWaveLength   = nullptr;
   QDoubleSpinBox *refIndexSolvent   = nullptr;

private slots:

   void enableZimmPlotParams()   const;
   void enableBerryPlotParams()   const;
   void enableKratkyPlotParams()  const;
   void enableGuinierPlotParams() const;

   void enableRefIndex () const;
   void enableWaveLength () const;

};

#endif // FFFSLSPARAMETERFRAME_H
