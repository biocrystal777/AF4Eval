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
#include "../smallQDerivates/af4numberedcombobox.h"
#include "../../Core/af4constants.h"
#include "../../Core/af4parameterstructs.h"

/*! **********************************************************************************************
***
***  \class   AF4AssignDataDialog "src/Gui/Calibration/af4assigndatadialog.h"
***  \brief   Widget to ensure that signals for MALLS, UV and/or RI correspond to the detected signals
***  \details   Not used at the moment
***  \author    Benedikt Häusele
***  \version   0.1 currently not used
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***  \todo     complete refactoring when used
***
*************************************************************************************************/


class AF4AssignDataDialog final : public QDialog
{
   Q_OBJECT
public:
   /*!
    * \brief AF4AssignDataDialog
    * \param headLines
    * \param numberOfPeaks
    * \param timeIndex
    * \param riIndices
    * \param mallsIndices
    * \param diffCoeffIndex
    * \param dCIndexChosen
    * \param rSIndex
    * \param rSIndexChosen
    * \param sedCoeffIndex
    * \param parent
    */
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

private:
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
   QList<AF4NumberedComboBox*> *peakBoxes = nullptr;
   QList<AF4NumberedComboBox*> *dimensionBoxes = nullptr;


   QList<QLabel*> peakChecker;
   QList<QLabel*> dimensionChecker;

   QPushButton* confirmer;
   QPushButton* aborter;

private slots:

   /*!
    * \brief checkDimConfiguration
    * \param id
    * \param index
    */
   void checkDimConfiguration();//(int index, int id); // Rearrange box labels after setting the peak

   /*!
    * \brief checkPeakConfiguration
    * \param id
    * \param index
    */
   void checkPeakConfiguration();//(int index, int id); // Rearrange box labels after setting

public slots:

   /*!
    * \brief writeUserOptionAndAccept
    */
   void writeUserOptionAndAccept();
};


#endif // FFFASSIGNDATADIALOG_H
