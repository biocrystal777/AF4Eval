#ifndef AF4CALIBPLOTWIDGET_H
#define AF4CALIBPLOTWIDGET_H

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
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../../Core/af4datatypes.h"

/*! *****************************************************************************************************************
***
***  \class     QwtDynPlotMarker "src/Gui/Calibration/af4calibplotwidget.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
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
***  \class     AF4Calibplotwidget "src/Gui/Calibration/af4calibplotwidget.h"
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018
***  \copyright CC CC BY-NC-ND 4.0
***
********************************************************************************************/


class AF4CalibPlotWidget final : public QWidget
{
   Q_OBJECT

public:

   /*!
    * \brief FFFCalibPlotWidget is default constructor
    * \param title
    * \param parent
    */
   AF4CalibPlotWidget(const QString& title, QWidget *parent);

   /*!
    *  \brief getPlotSignalChannel
    *
    */
   int getPlotSignalChannel();

   /*!
    * \brief setPlotSignalChannels
    * \param strs
    */
   void setPlotSignalChannels(const QStringList& strs);

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

   void setSignal1Channels(const QStringList& strs);

   /*!
    * \brief setSignal2Channels
    * \param strs
    */
   void setSignal2Channels(const QStringList& strs);

   /*!
    * \brief initPlot
    */
   void initPlot();

   /*!
    * \brief addPlotVLine
    * \param ctrlBox
    * \param color
    */
   void addPlotVLine(QDoubleSpinBox* ctrlBox, const  QColor &color);


protected:
   /*!
    * \brief destructor of this class
    */
   ~AF4CalibPlotWidget();

   AF4CalibPlotWidget(const AF4CalibPlotWidget& src)        = delete;
   AF4CalibPlotWidget& operator= (AF4CalibPlotWidget& src)  = delete;
   AF4CalibPlotWidget(AF4CalibPlotWidget&& src)             = delete;
   AF4CalibPlotWidget& operator= (AF4CalibPlotWidget&& src) = delete;

   /*!
    * \brief updatePlot updataes the data set and repaints plot
    */
   void updatePlot();

   QGridLayout            *lay           = nullptr;
   QwtPlot                *plot          = nullptr;
   QwtPlotGrid            *grid          = nullptr;
   QwtPlotCurve           *signal1Curve  = nullptr;
   QwtPlotCurve           *signal2Curve  = nullptr;

   QDoubleSpinBox         *scaleXRangeMin  = nullptr;
   QDoubleSpinBox         *scaleXRangeMax  = nullptr;

   AF4SciNotSpinBox        *scaleY1RangeMin = nullptr;
   AF4SciNotSpinBox        *scaleY1RangeMax = nullptr;
   QPushButton            *autoScaleY1Button = nullptr;

   AF4SciNotSpinBox        *scaleY2RangeMin = nullptr;
   AF4SciNotSpinBox        *scaleY2RangeMax = nullptr;
   QPushButton            *autoScaleY2Button = nullptr;


   QVector<double>         plotXData;
   QVecMatrix<double>      plotYData;

   QComboBox              *signal1Switch = nullptr;
   int signal1Ch = 0;
   int signal2Ch = 0;
   QComboBox              *signal2Switch  = nullptr;

   QList<QwtDynPlotMarker*> plotMarkers;
   QList<QwtSymbol*> symbols;

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


private slots:

   /*!
    * \brief adaptSignal2Switch
    * \param sig2Ch
    */
   void adaptSignal2Switch();

   /*!
    * \brief redrawSignal1
    * \param signalInt
    */
   void redrawSignal1(int signalInt);

   /*!
    * \brief redrawSignal2
    * \param signalInt
    */
   void redrawSignal2(int signalInt);


   // NEED CHANGES AT THE AF4TWOBOXWIDGET
   /*!
    * \brief adaptY1ScaleMinBoxLimit
    * \param maxOfMin
    */
   void adaptY1ScaleMinBoxLimit(double maxOfMin);

   /*!
    * \brief adaptY1ScaleMaxBoxLimit
    * \param minOfMax
    */
    void adaptY1ScaleMaxBoxLimit(double minOfMax);

   /*!
    * \brief reScaleY1Axis
    */
   void reScaleY1Axis();

   // NEED CHANGES AT THE AF4TWOBOXWIDGET
   /*!
    * \brief adaptY2ScaleMinBoxLimit
    * \param maxOfMin
    */
   void adaptY2ScaleMinBoxLimit(double maxOfMin);

   /*!
    * \brief adaptY2ScaleMaxBoxLimit
    * \param minOfMax
    */
   void adaptY2ScaleMaxBoxLimit(double minOfMax);

   /*!
    * \brief reScaleY2Axis

    */
   void reScaleY2Axis();

   /*!
    * \brief adaptXScaleMinBoxLimit
    * \param maxOfMin
    */
   void adaptXScaleMinBoxLimit(double maxOfMin);

   /*!
    * \brief adaptXScaleMaxBoxLimit
    * \param minOfMax
    */
   void adaptXScaleMaxBoxLimit(double minOfMax);

   /*!
    * \brief reScaleXAxis
    */
   void reScaleXAxis();
};
#endif // AF4CALIBPLOTWIDGET_H
