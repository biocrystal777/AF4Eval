#include "fffassigndatadialog.h"

using std::vector;
using std::string;

FFFAssignDataDialog::FFFAssignDataDialog(vector<string> *headLines,
                                         int numberOfPeaks,
                                         uint *timeIndex,
                                         QList<posPeakPair> *riIndices,
                                         QList<QList<posPeakPair> > *mallsIndices,
                                         uint *diffCoeffIndex,
                                         bool *dCIndexChosen,
                                         uint *rSIndex,
                                         bool *rSIndexChosen,
                                         uint *sedCoeffIndex,
                                         QWidget *parent
                                         )
   : QDialog(parent),
     headLines(headLines),
     numberOfPeaks(numberOfPeaks),
     timeIndex(timeIndex),
     riIndices(riIndices),
     mallsIndices(mallsIndices),
     diffCoeffIndex(diffCoeffIndex),
     dCIndexChosen(dCIndexChosen),
     rSIndex(rSIndex),
     rSIndexChosen(rSIndexChosen),
     sedCoeffIndex(sedCoeffIndex)
{
   layout = new QGridLayout(this);
   layout->addWidget(new QLabel(tr("Assign to their detector:"), this), 0, 0, 1, 8, Qt::AlignCenter);
   scrollArea = new QScrollArea(this);

   QRect screenGeometry = QApplication::desktop()->availableGeometry();
   qDebug() << "x:" << screenGeometry.width() << "y:" << screenGeometry.height();
   int xScreen = (screenGeometry.width()) * 4 / 5;
   int yScreen = (screenGeometry.height()) * 4 / 5;
   this->setMaximumSize(xScreen, yScreen);
   this->setMinimumSize(xScreen, yScreen);
   xScreen = (xScreen * 7) / 8;
   yScreen = (yScreen * 5) / 6;
   scrollArea->setMaximumSize(xScreen, yScreen);
   scrollArea->setMinimumSize(xScreen, yScreen);
   scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

   layout->addWidget(scrollArea, 1, 0, 10, 8, Qt::AlignCenter);
   scrollWidget = new QWidget(scrollArea);

   scrollArea->setWidget(scrollWidget);

   scrolledLayout = new QGridLayout(scrollWidget);

   peakBoxes = new QList<FFFNumberedComboBox*>();
   dimensionBoxes = new QList<FFFNumberedComboBox*>();
   dimensionChecker = new QList<QLabel*>();
   peakChecker = new QList<QLabel*>();

   uint numberOfColumns = headLines->size();
   for(uint i = 0; i < numberOfColumns; ++i){
      // headLine
      scrolledLayout->addWidget(new QLabel(tr("%1").arg(headLines->at(i).c_str()), scrollWidget), i, 0, 1, 4, Qt::AlignLeft);

      // dimensionBoxes
      FFFNumberedComboBox *newDimensionBox = new FFFNumberedComboBox(i, scrollWidget);
      // content of dimensionBoxes
      newDimensionBox->addItem("Times");                                                                        // index 0
      newDimensionBox->addItem("Diffusion Coefficents");                                                        // index 1
      newDimensionBox->addItem("Hydrodynamic Radii");                                                           // index 2
      newDimensionBox->addItem("RI Signals");                                                                   // index 3
      for(uint j = 0; j < NUMBER_OF_DETECTORS; ++j) newDimensionBox->addItem(tr("MALLS[%1] Signals").arg(j+1)); // index 4-11
      newDimensionBox->addItem("Sedimentation coefficient");                                                    // index 12
      dimensionBoxes->append(newDimensionBox);
      scrolledLayout->addWidget(newDimensionBox, i, 4, 1, 1, Qt::AlignCenter);
      // dimensionChecker
      dimensionChecker->append(new QLabel(" ", scrollWidget));
      scrolledLayout->addWidget(dimensionChecker->at(i), i, 6, 1, 1, Qt::AlignCenter);
      // peakBoxes
      FFFNumberedComboBox *newPeakBox = new FFFNumberedComboBox(i, scrollWidget);
      // content of peakBoxes
      for(int j = 0; j < numberOfPeaks; ++j) newPeakBox->addItem(tr("Peak %1").arg(j+1));
      peakBoxes->append(newPeakBox);
      qDebug() << i << 4;
      peakChecker->append(new QLabel(" ", scrollWidget));
      scrolledLayout->addWidget(peakChecker->at(i), i, 9, 1, 1, Qt::AlignCenter);
      scrolledLayout->addWidget(newPeakBox, i, 7, 1, 2, Qt::AlignCenter);
   }
   qDebug() << "AssignDialog mark 2";
   confirmer = new QPushButton("Continue ", this);
   QObject::connect(confirmer, SIGNAL(clicked()), this, SLOT(writeUserOptionAndAccept()));
   layout->addWidget(confirmer, 11, 6, 1, 1);
   aborter = new QPushButton("Abort", this);
   QObject::connect(aborter, SIGNAL(clicked()), this, SLOT(reject()));
   layout->addWidget(aborter, 11, 7, 1, 1);


   // initialize box entries

   //time
   if(dimensionBoxes->size() >= 1) dimensionBoxes->at(0)->setCurrentIndex(0);
   if(dimensionBoxes->size() >= 2) dimensionBoxes->at(1)->setCurrentIndex(1);
   if(dimensionBoxes->size() >= 3) dimensionBoxes->at(2)->setCurrentIndex(2);
   // malls
   for(int k=0; k < NUMBER_OF_DETECTORS; ++k){
      for(int i=0; i < numberOfPeaks; ++i){
         if(dimensionBoxes->size() > (3 + numberOfPeaks * k + i) ) dimensionBoxes->at(3 + numberOfPeaks * k + i)->setCurrentIndex(k + 4);
      }
   }
   // ri
   for(int i=0; i < numberOfPeaks; ++i){
      if(dimensionBoxes->size() > i + 3 + numberOfPeaks * NUMBER_OF_DETECTORS ) dimensionBoxes->at(3 + i + numberOfPeaks * NUMBER_OF_DETECTORS)->setCurrentIndex(3);
   }

   // peakBoxes


   for(uint i = 0 ; i < numberOfColumns; ++i){
      peakBoxes->at(i)->setCurrentIndex(i);
      peakBoxes->at(i)->show();
      peakChecker->at(i)->show();
   }

   checkDimConfiguration(0, 0);
   for(uint i = 0; i < numberOfColumns; ++i){
      QObject::connect(dimensionBoxes->at(i), SIGNAL(currentIndexChangedId(int, int)), this, SLOT(checkDimConfiguration(int, int)));
      QObject::connect(peakBoxes->at(i), SIGNAL(currentIndexChangedId(int, int)), this, SLOT(checkDimConfiguration(int,int)));
   }
   scrollWidget->show();

}

FFFAssignDataDialog::~FFFAssignDataDialog()
{
   if(peakBoxes) delete peakBoxes;
   if(dimensionBoxes) delete dimensionBoxes;
   if(peakChecker) delete peakChecker;
   if(dimensionChecker) delete dimensionChecker;
}

void FFFAssignDataDialog::checkDimConfiguration(int id, int index)
{
   bool activateConfirmerLater = true;
   QList<int> timeBoxIndices = QList<int>();
   QList<int> diffCoeffBoxIndices= QList<int>();
   QList<int> hydroDynRadBoxIndices = QList<int>();
   QList<int> riBoxIndices = QList<int>();
   QList<QList<int> > mallsBoxIndices = QList<QList<int> >();
   QList<int> sedCoeffIndices = QList<int>();
   for(int i = 0; i < NUMBER_OF_DETECTORS; ++i) mallsBoxIndices.append(QList<int>());

   // find distribution over all Boxes, hide or show peakBoxes;
   for(int i = 0; i < dimensionBoxes->size(); ++i){
      int boxValue = dimensionBoxes->at(i)->currentIndex();
      if(boxValue == 0){
         timeBoxIndices.append(i);
         peakBoxes->at(i)->hide();
         peakChecker->at(i)->hide();
      }
      else if(boxValue == 1){
         diffCoeffBoxIndices.append(i);
         peakBoxes->at(i)->hide();
         peakChecker->at(i)->hide();
      }
      else if(boxValue == 2){
         hydroDynRadBoxIndices.append(i);
         peakBoxes->at(i)->hide();
         peakChecker->at(i)->hide();
      }
      else if(boxValue == 3){
         riBoxIndices.append(i);
         peakBoxes->at(i)->show();
         peakChecker->at(i)->show();
      }
      else if(4 <= boxValue && boxValue <= 11){
         (mallsBoxIndices[boxValue-4]).append(i);
         peakBoxes->at(i)->show();
         peakChecker->at(i)->show();
      } else if ( boxValue == 12 ) {
         sedCoeffIndices.append(i);
         peakBoxes->at(i)->hide();
         peakChecker->at(i)->hide();
      }
   }

   // check for duplicates for data that should only occur once
   // times
   if(timeBoxIndices.size() > 1){
      for(int i = 0; i < timeBoxIndices.size(); ++i)
         dimensionChecker->at(timeBoxIndices.at(i))->setStyleSheet("QLabel { background-color : red; color : black; }");
      activateConfirmerLater = false;
   }
   else {
      for(int i = 0; i < timeBoxIndices.size(); ++i)
         dimensionChecker->at(timeBoxIndices.at(i))->setStyleSheet("QLabel { background-color : green; color : black; }");
   }
   for(int i = 0; i < timeBoxIndices.size(); ++i)
      dimensionChecker->at(timeBoxIndices.at(i))->setText(tr(" %1 / 1 ").arg(timeBoxIndices.size()));
   // diffusion Coefficients
   if(diffCoeffBoxIndices.size() > 1){
      for(int i = 0; i < diffCoeffBoxIndices.size(); ++i)
         dimensionChecker->at(diffCoeffBoxIndices.at(i))->setStyleSheet("QLabel { background-color : red; color : black; }");
      activateConfirmerLater = false;
   }
   else {
      for(int i = 0; i < diffCoeffBoxIndices.size(); ++i)
         dimensionChecker->at(diffCoeffBoxIndices.at(i))->setStyleSheet("QLabel { background-color : green; color : black; }");

   }
   for(int i = 0; i < diffCoeffBoxIndices.size(); ++i)
      dimensionChecker->at(diffCoeffBoxIndices.at(i))->setText(tr(" %1 / 1 ").arg(diffCoeffBoxIndices.size()));

   // hydrodynamic radii entries
   if(hydroDynRadBoxIndices.size() > 1){
      for(int i = 0; i < hydroDynRadBoxIndices.size(); ++i)
         dimensionChecker->at(hydroDynRadBoxIndices.at(i))->setStyleSheet("QLabel { background-color : red; color : black; }");
      activateConfirmerLater = false;
   }
   else {
      for(int i = 0; i < hydroDynRadBoxIndices.size(); ++i)
         dimensionChecker->at(hydroDynRadBoxIndices.at(i))->setStyleSheet("QLabel { background-color : green; color : black; }");
   }
   for(int i = 0; i < hydroDynRadBoxIndices.size(); ++i)
      dimensionChecker->at(hydroDynRadBoxIndices.at(i))->setText(tr(" %1 / 1 ").arg(hydroDynRadBoxIndices.size()));

   // check for multiple occurrences

   // ri entries
   if(riBoxIndices.size() > numberOfPeaks){
      for(int i = 0; i < riBoxIndices.size(); ++i)
         dimensionChecker->at(riBoxIndices.at(i))->setStyleSheet("QLabel { background-color : red; color : black; }");
      activateConfirmerLater = false;
   }
   else if(riBoxIndices.size() < numberOfPeaks){
      for(int i = 0; i < riBoxIndices.size(); ++i)
         dimensionChecker->at(riBoxIndices.at(i))->setStyleSheet("QLabel { background-color : #00BFFF; color : black; }");
      activateConfirmerLater = false;
   }
   else {
      for(int i = 0; i < riBoxIndices.size(); ++i)
         dimensionChecker->at(riBoxIndices.at(i))->setStyleSheet("QLabel { background-color : green; color : black; }");
   }
   for(int i = 0; i < riBoxIndices.size(); ++i)
      dimensionChecker->at(riBoxIndices.at(i))->setText(tr(" %1 / %2 ").arg(riBoxIndices.size()).arg(numberOfPeaks));
   // malls entries
   for(int k = 0; k < mallsBoxIndices.size(); ++k){
      if(mallsBoxIndices.at(k).size() > numberOfPeaks){
         for(int i = 0; i < mallsBoxIndices.at(k).size(); ++i){
            dimensionChecker->at(mallsBoxIndices.at(k).at(i))->setStyleSheet("QLabel { background-color : red; color : black; }");
         }
         activateConfirmerLater = false;
      }
      else if(mallsBoxIndices.at(k).size() < numberOfPeaks){
         for(int i = 0; i < mallsBoxIndices.at(k).size(); ++i){
            dimensionChecker->at(mallsBoxIndices.at(k).at(i))->setStyleSheet("QLabel { background-color : #00BFFF; color : black; }");
         }
         activateConfirmerLater = false;
      }
      else {
         for(int i = 0; i < mallsBoxIndices.at(k).size(); ++i){
            dimensionChecker->at(mallsBoxIndices.at(k).at(i))->setStyleSheet("QLabel { background-color : green; color : black; }");
         }
      }
      for(int i = 0; i < mallsBoxIndices.at(k).size(); ++i)
         dimensionChecker->at(mallsBoxIndices.at(k).at(i))->setText(tr(" %1 / %2 ").arg(mallsBoxIndices.at(k).size()).arg(numberOfPeaks));
   }

   // sed coeff
   if(sedCoeffIndices.size() > 1){
      for(int i = 0; i < sedCoeffIndices.size(); ++i)
         dimensionChecker->at(timeBoxIndices.at(i))->setStyleSheet("QLabel { background-color : red; color : black; }");
      activateConfirmerLater = false;
   }
   else {
      for(int i = 0; i < sedCoeffIndices.size(); ++i)
         dimensionChecker->at(sedCoeffIndices.at(i))->setStyleSheet("QLabel { background-color : green; color : black; }");
   }

   confirmer->setEnabled(activateConfirmerLater);
   checkPeakConfiguration(id, index);

}

void FFFAssignDataDialog::checkPeakConfiguration(int /* id */, int /* index */)
{
   bool activateConfirmerLater = true;
   QList<bool> checkedFalse;
   for (int i = 0; i < peakBoxes->size(); ++i){
      checkedFalse.append(false);
   }
   for(int i = 0; i < peakBoxes->size(); ++i){
      for(int j = i+1; j < peakBoxes->size(); ++j){
         if((dimensionBoxes->at(i)->currentIndex() > 2 )
               && (dimensionBoxes->at(j)->currentIndex() > 2)){
            if((peakBoxes->at(i)->currentIndex() == peakBoxes->at(j)->currentIndex())
                  && (dimensionBoxes->at(i)->currentIndex() == dimensionBoxes->at(j)->currentIndex())){
               peakChecker->at(i)->setStyleSheet("QLabel { background-color : red; color : black; }");
               peakChecker->at(j)->setStyleSheet("QLabel { background-color : red; color : black; }");
               peakChecker->at(i)->setText(tr(" >1 "));
               peakChecker->at(j)->setText(tr(" >1 "));
               if(dimensionBoxes->at(i)->currentIndex() > 4)
                  activateConfirmerLater = false;
               checkedFalse[j] = true;
               checkedFalse[i] = true;
            }
            else {
               if(!checkedFalse.at(i)){
                  peakChecker->at(i)->setStyleSheet("QLabel { background-color : green; color : black; }");
                  peakChecker->at(i)->setText(tr(" ok "));
               }
               if(!checkedFalse.at(j)){
                  peakChecker->at(j)->setStyleSheet("QLabel { background-color : green; color : black; }");
                  peakChecker->at(j)->setText(tr(" ok "));
               }
            }
         }
      }
   }
   confirmer->setEnabled(activateConfirmerLater);
}

void FFFAssignDataDialog::writeUserOptionAndAccept()
{

   posPeakPair ppPair;
   for (int i = 0; i < NUMBER_OF_DETECTORS; ++i){
      mallsIndices->append(QList<posPeakPair>());
   }
   *dCIndexChosen = false;
   *rSIndexChosen = false;

   for(int i = 0; i < dimensionBoxes->size(); ++i){
      if(dimensionBoxes->at(i)->currentIndex() == 0)
         *timeIndex = i;
      else if(dimensionBoxes->at(i)->currentIndex() == 1){
         *diffCoeffIndex = i;
         *dCIndexChosen = true;
      }
      else if(dimensionBoxes->at(i)->currentIndex() == 2){
         if(rSIndex ) *rSIndex = i;
         *rSIndexChosen = true;
      }
      // lists now
      else if(dimensionBoxes->at(i)->currentIndex() == 3){
         ppPair.position = i;
         ppPair.peak = peakBoxes->at(i)->currentIndex();
         riIndices->append(ppPair);
      }
      else if(4 <= dimensionBoxes->at(i)->currentIndex()
              && dimensionBoxes->at(i)->currentIndex() <= 11){
         ppPair.position = i;
         ppPair.peak = peakBoxes->at(i)->currentIndex();
         ((*mallsIndices)[dimensionBoxes->at(i)->currentIndex() - 4]).append(ppPair);
      } else if ( dimensionBoxes->at(i)->currentIndex() == 12 ) {
         *sedCoeffIndex = i;
      }
   }

   accept();
}

