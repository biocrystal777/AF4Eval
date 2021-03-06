#ifndef AF4DIFFEVALUATIONWIDGET_H
#define AF4DIFFEVALUATIONWIDGET_H

#include <vector>
#include <QApplication>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QCheckBox>
#include "./af4expsettingsframe.h"
#include "../Calibration/af4calibsettingsframe.h"
#include "../Calibration/af4channeldimswidget.h"
#include "../Calibration/af4channelcalibwidget.h"
#include "../GeneralWidgets/af4log.h"
#include "../GeneralWidgets/af4fileinoutwidget.h"
#include "../Core/af4evalmacros.h"
#include "../../Core/af4csvparser.h"
#include "../../Core/af4diffevaluator.h"
#include "../../Core/af4csvwriter.h"

/*! **********************************************************************************************
***
***  \class     AF4DiffEvaluationWidget "src/Gui/Calibration/af4diffevaluationwidget.h"
***  \brief     AF4DiffEvaluationWidget Small GUI for quick estimations of Dvia classical
***                                               Stokes Einstein equation
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4DiffEvaluationWidget final : public QFrame {

   Q_OBJECT
public:

   /*!
    * \brief FFFEvaluationWidget the constructor of this class
    * \param channelConfigWidgets Pointer to the map of existing channelConfigWidgets
    * \param channelCalibWidgets Pointer to the map of existing channelCalibWidgets
    * \param parent
    */
   AF4DiffEvaluationWidget(
         QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets,
         QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > channelCalibWidgets,
         QWidget *parent);
   ~AF4DiffEvaluationWidget(){}

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
    *
    *
    *
    *
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

public slots:
   /*!
    * \brief adaptChannelParameters adapts the comboboxes according
    *        to the internal structures (QMaps) that contain
    *        the information about all existing channels and
    *        calibration profiles. Additionally, displayed
    *        values of the chosen values are updated
    */
   //void adaptChannelParameters();

private slots:
   /*!
    * \brief startEvaluation starts the actual calculation
    */
   void startEvaluation();

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;

private:
   QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> >   channelConfigWidgets;
   //QMap<QString, QMap<QString, AF4ChannelCalibWidget*>*>  *channelCalibWidgets    = nullptr;
   QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > channelCalibWidgets;

   QGridLayout           *widgetLayout           = nullptr;
   AF4FileInOutWidget    *fileWidget             = nullptr;
   AF4CalibSettingsFrame *calibSettingsFrame     = nullptr;
   AF4ExpSettingsFrame   *expSettingsFrame       = nullptr;
   QFrame                *parameterSettingsFrame = nullptr;
   QPushButton           *settingsWriter         = nullptr;
   QPushButton           *evalStarter            = nullptr;
   QCheckBox             *displayZeroMessages    = nullptr;

   NO_COPY_MOVE_CTORS(AF4DiffEvaluationWidget)
};

#endif // AF4DIFFEVALUATIONWIDGET_H

