#ifndef AF4PEAKPARAMETERFRAME_H
#define AF4PEAKPARAMETERFRAME_H

#include <QSettings>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include "../GeneralWidgets/af4log.h"
#include "../smallQDerivates/af4scinotspinbox.h"
#include "../smallQDerivates/af4numberedtoolbutton.h"
#include "../../Core/af4parameterstructs.h"

// forward declaration to use this as exclusive
// parent widget type for AF4SinglePeakParameterWidget
// ----> Pseudo-Nesting :-)
class AF4PeakParameterFrame;

/*! **********************************************************************************************
***
***  \class   AF4SinglePeakParameterWidget "src/Gui/Calibration/af4peakparameterframe.h"
***  \brief   contains the material specific data for a peak for MALLS evaluation
***  \details
***  \author    Benedikt Häusele
***  \version   0.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4SinglePeakParameterWidget final : public QFrame
{
   Q_OBJECT
public:
   /*!
    * \brief AF4SingelPeakParameterWidget constructs a new instance of this class
    *        and loads settings according to its peakNo
    * \param peakNo
    * \param parent
    */
   AF4SinglePeakParameterWidget(int peakNo,
                                bool checkSettings,
                                const QString &parentPrefix,
                                const int parentId,
                                AF4PeakParameterFrame *parent);
   /*!
    * \brief destructor calls settings;
    */
   ~AF4SinglePeakParameterWidget();

   AF4SinglePeakParameterWidget(const AF4SinglePeakParameterWidget& src)        = delete;
   AF4SinglePeakParameterWidget& operator= (AF4SinglePeakParameterWidget& src)  = delete;
   AF4SinglePeakParameterWidget(AF4SinglePeakParameterWidget&& src)             = delete;
   AF4SinglePeakParameterWidget& operator= (AF4SinglePeakParameterWidget&& src) = delete;

   /*!
    * \brief changePeakNo
    * \param newPeakNo
    */
   void changePeakNo(int newPeakNo);

   /*!
    * \brief makeSingleSPPW
    * \param makeSingle
    */
   void makeSingleSPPW(bool makeSingle);

    Peakproperties getPeakProperties() const;

   /*!
    * \brief getStartTime
    * \return
    */
   inline double    getStartTime()              const { return this->startTime->value(); }
   /*!
    * \brief getEndTime
    * \return
    */
   inline double    getEndTime()                const { return this->endTime->value();   }
   /*!
    * \brief getSecondVirUse
    * \return
    */
   inline bool     getSecondVirUse()        const { return this->useSecondVirCoeff->isChecked(); }
   /*!
    * \brief getSecondVirCoeff
    * \return
    */
   inline double   getSecondVirCoeff()      const { return this->secondVirCoeff->value();        }
   /*!
    * \brief getThirdVirUse
    * \return
    */
   inline bool     getThirdVirUse()         const { return this->useThirdVirCoeff->isChecked();  }
   /*!
    * \brief getThirdVirCoeff
    * \return
    */
   inline double   getThirdVirCoeff()       const { return this->thirdVirCoeff->value();         }
   /*!
    * \brief getRefIndexInc
    * \return
    */
   inline double   getRefIndexInc()         const { return this->refIndexIncrement->value();     }
   /*!
    * \brief getWaveLength
    * \return
    */
   inline double   getWaveLength()          const { return this->waveLength->value();            }
   /*!
    * \brief getEpsilon
    * \return
    */
   inline double   getEpsilon()             const { return this->epsilon->value();               }
   /*!
    * \brief getFractionDescription
    * \return
    */
   inline QString  getFractionDescription() const { return this->fractionDescription->text();    }


public slots:
   /*!
    * \brief adaptConcentrationMode
    * \param mode
    */
   void adaptConcentrationMode(SLSConcMode mode);

private:

   int peakNo;
   const QString parentPrefix;
   const int parentId;


   QGridLayout           *lay                 = nullptr;
   //QLabel                *label               = nullptr;
   QDoubleSpinBox        *startTime           = nullptr;
   QDoubleSpinBox        *endTime             = nullptr;
   QCheckBox             *useSecondVirCoeff   = nullptr;
   AF4SciNotSpinBox       *secondVirCoeff      = nullptr;
   QCheckBox             *useThirdVirCoeff    = nullptr;
   AF4SciNotSpinBox       *thirdVirCoeff       = nullptr;
   QDoubleSpinBox        *refIndexIncrement   = nullptr;
   QDoubleSpinBox        *waveLength          = nullptr;
   AF4SciNotSpinBox       *epsilon             = nullptr;
   QCheckBox             *useKnownMW          = nullptr;
   AF4SciNotSpinBox       *knownMW             = nullptr;
   QLineEdit             *fractionDescription = nullptr;
   AF4NumberedToolButton *removeButton        = nullptr;

   /*!
    * \brief loadSettings
    */
   void loadSettings();
   /*!
    * \brief writeSettings
    */
   void writeSettings();
};


/*! **********************************************************************************************
***
***  \class   AF4PeakParameterFrame "src/Gui/Calibration/af4peakparameterframe.h"
***  \brief   contains a variable number of AF4SinglePeakParameterWidget
***  \details
***  \author    Benedikt Häusele
***  \version   0.1
***  \date      2018-08-31
***  \copyright CC CC BY-NC-ND 4.0
***
*************************************************************************************************/

class AF4PeakParameterFrame final : public QFrame
{
   Q_OBJECT
public:
   /*!
    * \brief AF4PeakParameterFrame
    * \param prefix
    * \param id
    * \param parent
    */
   AF4PeakParameterFrame(const QString& prefix, int id, QWidget *parent);
   /*!
    * \brief destructor, calls writeSettings
    */
   ~AF4PeakParameterFrame();

   AF4PeakParameterFrame(const AF4PeakParameterFrame& src)        = delete;
   AF4PeakParameterFrame& operator= (AF4PeakParameterFrame& src)  = delete;
   AF4PeakParameterFrame(AF4PeakParameterFrame&& src)             = delete;
   AF4PeakParameterFrame& operator= (AF4PeakParameterFrame&& src) = delete;

   /*!
    * \brief getNumberOfPeaks
    * \return number of peak widgets
    */
   int getNumberOfPeaks() const { return peakParWidgets.size(); }

   /*!
    * \brief getSampleProperties
    * \return
    */
   SampleProperties getSampleProperties() const;

   /*!
    * \brief getStartTimes
    * \return
    */
   QList<double> getStartTimes() const;
   /*!
    * \brief getEndTimes
    * \return
    */
   QList<double> getEndTimes() const;
   /*!
    * \brief getSecondVirUses
    * \return
    */
   QList<bool>   getSecondVirUses() const;
   /*!
    * \brief getSecondVirCoeffs
    * \return
    */
   QList<double> getSecondVirCoeffs() const;
   /*!
    * \brief getThirdVirUses
    * \return
    */
   QList<bool>   getThirdVirUses() const;
   /*!
    * \brief getThirdVirCoeffs
    * \return
    */
   QList<double> getThirdVirCoeffs() const;
   /*!
    * \brief getRefIndexIncs
    * \return
    */
   QList<double> getRefIndexIncs() const;
   /*!
    * \brief getRefIndexIncs
    * \return
    */
   QList<double> getWaveLengths() const;
   /*!
    * \brief getEpsilons
    * \return
    */
   QList<double> getEpsilons() const;
   /*!
    * \brief getFractionDescriptions
    * \return
    */
   QList<QString> getFractionDescriptions() const;
   /*!
    * \brief write Settings of the widget
    */
   void writeSettings();
public slots:
   /*!
    * \brief adoptConcentrationMode
    * \param mode
    */
   void adoptConcentrationMode(SLSConcMode mode); // 1: RI, 2: Absorbance
   /*!
    * \brief removePeakParWidget
    * \param peakNo
    */
   void removePeakParWidget(const int peakNo);

private:

   const QString prefix;
   const int id;

   QGridLayout *lay           = nullptr;
   QScrollArea *scrollArea    = nullptr;
   QWidget     *areaWidget    = nullptr;
   QHBoxLayout *peakParLayout = nullptr;
   QPushButton *peakAdder     = nullptr;
   QList<AF4SinglePeakParameterWidget*> peakParWidgets;

private slots:
   /*!
    * \brief addPeakParWidget
    */
   void addPeakParWidget();

};

#endif // AF4PEAKPARAMETERFRAME_H
