#ifndef FFFSLSEVALUATIONWIDGET_H
#define FFFSLSEVALUATIONWIDGET_H

#include <QApplication>
#include <QRadioButton>
#include <QGroupBox>
#include <QList>
#include <vector>
#include <map>
#include <limits>
#include "../GeneralWidgets/ffflog.h"
#include "../GeneralWidgets/fffmultiplefilewidget.h"
#include "../GeneralWidgets/ffffileinoutwidget.h"
#include "./fffassigndatadialog.h"
#include "./fffslscalibrationframe.h"
#include "./fffpeakparameterframe.h"
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../../Core/fffconstants.h"
#include "../../Core/fffcsvparser.h"
#include "../../Core/fffslsevaluator.h"
#include "../../Core/fffcsvwriter.h"
#include "../../Core/fffparameterstructs.h"

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
   FFFMultipleFileWidget *inputFileWidget = nullptr;

   FFFFileInOutWidget *fileWidget         = nullptr;
   FFFFileInOutWidget *gsfileWidget       = nullptr;

   FFFPeakParameterFrame *peakParameterFrame = nullptr;
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


private:

signals:

   void concModeChanged(uint mode);

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
