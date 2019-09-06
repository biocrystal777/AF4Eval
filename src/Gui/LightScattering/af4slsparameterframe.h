#ifndef FFFSLSPARAMETERFRAME_H
#define FFFSLSPARAMETERFRAME_H

#include <QSettings>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../../Core/af4parameterstructs.h"
#include "../Core/af4evalmacros.h"

/*! **********************************************************************************************
***
***  \class   AF4SLSParameterFrame "src/Gui/Calibration/af4slsparameterframe.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   0.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/


class AF4SLSParameterFrame : public QFrame
{
   Q_OBJECT
public:
   /*!
    * \brief FFFSLSParameterFrame
    * \param prefix
    * \param id
    * \param parent
    */
   explicit AF4SLSParameterFrame(const QString &prefix, int id, QWidget *parent);

   /*!
    * \brief getSLSParameters
    * \return
    */
   SLSParameters getSLSParameters();

   /*!
    * \brief getSLSPlotMode
    * \return
    */
   SLSPlotMode getSLSPlotMode();
   /*!
    * \brief getSLSConcMode
    * \return
    */
   SLSConcMode getSLSConcMode();

   /*!
    * \brief getWaveLength
    * \return
    */
   inline double getWaveLength() const { return laserWaveLength->value(); }

   /*!
    * \brief getRefIndexSolvent
    * \return
    */
   inline double getRefIndexSolvent() const { return refIndexSolvent->value(); }

   /*!
    * \brief writeSettings
    */
   void writeSettings() const;

private:
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

   AF4SciNotSpinBox *concentrationCut = nullptr;
   QDoubleSpinBox *laserWaveLength   = nullptr;
   QDoubleSpinBox *refIndexSolvent   = nullptr;

   // Implement later as lamba when used!
/*
private slots:

   void enableZimmPlotParams()    const;
   void enableBerryPlotParams()   const;
   void enableKratkyPlotParams()  const;
   void enableGuinierPlotParams() const;
   void enableRefIndex ()         const;
   void enableWaveLength ()       const;   
   */
   NO_COPY_MOVE_CTORS(AF4SLSParameterFrame)
};

#endif // FFFSLSPARAMETERFRAME_H
