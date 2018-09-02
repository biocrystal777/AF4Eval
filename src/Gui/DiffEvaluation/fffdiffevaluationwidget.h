#ifndef FFFDIFFEVALUATIONWIDGET_H
#define FFFDIFFEVALUATIONWIDGET_H

#include <vector>
#include <QApplication>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QCheckBox>
#include "./fffexpsettingsframe.h"
#include "../Calibration/af4calibsettingsframe.h"
#include "../Calibration/af4channeldimswidget.h"
#include "../Calibration/af4channelcalibwidget.h"
#include "../GeneralWidgets/ffflog.h"
#include "../GeneralWidgets/ffffileinoutwidget.h"
#include "../../Core/af4csvparser.h"
#include "../../Core/af4diffevaluator.h"
#include "../../Core/af4csvwriter.h"

class FFFSingleColumnChooseDialog : public QDialog
{
   Q_OBJECT
public:
   explicit FFFSingleColumnChooseDialog(const std::vector<std::string>& columnDelimiters,
                                        uint *timeIndex,
                                        QWidget* parent = nullptr);
   ~FFFSingleColumnChooseDialog(){}

   FFFSingleColumnChooseDialog(const FFFSingleColumnChooseDialog& src) = delete;
   FFFSingleColumnChooseDialog& operator= (FFFSingleColumnChooseDialog& src) = delete;
   FFFSingleColumnChooseDialog(FFFSingleColumnChooseDialog&& src) = delete;
   FFFSingleColumnChooseDialog& operator= (FFFSingleColumnChooseDialog&& src) = delete;

protected:
   QList<QRadioButton*> columnNames;
   uint *timeIndex                        = nullptr;
   QGridLayout *layout                    = nullptr;
   QGroupBox* groupBox                    = nullptr;
   QGridLayout *boxLayout                 = nullptr;
   QPushButton *accepter                  = nullptr;
   QPushButton *decliner                  = nullptr;

protected slots:
   void acceptAndgetIndex();
};


/**************************************************************
***
***  FFFEvaluationWidget
***      (using Qt 4.8.2)
***
**************************************************************/

/*!
 * \brief The FFFEvaluationWidget class
 * \details
 * \author Benedikt Haeusele
 * \version 1.0
 * \date March 2013
 * \copyright GNU General Public License version 3.0
 */

class FFFDiffEvaluationWidget : public QFrame {

   Q_OBJECT
public:

   /*!
    * \brief FFFEvaluationWidget the constructor of this class
    * \param channelConfigWidgets Pointer to the map of existing channelConfigWidgets
    * \param channelCalibWidgets Pointer to the map of existing channelCalibWidgets
    * \param parent
    */
   explicit FFFDiffEvaluationWidget(
         QMap<QString, AF4ChannelDimsWidget*>* channelConfigWidgets,
         QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>* channelCalibWidgets,
         QWidget *parent = nullptr);
   ~FFFDiffEvaluationWidget();

   FFFDiffEvaluationWidget(const FFFDiffEvaluationWidget& src) = delete;
   FFFDiffEvaluationWidget& operator= (FFFDiffEvaluationWidget& src) = delete;
   FFFDiffEvaluationWidget(FFFDiffEvaluationWidget&& src) = delete;
   FFFDiffEvaluationWidget& operator= (FFFDiffEvaluationWidget&& src) = delete;

   /*!
    * \brief getTemperatureVal returns the displayed temmperature
    * \return temperature
    */
   inline double getTemperatureVal() const { return expSettingsFrame->getTemperatureVal(); }

   /*!
    * \brief getVoidPeakTimeVal returns the displayed voidPeakTime
    * \return voidPeakTime
    */
   inline double getVoidPeakTimeVal() const { return expSettingsFrame->getVoidPeakTimeVal(); }

   /*!
    * \brief getRelFocusPointVal returns the displayed voidPeakTime
    * \return relFocusPoint
    */
   inline double getRelFocusPointVal() const { return expSettingsFrame->getRelFocusPointVal(); }

   /*!
    * \brief getElutionFlowVal returns the displayed elutionFlow
    * \return elutionFlow
    */
   inline double getElutionFlowVal() const { return expSettingsFrame->getElutionFlowVal(); }

   /*!
    * \brief getCrossFlowVal returns the displayed crossFlow
    * \return crossFlow
    */
   inline double getCrossFlowVal() const { return expSettingsFrame->getCrossFlowVal(); }

   /*!
    * \brief getLeftOffsetTimeVal returns the displayed leftOffsetTime (t0)
    * \return leftOffsetTime
    */
   inline double getLeftOffsetTimeVal() const { return expSettingsFrame->getLeftOffsetTimeVal(); }



   /*!
    * \brief adaptExpParameters adapts the required parameters.
    * \param tempVal temperature
    * \param eluFlowVal
    * \param crossFlowVal
    * \param voidPeakVal
    */
   void adaptExpParameters(double tempVal,
                           double eluFlowVal,
                           double crossFlowVal,
                           double voidPeakVal);

protected:

   QMap<QString, AF4ChannelDimsWidget*>                 *channelConfigWidgets   = nullptr;
   QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>  *channelCalibWidgets    = nullptr;

   QGridLayout           *widgetLayout           = nullptr;
   FFFFileInOutWidget    *fileWidget             = nullptr;
   AF4CalibSettingsFrame *calibSettingsFrame     = nullptr;
   FFFExpSettingsFrame   *expSettingsFrame       = nullptr;
   QFrame                *parameterSettingsFrame = nullptr;
   QPushButton           *settingsWriter         = nullptr;
   QPushButton           *evalStarter            = nullptr;
   QCheckBox             *displayZeroMessages    = nullptr;

protected slots:

   /*!
    * \brief startEvaluation starts the actual calculation
    */
   void startEvaluation();

   void saveParameters() const;

public slots:
   /*!
    * \brief adaptChannelParameters adapts the comboboxes according
    *        to the internal structures (QMaps) that contain
    *        the information about all existing channels and
    *        calibration profiles. Additionally, displayed
    *        values of the chosen values are updated
    */
   void adaptChannelParameters();

};

#endif // FFFDIFFEVALUATIONWIDGET_H

