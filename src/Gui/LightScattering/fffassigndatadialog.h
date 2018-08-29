#ifndef FFFASSIGNDATADIALOG_H
#define FFFASSIGNDATADIALOG_H

#include <QRect>
#include <QDesktopWidget>
#include <QApplication>
#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QList>
#include <QLabel>
#include <vector>
#include <limits>
#include "../GeneralWidgets/ffflog.h"
#include "./posPeakPair.h"
#include "../smallQDerivates/fffnumberedcombobox.h"
#include "../../Core/fffconstants.h"

class FFFAssignDataDialog : public QDialog
{
   Q_OBJECT
public:
   explicit FFFAssignDataDialog(std::vector<std::string> *headLines,
                                int numberOfPeaks,
                                uint *timeIndex,
                                QList<posPeakPair>* riIndices,
                                QList<QList<posPeakPair> >* mallsIndices,
                                uint *diffCoeffIndex = nullptr,
                                bool *dCIndexChosen = nullptr,
                                uint *rSIndex = nullptr,
                                bool *rSIndexChosen = nullptr,
                                uint *sedCoeffIndex = nullptr,
                                QWidget *parent = nullptr
                                );
   ~FFFAssignDataDialog();

    FFFAssignDataDialog(const FFFAssignDataDialog& src) = delete;
    FFFAssignDataDialog& operator= (const FFFAssignDataDialog& src) = delete;
    FFFAssignDataDialog(FFFAssignDataDialog&& src) = delete;
    FFFAssignDataDialog& operator= (FFFAssignDataDialog&& src) = delete;

protected:
   QGridLayout *layout;
   QScrollArea *scrollArea;
   QWidget *scrollWidget;
   std::vector<std::string> *headLines;
   int numberOfPeaks;
   uint *timeIndex;
   QList<posPeakPair>* riIndices;
   QList<QList<posPeakPair> >* mallsIndices;
   uint *diffCoeffIndex;
   bool *dCIndexChosen;
   uint *rSIndex;
   bool *rSIndexChosen;
   uint *sedCoeffIndex;

   // Items
   QGridLayout *scrolledLayout;
   QList<FFFNumberedComboBox*> *peakBoxes = nullptr;
   QList<FFFNumberedComboBox*> *dimensionBoxes = nullptr;
   QList<QLabel*> *peakChecker = nullptr;
   QList<QLabel*> *dimensionChecker = nullptr;

   QPushButton* confirmer;
   QPushButton* aborter;

protected slots:

   void checkDimConfiguration(int id, int index); // Rearrange box labels after setting the peak
   void checkPeakConfiguration(int id, int index); // Rearrange box labels after setting

public slots:
   void writeUserOptionAndAccept();
};


#endif // FFFASSIGNDATADIALOG_H
