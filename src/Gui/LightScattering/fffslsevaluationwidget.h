#ifndef FFFSLSEVALUATIONWIDGET_H
#define FFFSLSEVALUATIONWIDGET_H

#include <QApplication>
#include <QRadioButton>
#include <QGroupBox>
#include <QList>
#include <vector>
#include <map>
#include <limits>
#include "../GeneralWidgets/af4log.h"
//#include "../GeneralWidgets/fffmultiplefilewidget.h"
#include "../GeneralWidgets/af4fileinoutwidget.h"
#include "./af4assigndatadialog.h"
#include "./fffslscalibrationframe.h"
//#include "./fffpeakparameterframe.h"
#include "./af4peakparameterframe.h"
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../../Core/af4constants.h"
#include "../../Core/af4csvparser.h"
#include "../../Core/af4slsevaluator.h"
#include "../../Core/af4csvwriter.h"
#include "../../Core/af4parameterstructs.h"

class FFFSLSEvaluationWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FFFSLSEvaluationWidget(QWidget *parent = nullptr);
   ~FFFSLSEvaluationWidget();

   FFFSLSEvaluationWidget(const FFFSLSEvaluationWidget& src) = delete;
   FFFSLSEvaluationWidget& operator= (FFFSLSEvaluationWidget& src) = delete;
   FFFSLSEvaluationWidget(FFFSLSEvaluationWidget&& src) = delete;
   FFFSLSEvaluationWidget& operator= (FFFSLSEvaluationWidget&& src) = delete;

protected:

   QGridLayout *widgetLayout              = nullptr;
   //FFFMultipleFileWidget *inputFileWidget = nullptr;

   AF4FileInOutWidget *fileWidget         = nullptr;
   AF4FileInOutWidget *gsfileWidget       = nullptr;

   //FFFPeakParameterFrame *peakParameterFrame = nullptr;
   AF4PeakParameterFrame *peakParameterFrame = nullptr;
   FFFSLSCalibrationFrame *calibrationFrame  = nullptr;

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
   FFFTwoBoxWidget *concentrationCut         = nullptr;
   QDoubleSpinBox *laserWaveLength           = nullptr;
   QDoubleSpinBox* refIndexSolvent           = nullptr;

   QPushButton* settingsWriter               = nullptr;
   QPushButton* evalStarter                  = nullptr;



   void loadSettings() const;
   void writeSettings() const;

   void logChosenIndices(uint timeIndex,
                         QList<posPeakPair> * riIndices,
                         QList<QList<posPeakPair> > *mallsIndices,
                         uint diffCoeffIndex = 0,
                         uint rSIndex = 0,
                         bool diffCoeffIndexChosen = false,
                         bool rSIndexChosen = false
         );
signals:

   void concModeChanged(SLSConcentrationMode);

public slots:

   /*!
    * \brief chooseOutputFile opens a dialog to choose a folder and filename
    *                         for the output file
    */
   void saveParameters() const;
   void startEvaluation();

private slots:

   void emitConcModeChanged(bool dummy);

};

#endif // FFFSLSEVALUATIONWIDGET_H
