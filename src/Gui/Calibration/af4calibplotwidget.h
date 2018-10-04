#ifndef AF4CALIBPLOTWIDGET_H
#define AF4CALIBPLOTWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QDoubleSpinBox>
#include "./af4signalplot.h"
#include "../Core/af4csvparser.h"

class AF4CalibPlotWidget final : public QWidget
{
   Q_OBJECT

public:
   AF4CalibPlotWidget(QWidget *parent);

   void connectMarkers(QWeakPointer<QDoubleSpinBox> leftOffset,
                       QWeakPointer<QDoubleSpinBox> tVoid,
                       QWeakPointer<QDoubleSpinBox> tElution);
   //void disconnectMarkers(QDoubleSpinBox *leftOffset, QDoubleSpinBox *tVoid, QDoubleSpinBox *tElution);
   void disconnectCurrentMarkers();
   int setPlotData(const QString &fileName);

private:
   QVBoxLayout   *lay   = nullptr;
   AF4SignalPlot *plot1 = nullptr;
   AF4SignalPlot *plot2 = nullptr;

   QWeakPointer<QDoubleSpinBox> leftOffset;
   QWeakPointer<QDoubleSpinBox> tVoid;
   QWeakPointer<QDoubleSpinBox> tElution;
};

#endif // AF4CALIBPLOTWIDGET_H
