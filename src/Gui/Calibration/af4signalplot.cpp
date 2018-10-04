#include "./af4signalplot.h"

//-/////////////////////////////////////
//
//  Public stuff
//
//-/////////////////////////////////////

AF4SignalPlot::AF4SignalPlot(const QString& title, QWidget *parent) : QWidget(parent)
{
   //qDebug() << 10;

   lay = new QGridLayout(this);

   const QRect rec = QApplication::desktop()->availableGeometry();
   const uint screenWidth  = rec.width();
   const uint screenHeight = rec.height();
   this->setMinimumSize(screenWidth/10*4, screenHeight/10*4);

   signal1Switch = new QComboBox(this);
   //signal1Switch->blockSignals(true);

   signal1Switch->setToolTip("Load calibration data set to choose a signal.");
   signal1Switch->setMinimumContentsLength(30);
   signal1Switch->setFixedWidth(this->width() * 0.25);
   connect(signal1Switch, qOverload<int>(&QComboBox::currentIndexChanged),
           [this](int i) {
      this->signal1Ch = i;
      signal1Switch->setToolTip(signal1Switch->currentText());
      signal1Curve->setSamples(plotXData, plotYData[signal1Ch]);
      autoScaleY1Axis();
   } );
   lay->addWidget(signal1Switch, 1, 2, 1, 1);
   lay->addWidget(new QLabel("Signals 1", this), 1,1,1,1);

   signal2Switch = new QComboBox(this);
   //signal2Switch->blockSignals(true);
   signal2Switch->setToolTip("Load calibration data set to choose a signal.");
   signal2Switch->setMinimumContentsLength(30);
   signal2Switch->setFixedWidth(this->width() * 0.25);
   connect(signal2Switch, qOverload<int>(&QComboBox::currentIndexChanged),
           this,[this](int i) {
      signal2Ch = i;
      signal2Switch->setToolTip(signal2Switch->currentText());
      signal2Curve->setSamples(plotXData, plotYData[signal2Ch]);
      autoScaleY2Axis();
   } );

   lay->addWidget(signal2Switch, 1, 5, 1, 1);
   lay->addWidget(new QLabel("Signals 2", this), 1,4,1,1);

   plot = new QwtPlot(this);
   lay->addWidget(plot, 2, 1, 5, 5);
   plot->setCanvasBackground(Qt::white);
   plot->setAxisTitle(QwtPlot::xBottom, tr( "time / min" ) );
   plot->setAxisTitle(QwtPlot::yLeft,  tr( "Δ(Signal 1)" ) );
   plot->enableAxis(QwtPlot::yRight);
   plot->setAxisTitle(QwtPlot::yRight, tr( "Δ(Signal 2)" ) );
   plot->setTitle(title);
   grid = new QwtPlotGrid;
   grid->enableXMin(true);
   grid->enableYMin(true);
   grid->setMajorPen( QPen( Qt::lightGray, 0, Qt::SolidLine ));
   grid->setMinorPen( QPen( Qt::white, 0, Qt::DotLine ) );
   grid->attach(plot);

   scaleXRangeMin = new QDoubleSpinBox(this);
   scaleXRangeMin->setDecimals(1);
   scaleXRangeMin->setMinimum(0.0);
   scaleXRangeMin->setMaximum(300.0);
   lay->addWidget(scaleXRangeMin, 7, 1);

   scaleXRangeMax = new QDoubleSpinBox(this);

   scaleXRangeMax->setDecimals(1);
   scaleXRangeMax->setMinimum(1.0);
   scaleXRangeMax->setMaximum(600.0);
   lay->addWidget(scaleXRangeMax, 7, 5 );

   auto reScaleXAxis = [this] () {
      plot->setAxisScale(QwtPlot::xBottom, this->scaleXRangeMin->value(), this->scaleXRangeMax->value());
      plot->replot();
   };

   connect(scaleXRangeMin, qOverload<double>(&QDoubleSpinBox::valueChanged), this, reScaleXAxis);
   connect(scaleXRangeMax, qOverload<double>(&QDoubleSpinBox::valueChanged), this, reScaleXAxis);
   connect(scaleXRangeMin, qOverload<double>(&QDoubleSpinBox::valueChanged),
                    this, [this] (double minOfMax){ scaleXRangeMax->setMinimum(minOfMax + 1.0); } );
   connect(scaleXRangeMax, qOverload<double>(&QDoubleSpinBox::valueChanged),
                    this, [this] (double maxOfMin){ scaleXRangeMin->setMaximum(maxOfMin - 1.0); } );

   scaleXRangeMax->setValue(10.0);

   scaleY1RangeMin = new AF4SciNotSpinBox(false, this);
   scaleY1RangeMin->setObjectName(tr("Y1Min"));
   scaleY1RangeMin->setToolTip("Lower scale limit of left Y-Axis");
   scaleY1RangeMin->setSignifandDecimals(1);
   scaleY1RangeMin->setSignifandSingleStep(0.1);
   lay->addWidget(scaleY1RangeMin, 6, 0, Qt::AlignRight);
   scaleY1RangeMax = new AF4SciNotSpinBox(false, this);
   scaleY1RangeMax->setObjectName(tr("Y1Max"));
   scaleY1RangeMax->setToolTip("Upper scale limit of left Y-Axis");
   scaleY1RangeMax->setSignifandDecimals(1);
   scaleY1RangeMax->setSignifandSingleStep(0.1);   

   lay->addWidget(scaleY1RangeMax, 2, 0, Qt::AlignRight);

   auto reScaleY1Axis = [this] () {
      plot->setAxisScale(QwtPlot::yLeft, scaleY1RangeMin->value(), scaleY1RangeMax->value());
      plot->replot();
   };
   connect(scaleY1RangeMin, qOverload<>(&AF4SciNotSpinBox::valueChanged), this, reScaleY1Axis);
   connect(scaleY1RangeMax, qOverload<>(&AF4SciNotSpinBox::valueChanged), this, reScaleY1Axis);
   connect(scaleY1RangeMin, qOverload<double>(&AF4SciNotSpinBox::valueChanged),
                    this, [this] (double m){ scaleY1RangeMax->setMinimum(m); } );
   connect(scaleY1RangeMax, qOverload<double>(&AF4SciNotSpinBox::valueChanged),
                    this, [this] (double m){ scaleY1RangeMin->setMaximum(m); } );

   autoScaleY1Button = new QPushButton("Rescale Y1");
   connect(autoScaleY1Button, &QPushButton::pressed, this, &AF4SignalPlot::autoScaleY1Axis);
   lay->addWidget(autoScaleY1Button, 3, 0, Qt::AlignRight);

   scaleY1RangeMin->setValue(1);
   scaleY1RangeMax->setValue(100);

   scaleY2RangeMin = new AF4SciNotSpinBox( false, this);
   scaleY2RangeMin->setToolTip("Lower scale limit of right Y-Axis");
   scaleY2RangeMin->setSignifandDecimals(1);
   scaleY2RangeMin->setSignifandSingleStep(0.1);
   lay->addWidget(scaleY2RangeMin, 6, 6, Qt::AlignLeft);   
   scaleY2RangeMax = new AF4SciNotSpinBox(false, this);
   scaleY2RangeMax->setToolTip("Upper scale limit of right Y-Axis");
   scaleY2RangeMax->setSignifandDecimals(1);
   scaleY2RangeMax->setSignifandSingleStep(0.1);

   lay->addWidget(scaleY2RangeMax, 2, 6, Qt::AlignLeft);

   auto reScaleY2Axis = [this] () {
      plot->setAxisScale(QwtPlot::yRight, scaleY2RangeMin->value(), scaleY2RangeMax->value());
      plot->replot();
   };
   connect(scaleY2RangeMin, qOverload<>(&AF4SciNotSpinBox::valueChanged), this, reScaleY2Axis);
   connect(scaleY2RangeMax, qOverload<>(&AF4SciNotSpinBox::valueChanged), this, reScaleY2Axis);
   connect(scaleY2RangeMin, qOverload<double>(&AF4SciNotSpinBox::valueChanged),
                    this, [this] (double minOfMax){ scaleY2RangeMax->setMinimum(minOfMax); } );
   connect(scaleY2RangeMax, qOverload<double>(&AF4SciNotSpinBox::valueChanged),
                    this, [this] (double maxOfMin){ scaleY2RangeMin->setMaximum(maxOfMin); } );

   autoScaleY2Button = new QPushButton("Rescale Y2");
   connect(autoScaleY2Button, &QPushButton::pressed, this, &AF4SignalPlot::autoScaleY2Axis);
   lay->addWidget(autoScaleY2Button, 3, 6, Qt::AlignLeft);
   //qDebug() << 100;
   defaultInitData();
  // qDebug() << 150;
   initPlot();
//   qDebug() << 200;
}

void AF4SignalPlot::initPlot()
{   
   if(!signal1Curve){
      signal1Curve = new QwtPlotCurve(QString("Measured"));
      signal1Curve->setStyle(QwtPlotCurve::Lines);
      signal1Curve->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
      signal1Curve->setPen( QPen( QBrush(QColor(0x00, 0x00, 0xc0)), 2.0, Qt::SolidLine));
      signal1Curve->attach(plot);
      signal1Curve->show();

   }
   signal1Switch->setEnabled(true);
   if(!signal2Curve){
      signal2Curve = new QwtPlotCurve(QString("Measured"));
      signal2Curve->setStyle(QwtPlotCurve::Lines);
      signal2Curve->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
      signal2Curve->setPen( QPen( QBrush(QColor(0x00, 0x90, 0xc0)), 2.0, Qt::SolidLine));
      signal2Curve->attach(plot);
      signal2Curve->show();
   }
   signal2Switch->setEnabled(true);
   this->updatePlot();
}

void AF4SignalPlot::setSignal1Channels(const QStringList &strs)
{
   signal1Switch->blockSignals(true);
   signal1Switch->clear();
   QFontMetrics fm = signal1Switch->fontMetrics();
   int width = signal1Switch->width() * 0.95;
   for(const QString &entry : strs){
      QString shortStr = fm.elidedText( entry , Qt::ElideMiddle, width);
      signal1Switch->addItem(shortStr, QVariant(entry));
   }
   signal1Switch->setToolTip(signal1Switch->currentData().toString());
   signal1Switch->blockSignals(false);
}

void AF4SignalPlot::setSignal2Channels(const QStringList &strs)
{
   signal2Switch->blockSignals(true);
   signal2Switch->clear();
   QFontMetrics fm = signal2Switch->fontMetrics();
   int width = signal2Switch->width() * 0.95;
   for(const QString &entry : strs){
      QString shortStr = fm.elidedText( entry , Qt::ElideMiddle, width);
      signal2Switch->addItem(shortStr, QVariant(entry));
   }
   signal2Switch->setToolTip(signal2Switch->currentData().toString());
   signal2Switch->blockSignals(false);
}

void AF4SignalPlot::addPlotVLine(const QString &markerName, const QColor &color)
{
   //QString name("name");
   QwtDynPlotMarker *marker = plotMarkers[markerName] = new QwtDynPlotMarker();
   QwtSymbol        *symbol = symbols[markerName]     = new QwtSymbol(QwtSymbol::VLine);
   //QwtDynPlotMarker *marker = plotMarkers[markerName];
   //QwtSymbol        *symbol = symbols[markerName];
   marker->setLinePen(color, 4.0);
   symbol->setPen(color, 2.0);
   //symbol->setSize(1);
   symbol->setSize(this->maximumHeight());
   //symbol->setSize(10000);
   marker->setSymbol(symbol);
   //marker->setXValue(ctrlBox->value());
   marker->attach(plot);   
   connect(marker, &QwtDynPlotMarker::posChanged, plot, &QwtPlot::replot);
   plot->replot();
}

void AF4SignalPlot::connectToPlotItem(QDoubleSpinBox *const ctrlBox, const QString &markerName)
{
   QwtDynPlotMarker *marker = plotMarkers[markerName];
   marker->setXValue(ctrlBox->value());
   connect(ctrlBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
           marker, &QwtDynPlotMarker::setXValueEmit);
   //this->updatePlot();
   plot->replot();
}

void AF4SignalPlot::disconnectFromPlotItem(QDoubleSpinBox *const ctrlBox, const QString &markerName)
{
   disconnect(ctrlBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
              plotMarkers[markerName], &QwtDynPlotMarker::setXValueEmit);
   //connect(ctrlBox, &QDoubleSpinBox::valueChanged, marker, &QwtDynPlotMarker::setXValueEmit);
   //disconnect(ctrlBox, SIGNAL(valueChanged(double)), plotMarkers[markerName], SLOT(setXValueEmit(double)));
}


//-/////////////////////////////////////
//
//  Public slots
//
//-/////////////////////////////////////

void AF4SignalPlot::autoScaleY1Axis()
{
   // find range
   double minX = scaleXRangeMin->value();
   double maxX = scaleXRangeMax->value();

   int minXInd = std::distance(plotXData.begin(), std::lower_bound(plotXData.begin(), plotXData.end(), minX));
   int maxXInd = std::distance(plotXData.begin(), std::upper_bound(plotXData.begin(), plotXData.end(), maxX));
   double minY = *(std::min_element(plotYData[signal1Ch].begin() + minXInd, plotYData[signal1Ch].begin() + maxXInd));
   double maxY = *(std::max_element(plotYData[signal1Ch].begin() + minXInd, plotYData[signal1Ch].begin() + maxXInd));

   maxY += 0.1 * fabs(maxY-minY);
   scaleY1RangeMin->blockSignals(true);
   scaleY1RangeMax->blockSignals(true);
   bool ok;
   scaleY1RangeMin->setValue(minY, &ok);
   if(!ok) AF4Log::logWarning(tr("Bug scaleY1RangeMin->setValue(minY, &ok);"));
   scaleY1RangeMax->setValue(maxY);
   if(!ok) AF4Log::logWarning(tr("Bug scaleY1RangeMax->setValue(minY, &ok);"));
   scaleY1RangeMin->setMaximum(maxY);
   if(!ok) AF4Log::logWarning(tr("Bug scaleY1RangeMin->setMaximum(maxY);"));
   scaleY1RangeMax->setMinimum(minY);
   if(!ok) AF4Log::logWarning(tr("Bug scaleY1RangeMax->setMinimum(minY);"));
   plot->setAxisScale(QwtPlot::yLeft, minY, maxY);
   plot->replot();
   scaleY1RangeMin->blockSignals(false);
   scaleY1RangeMax->blockSignals(false);
}

void AF4SignalPlot::autoScaleY2Axis()
{
   // find range
   double minX = scaleXRangeMin->value();
   double maxX = scaleXRangeMax->value();
   int minXInd = std::distance(plotXData.begin(), std::lower_bound(plotXData.begin(), plotXData.end(), minX));
   int maxXInd = std::distance(plotXData.begin(), std::upper_bound(plotXData.begin(), plotXData.end(), maxX));
   double minY = *(std::min_element(plotYData[signal2Ch].begin() + minXInd, plotYData[signal2Ch].begin() + maxXInd));
   double maxY = *(std::max_element(plotYData[signal2Ch].begin() + minXInd, plotYData[signal2Ch].begin() + maxXInd));

   maxY += 0.1 * fabs(maxY-minY);
   this->scaleY2RangeMin->blockSignals(true);
   this->scaleY2RangeMax->blockSignals(true);
   this->scaleY2RangeMin->setValue(minY);
   this->scaleY2RangeMax->setValue(maxY);
   this->plot->setAxisScale(QwtPlot::yRight, minY, maxY);
   this->plot->replot();
   scaleY2RangeMin->blockSignals(false);
   scaleY2RangeMax->blockSignals(false);
}

void AF4SignalPlot::setXScale(double minX, double maxX)
{
   this->scaleXRangeMin->blockSignals(true);
   this->scaleXRangeMax->blockSignals(true);
   this->scaleXRangeMin->setValue(minX);
   this->scaleXRangeMax->setValue(maxX);
   this->plot->setAxisScale(QwtPlot::xBottom, minX, maxX);
   plot->replot();
   this->scaleXRangeMin->blockSignals(false);
   this->scaleXRangeMax->blockSignals(false);
}

//-/////////////////////////////////////
//
//  Private stuff
//
//-/////////////////////////////////////

AF4SignalPlot::~AF4SignalPlot()
{
   if(grid) {delete grid; grid = nullptr;}
}

void AF4SignalPlot::updatePlot()
{
   signal1Curve->setSamples(plotXData, plotYData[signal1Ch]);
   signal2Curve->setSamples(plotXData, plotYData[signal2Ch]);
   plot->repaint();
}

void AF4SignalPlot::defaultInitData()
{
   int dSize = 100;
   plotXData.resize(dSize);
   for(int i = 0; i < dSize; ++i) plotXData[i] = double(i);
   plotYData.resize(4);
   for(auto &sig : plotYData) sig.resize(dSize);
   for(int i = 0; i < dSize; ++i){
      plotYData[0][i] = double(i) * 0.5;
      plotYData[1][i] = double(i%20);
      plotYData[2][i] = sin(double(i)*0.2) * 15.0;
      plotYData[3][i] = cos(double(i)*0.2) * 20.0;
   }
   signal1Switch->blockSignals(true);
   signal1Switch->insertItem(0, "line");
   signal1Switch->insertItem(1, "rep");
   signal1Switch->insertItem(2, "sin");
   signal1Switch->insertItem(3, "cos");
   signal1Switch->blockSignals(false);
   signal2Switch->blockSignals(true);
   signal2Switch->insertItem(0, "line");
   signal2Switch->insertItem(1, "rep");
   signal2Switch->insertItem(2, "sin");
   signal2Switch->insertItem(3, "cos");
   signal2Switch->blockSignals(false);

}
