#ifndef FFFPEAKPARAMETERFRAME_H
#define FFFPEAKPARAMETERFRAME_H

#include <QSettings>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include "../smallQDerivates/ffftwoboxwidget.h"
#include "../smallQDerivates/fffnumberedtoolbutton.h"
#include "../../Core/fffparameterstructs.h"

class FFFPeakParameterFrame final : public QFrame
{
   Q_OBJECT
public:
   explicit FFFPeakParameterFrame(const QString& prefix, int id, QWidget *parent = nullptr);
   ~FFFPeakParameterFrame();

   FFFPeakParameterFrame(const FFFPeakParameterFrame& src) = delete;
   FFFPeakParameterFrame& operator= (FFFPeakParameterFrame& src) = delete;
   FFFPeakParameterFrame(FFFPeakParameterFrame&& src) = delete;
   FFFPeakParameterFrame& operator= (FFFPeakParameterFrame&& src) = delete;

   inline void setId(int newId){ id = newId; }

   inline int getNumberOfPeaks() const { return peakParWidgets->size(); }



   SampleProperties getSampleProperties();



   inline QList<double> getStartTimes() const
   {
      QList<double> sT;
      for(QDoubleSpinBox *box : *startTimes) sT.append(box->value());
      return sT;
   }

   inline QList<double> getEndTimes() const
   {
      QList<double> sT;
      for(QDoubleSpinBox *box : *endTimes) sT.append(box->value());
      return sT;
   }

   inline QList<bool> getSecondVirUse() const
   {
      QList<bool> sT;
      for(QCheckBox *box : *useSecondVirCoeffs) sT.append(box->isChecked());
      return sT;
   }

   inline QList<double> getSecondVirCoeffs() const
   {
      QList<double> sT;
      for(FFFTwoBoxWidget *box : *secondVirCoeffs) sT.append(box->value());
      return sT;
   }

   inline QList<bool> getThirdVirUse() const
   {
      QList<bool> sT;
      for(QCheckBox *box : *useThirdVirCoeffs) sT.append(box->isChecked());
      return sT;
   }

   inline QList<double> getThirdVirCoeffs() const
   {
      QList<double> sT;
      for(FFFTwoBoxWidget *box : *thirdVirCoeffs) sT.append(box->value());
      return sT;
   }

   inline QList<double> getRefIndexIncs() const
   {
      QList<double> sT;
      for(QDoubleSpinBox *box : *refIndexIncrements) sT.append(box->value());
      return sT;
   }

   inline QList<double> getWaveLengths() const
   {
      QList<double> sT;
      for(QDoubleSpinBox *box : *waveLengths) sT.append(box->value());
      return sT;
   }

   inline QList<double> getEpsilon() const
   {
      QList<double> sT;
      for(QDoubleSpinBox *box : *waveLengths) sT.append(box->value());
      return sT;
   }

   inline QList<QString> getFractionDescriptions() const
   {
      QList<QString> sT;
      for(QLineEdit *line : *fractionDescription) sT.append(line->text());
      return sT;
   }

   void writeSettings();

private:
   QString prefix;
   int id;

   QGridLayout *lay;
   QScrollArea* scrollArea;
   QWidget *areaWidget;
   QHBoxLayout *peakParLayout;
   QList<QWidget*>* peakParWidgets;
   QList<QGridLayout*>* peakParLayouts;
   QList<QLabel*>* peakParLabels;
   QList<QDoubleSpinBox*> *startTimes;
   QList<QDoubleSpinBox*> *endTimes;
   QList<QCheckBox*> *useSecondVirCoeffs;
   QList<FFFTwoBoxWidget*> *secondVirCoeffs;
   QList<QCheckBox*> *useThirdVirCoeffs;
   QList<FFFTwoBoxWidget*> *thirdVirCoeffs;
   QList<QDoubleSpinBox*> *refIndexIncrements;
   QList<QDoubleSpinBox*> *waveLengths;
   QList<FFFTwoBoxWidget*> *epsilon;
   QList<QCheckBox*>* useKnownMW;
   QList<FFFTwoBoxWidget*>* knownMWs;
   QList<QLineEdit*> *fractionDescription;
   QList<FFFNumberedToolButton*>* removeButtons;
   QPushButton *peakAdder;

   void enableTimeFrameWidgets(bool enable);

public slots:
   void adoptConcentrationMode(uint mode); // 1: RI, 2: Absorbance

private slots:
   void addPeakParWidget();
   void removePeakParWidget(const int number);


};

#endif // FFFPEAKPARAMETERFRAME_H
