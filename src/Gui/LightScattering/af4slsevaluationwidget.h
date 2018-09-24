#ifndef AF4SLSEVALUATIONWIDGET_H
#define AF4SLSEVALUATIONWIDGET_H

#include <QApplication>
#include <QRadioButton>
#include <QGroupBox>
#include <QList>
#include <vector>
#include <map>
#include <limits>
#include "./af4assigndatadialog.h"
#include "./af4slscalibrationframe.h"
#include "./af4peakparameterframe.h"
#include "../GeneralWidgets/af4log.h"
#include "../GeneralWidgets/af4fileinoutwidget.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../../Core/af4constants.h"
#include "../../Core/af4csvparser.h"
#include "../../Core/af4slsevaluator.h"
#include "../../Core/af4csvwriter.h"
#include "../../Core/af4parameterstructs.h"

/*! **********************************************************************************************
***
***  \class   AF4SLSEvaluationWidget "src/Gui/Calibration/af4slscalibrationframe.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   0.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/


class AF4SLSEvaluationWidget final : public QWidget
{
   Q_OBJECT
public:
   /*!
    * \brief FFFSLSEvaluationWidget
    * \param parent
    */
   explicit AF4SLSEvaluationWidget(QWidget *parent);
   /*!
    * \brief destructor
    *
    */
   ~AF4SLSEvaluationWidget();

   AF4SLSEvaluationWidget(const AF4SLSEvaluationWidget& src) = delete;
   AF4SLSEvaluationWidget& operator= (AF4SLSEvaluationWidget& src) = delete;
   AF4SLSEvaluationWidget(AF4SLSEvaluationWidget&& src) = delete;
   AF4SLSEvaluationWidget& operator= (AF4SLSEvaluationWidget&& src) = delete;

private:

   QGridLayout *widgetLayout                 = nullptr;
   AF4FileInOutWidget *fileWidget            = nullptr;
   AF4FileInOutWidget *gsfileWidget          = nullptr;

   AF4PeakParameterFrame *peakParameterFrame = nullptr;
   AF4SLSCalibrationFrame *calibrationFrame  = nullptr;

   QFrame* evaluationFrame                   = nullptr;
   QGridLayout* evaluationLayout             = nullptr;
   QGroupBox* evalGroup                      = nullptr;
   QHBoxLayout* evalLayout                   = nullptr;
   QRadioButton* zimmPlot                    = nullptr;
   QRadioButton* berryPlot                   = nullptr;
   QRadioButton* kratkyPlot                  = nullptr;
   QRadioButton* guinierPlot                 = nullptr;

   QGroupBox* concGroup                      = nullptr;
   QHBoxLayout* concLayout                   = nullptr;
   QRadioButton* refIndexConc                = nullptr;
   QRadioButton* absorbanceConc              = nullptr;

   QCheckBox* calculateShapeFactor           = nullptr;
   QCheckBox* evalPeaksSeparately            = nullptr;
   AF4SciNotSpinBox *concentrationCut         = nullptr;
   QDoubleSpinBox *laserWaveLength           = nullptr;
   QDoubleSpinBox* refIndexSolvent           = nullptr;

   QPushButton* settingsWriter               = nullptr;
   QPushButton* evalStarter                  = nullptr;

   /*!
    * \brief loadSettings
    */
   void loadSettings() const;
   /*!
    * \brief writeSettings
    */
   void writeSettings() const;

   /*!
    * \brief logChosenIndices
    * \param timeIndex
    * \param riIndices
    * \param mallsIndices
    * \param diffCoeffIndex
    * \param rSIndex
    * \param diffCoeffIndexChosen
    * \param rSIndexChosen
    */
   void logChosenIndices(uint timeIndex,
                         const QList<posPeakPair> &riIndices,
                         const QList<QList<posPeakPair> > &mallsIndices,
                         uint diffCoeffIndex = 0,
                         uint rSIndex = 0,
                         bool diffCoeffIndexChosen = false,
                         bool rSIndexChosen = false
         );
signals:

   /*!
    * \brief concModeChanged
    */
   void concModeChanged(SLSConcMode mode);

public slots:

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;

   /*!
    * \brief startEvaluation
    */
   void startEvaluation();

private slots:
   /*!
    * \brief emitConcModeChanged
    */
   void emitConcModeChanged();
};

#endif // AF4SLSEVALUATIONWIDGET_H
