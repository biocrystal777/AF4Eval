#ifndef AF4SIGNALPLOT_H
#define AF4SIGNALPLOT_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_text_label.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_widget_overlay.h>
#include "../Core/af4evalmacros.h"
#include "../Core/af4evalmacros.h"
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../../Core/af4datatypes.h"

/*! *****************************************************************************************************************
***
***  \class     QwtDynPlotMarker "src/Gui/Calibration/af4calibplotwidget.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright LGPL v3.0
***  \brief     Facultatively responsive Qwtplotmarker that can emit a signal when its position has been shifted
***
*********************************************************************************************************************/

class QwtDynPlotMarker : public QObject, public QwtPlotMarker
{
   Q_OBJECT

public:
   explicit QwtDynPlotMarker( const QString &title = QString::null ) : QwtPlotMarker( title ) {}
   explicit QwtDynPlotMarker( const QwtText &title )                 : QwtPlotMarker( title ) {}

signals:
   /*!
    * \brief posChanged Emitted when the x position is changed via xvalue slot
    */
   void posChanged();

public slots:

   /*!
    * \brief setXValueSlot
    * \param xValue
    */
   void setXValueEmit(double xValue){
      setXValue(xValue);
      emit posChanged();
   }


};


/* Implement  later to speed up draw events

class VLineOverlay : public QwtWidgetOverlay {

  public:
   VLineOverlay( QWidget *QwtPlot ) : QwtWidgetOverlay(QwtPlot) {}

};
*/

/*! ***************************************************************************************
***
***  \class     AF4SignalPlot "src/Gui/Calibration/af4signalplot.h"
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018
***  \copyright LGPL v3.0
***
********************************************************************************************/


class AF4SignalPlot final : public QWidget
{
   Q_OBJECT

public:

   /*!
    * \brief FFFCalibPlotWidget is default constructor
    * \param title
    * \param parent
    */
   AF4SignalPlot(const QString& title, QWidget *parent);

   /*!
    * \brief initPlot
    */
   void initPlot();

   /*!
    * \brief setSignal1Channels
    * \param strs
    */
   void setSignal1Channels(const QStringList& strs);

   /*!
    * \brief setSignal2Channels
    * \param strs
    */
   void setSignal2Channels(const QStringList& strs);

   /*!
    * \brief setPlotXData
    * \param newXData
    */
   void setPlotXData(QVector<double> &newXData)      { plotXData = newXData; }

   /*!
    * \brief setPlotYData
    * \param newYData
    */
   void setPlotYData(QVecMatrix<double> &newYData)   { plotYData = newYData; }

   /*!
    * \brief addPlotVLine
    * \param ctrlBox
    * \param color
    */
   void addPlotVLine(const QString &markerName, const QColor &color);
   
   /*!
    * \brief connectToPlotItem
    * \param ctrlBox
    * \param markerName
    */
   void connectToPlotItem(QDoubleSpinBox *const ctrlBox, const QString &markerName);

   /*!
    * \brief disconnectFromPlotItem
    * \param ctrlBox
    * \param markerName
    */
   void disconnectFromPlotItem(QDoubleSpinBox *const ctrlBox, const QString &markerName);


public slots:

   /*!
    * \brief autoScaleY1Axis
    */
   void autoScaleY1Axis();

   /*!
    * \brief autoScaleY2Axis
    */
   void autoScaleY2Axis();

   /*!
    * \brief setXScale
    * \param minX
    * \param maxX
    */
   void setXScale(double minX, double maxX);

private:
   /*!
    * \brief destructor of this class
    */
   ~AF4SignalPlot();

   void defaultInitData();
   /*!
    * \brief updatePlot updataes the data set and repaints plot
    */
   void updatePlot();

   QGridLayout             *lay               = nullptr;
   QwtPlot                 *plot              = nullptr;
   QwtPlotGrid             *grid              = nullptr;
   QwtPlotCurve            *signal1Curve      = nullptr;
   QwtPlotCurve            *signal2Curve      = nullptr;

   QDoubleSpinBox          *scaleXRangeMin    = nullptr;
   QDoubleSpinBox          *scaleXRangeMax    = nullptr;
   AF4SciNotSpinBox        *scaleY1RangeMin   = nullptr;
   AF4SciNotSpinBox        *scaleY1RangeMax   = nullptr;
   QPushButton             *autoScaleY1Button = nullptr;
   AF4SciNotSpinBox        *scaleY2RangeMin   = nullptr;
   AF4SciNotSpinBox        *scaleY2RangeMax   = nullptr;
   QPushButton             *autoScaleY2Button = nullptr;

   QVector<double>          plotXData;
   QVecMatrix<double>       plotYData;
   int signal1Ch = 0;
   int signal2Ch = 0;
   QComboBox               *signal1Switch     = nullptr;
   QComboBox               *signal2Switch     = nullptr;
   //QList<QwtDynPlotMarker*> plotMarkers;
   //QList<QwtSymbol*>        symbols;
   QMap<QString, QwtDynPlotMarker*> plotMarkers;
   QMap<QString, QwtSymbol*>        symbols;

   NO_COPY_ASSIGNMENT_CTORS(AF4SignalPlot)
};
#endif // AF4SIGNALPLOT_H
