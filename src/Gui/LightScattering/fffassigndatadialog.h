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
#include "../GeneralWidgets/af4log.h"
#include "./posPeakPair.h"
#include "../smallQDerivates/fffnumberedcombobox.h"
#include "../../Core/af4constants.h"

/*! **********************************************************************************************
***
***  \class   AF4Log "src/Gui/Calibration/af4log.h"
***  \brief   Logging widget
***  \details The class is implemented as singleton and public static functions can be
***           accessed. All functions can be called globally.
***
***  \author    Benedikt Häusele
***  \version   1.0
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/


class AF4AssignDataDialog : public QDialog
{
   Q_OBJECT
public:
   explicit AF4AssignDataDialog(std::vector<std::string> *headLines,
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
   ~AF4AssignDataDialog();

    AF4AssignDataDialog(const AF4AssignDataDialog& src) = delete;
    AF4AssignDataDialog& operator= (const AF4AssignDataDialog& src) = delete;
    AF4AssignDataDialog(AF4AssignDataDialog&& src) = delete;
    AF4AssignDataDialog& operator= (AF4AssignDataDialog&& src) = delete;

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
