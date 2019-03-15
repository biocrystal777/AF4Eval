#include "af4calibplotwidget.h"

AF4CalibPlotWidget::AF4CalibPlotWidget(QWidget *parent) : QWidget(parent)
{
   const QRect rec = QApplication::desktop()->availableGeometry();
   const uint screenWidth  = rec.width();
   const uint screenHeight = rec.height();
   //qDebug() << 12;
   lay = new QVBoxLayout(this);
   plot1 = new AF4SignalPlot(QString("Calibration signal (t_void region)"), this);
   //qDebug() << 15;
   plot1->setMinimumWidth(screenWidth/10*4);
   plot1->setMaximumWidth(screenWidth/10*4);
   plot1->setMinimumHeight(screenHeight/20*5);
   plot1->setMaximumHeight(screenHeight/10*5);

   //frameLayout->addWidget(plot1,0,15,8,4);
   lay->addWidget(plot1);

   plot2 = new AF4SignalPlot(QString("Calibration signal (t_e region)"), this);
   plot2->setMinimumWidth(screenWidth/10*4);
   plot2->setMaximumWidth(screenWidth/10*4);
   plot2->setMinimumHeight(screenHeight/20*5);
   plot2->setMaximumHeight(screenHeight/10*5);
   lay->addWidget(plot2);
   //frameLayout->addWidget(plot2,8,15,8,4);
   //QString calibFileName = getInputFilePath(false);

   //int error = setPlotDataFromFile();
   int error = 0;
   if(!error){
      plot1->initPlot();
      plot1->addPlotVLine("offset",      QColor(0x00,0x00,0x00));
      plot1->addPlotVLine("voidPeak",    QColor(0x00,0x66,0xFF));
      plot2->initPlot();
      plot2->addPlotVLine("elutionPeak", QColor(0xFF, 0x55, 0x00));
   }
}

void AF4CalibPlotWidget::connectMarkers(QWeakPointer<QDoubleSpinBox> leftOffset,
                                        QWeakPointer<QDoubleSpinBox> tVoid,
                                        QWeakPointer<QDoubleSpinBox> tElution)
{
   plot1->connectToPlotItem(leftOffset.data(), "offset");
   plot1->connectToPlotItem(tVoid.data(),      "voidPeak");
   plot2->connectToPlotItem(tElution.data(),   "elutionPeak");
   this->leftOffset = leftOffset;
   this->tVoid = tVoid;
   this->tElution = tElution;
}

void AF4CalibPlotWidget::disconnectCurrentMarkers()
{
   if(!leftOffset.isNull())
      plot1->disconnectFromPlotItem(leftOffset.data(), "offset");
   if(!tVoid.isNull())
      plot1->disconnectFromPlotItem(tVoid.data(), "voidPeak");
   if(!tElution.isNull())
      plot2->disconnectFromPlotItem(tElution.data(), "elutionPeak");
}

int AF4CalibPlotWidget::setPlotData(const QString &fileName)
{
   if(!QFile::exists(fileName)){
      AF4Log::logWarning(QString("Calibration file does not exist!"));
      return 1;
   }
   AF4CsvParser parser(fileName.toStdString(), ',', '.');
   int errorCode;
   uint errorInLine;
   errorCode = parser.parseFile(&errorInLine);
   if(errorCode){
      AF4Log::logError(tr("Error %1").arg(errorCode), true);
         AF4Log::logError(tr("Diffusion Evaluation Aborted while reading the input file."), true);
         return 2;
   } else AF4Log::logText(tr("File %1 read").arg(fileName));
   parser.getData();
   std::vector<std::string> headLines = parser.getHeadLines();
   matD data = parser.getData();

   QVector<double> plotX = QVector<double>::fromStdVector( data[0] );
   QVecMatrix<double> plotY;
   plotY.resize(data.size() -1);
   for(int i = 0; i < plotY.size(); ++i){
      plotY[i] = QVector<double>::fromStdVector( data[i+1] );
      // adjust plot Signals to non-negative values
      // as soon as AF4SciNotBox is not entirely functional for negative vals
      double minY = *(std::min_element(plotY[i].begin(), plotY[i].end()));
      double maxY = *(std::max_element(plotY[i].begin(), plotY[i].end()));
      for(double& v : plotY[i]){
         v -= minY;
         v += 0.01 * fabs(maxY-minY);
      }
   }

   QStringList signalSwitchEntries;
   for(std::string &entry : headLines)
      signalSwitchEntries.append(QString::fromStdString(entry));
   signalSwitchEntries.removeFirst(); // remove entry for time column

   plot1->setPlotXData( plotX );
   plot1->setPlotYData(plotY);
   plot1->setXScale(5,10);
   plot1->autoScaleY1Axis();
   plot1->autoScaleY2Axis();
   plot1->setSignal1Channels(signalSwitchEntries);
   plot1->setSignal2Channels(signalSwitchEntries);

   plot2->setPlotXData( plotX );
   plot2->setPlotYData(plotY);
   plot2->setXScale(8,60);
   plot2->autoScaleY1Axis();
   plot2->autoScaleY2Axis();
   plot2->setSignal1Channels(signalSwitchEntries);
   plot2->setSignal2Channels(signalSwitchEntries);

   return 0;
}
