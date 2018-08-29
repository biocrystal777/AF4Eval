#ifndef FFFCALIBPLOTWIDGET_H
#define FFFCALIBPLOTWIDGET_H

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
#include "../GeneralWidgets/ffflog.h"
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../../Core/fffdatatypes.h"


class QwtDynPlotMarker : public QObject, public QwtPlotMarker
{
   Q_OBJECT

public:
   explicit QwtDynPlotMarker( const QString &title = QString::null ) : QwtPlotMarker( title ) {}
   explicit QwtDynPlotMarker( const QwtText &title )                 : QwtPlotMarker( title ) {}   

signals:
   void posChanged();

public slots:

   void setXValueSlot(double xValue);

};


/* Implement  later to speed up draw events

class VLineOverlay : public QwtWidgetOverlay {

  public:
   VLineOverlay( QWidget *QwtPlot ) : QwtWidgetOverlay(QwtPlot) {}


};
*/


class FFFCalibPlotWidget : public QWidget
{
   Q_OBJECT

public:
   explicit FFFCalibPlotWidget(const QString& title, QWidget *parent);

   int getPlotSignalChannel();
   void setPlotSignalChannels(const QStringList& strs);

   inline void setPlotXData(QVector<double> &newXData)      { plotXData = newXData; }
   inline void setPlotYData(QVecMatrix<double> &newYData)   { plotYData = newYData; }

   void setSignal1Channels(const QStringList& strs);
   void setSignal2Channels(const QStringList& strs);

   void initPlot();


   void addPlotVLine(QDoubleSpinBox* ctrlBox, const  QColor &color);


private:
   /*!
    * \brief destructor of this class
    */
   ~FFFCalibPlotWidget();

   FFFCalibPlotWidget(const FFFCalibPlotWidget& src)        = delete;
   FFFCalibPlotWidget& operator= (FFFCalibPlotWidget& src)  = delete;
   FFFCalibPlotWidget(FFFCalibPlotWidget&& src)             = delete;
   FFFCalibPlotWidget& operator= (FFFCalibPlotWidget&& src) = delete;

   void updatePlot();

   QGridLayout            *lay           = nullptr;
   QwtPlot                *plot          = nullptr;
   QwtPlotGrid            *grid          = nullptr;
   QwtPlotCurve           *signal1Curve  = nullptr;
   QwtPlotCurve           *signal2Curve  = nullptr;

   QDoubleSpinBox         *scaleXRangeMin  = nullptr;
   QDoubleSpinBox         *scaleXRangeMax  = nullptr;

   FFFTwoBoxWidget        *scaleY1RangeMin = nullptr;
   FFFTwoBoxWidget        *scaleY1RangeMax = nullptr;
   QPushButton            *autoScaleY1Button = nullptr;

   FFFTwoBoxWidget        *scaleY2RangeMin = nullptr;
   FFFTwoBoxWidget        *scaleY2RangeMax = nullptr;
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

   void autoScaleY1Axis();
   void autoScaleY2Axis();
   void setXScale(double minX, double maxX);


private slots:

   void adaptSignal1Switch(int sig1Ch);
   void adaptSignal2Switch(int sig2Ch);

   void redrawSignal1(int signalInt);
   void redrawSignal2(int signalInt);

   void adaptY1ScaleMinBoxLimit(double maxOfMin);
   void adaptY1ScaleMaxBoxLimit(double minOfMax);
   void reScaleY1Axis(double dummy);

   void adaptY2ScaleMinBoxLimit(double maxOfMin);
   void adaptY2ScaleMaxBoxLimit(double minOfMax);
   void reScaleY2Axis(double dummy);

   void adaptXScaleMinBoxLimit(double maxOfMin);
   void adaptXScaleMaxBoxLimit(double minOfMax);
   void reScaleXAxis(double dummy);


};



#endif // FFFCALIBPLOTWIDGET_H
