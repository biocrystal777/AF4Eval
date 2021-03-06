#ifndef AF4CALIBORGFRAME_H
#define AF4CALIBORGFRAME_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include "af4calibrationdialogs.h"
#include "./af4calibplotwidget.h"
#include "./af4channelcalibwidget.h"
#include "./af4channeldimswidget.h"
#include "../../Core/af4evalmacros.h"
#include "../../Core/af4calibrator.h"
#include "../../Core/af4csvwriter.h"

/*! **************************************************************************************************************
***
***  \class     AF4CalibOrgFrame "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-08-31
***  \todo
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/

class AF4CalibOrgFrame final : public QFrame {

   Q_OBJECT

public:
   /*!
    * \brief AF4CalibOrgFrame
    * \param channelSelection
    * \param channelConfigWidgets
    * \param parent
    */
   explicit AF4CalibOrgFrame(QWeakPointer<QComboBox> channelSelection,
                             QWeakPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets,
                             QWidget *parent);
   /*!
    */
   ~AF4CalibOrgFrame();

   /*!
    * \brief getChannelCalibWidgets
    * \return
    */
   auto getChannelCalibWidgets() const -> QWeakPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > {
      return this->channelCalibWidgets.toWeakRef();
   }
   /*!
    * \brief getParamsForCalibration
    * \return
    */
   ParametersForCalibration getParamsForCalibration() const;

   /*!
    * \brief getCalibModes
    * \return
    */
   CalibModeSettings getCalibModes() const;

public slots:
   /*!
    * \brief adaptPlotData
    */
   void adaptPlotData();
   /*!
    * \brief switchToFirstCalibWidget
    * \param channelName
    */
   void switchToFirstCalibWidget(const QString channelName);
   /*!
    * \brief adaptAllCalibWidgetIds
    */
   void adaptAllCalibWidgetIds();
   /*!
    * \brief adaptAllCalibWidgetNames
    */
   void adaptAllCalibWidgetNames();
   /*!
    * \brief renameConnectedChannel
    * \param oldChName
    * \param newChName
    */
   void renameConnectedChannel(const QString oldChName, const QString newChName);
   /*!
    * \brief addConnectedChannel
    * \param newChName
    */
   void addConnectedChannel(const QString newChName);
   /*!
    * \brief deleteConnectedChannel
    * \param rmChName
    * \param newChToDisplayName
    */
   void deleteConnectedChannel(const QString channelToRemove, const QString channelToDisplay);


   void setClassicalVolume(double value){ curCalibWidget->setClassicalVolume(value); }

   void setHydrodynVolume(double value){ curCalibWidget->setHydrodynVolume(value); }

   void setGeometVolume(double value){ curCalibWidget->setGeometVolume(value); }
   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidthClassical(double value) { curCalibWidget->setChannelWidthClassical(value); }

   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidthGeo(double value) { curCalibWidget->setChannelWidthGeo(value); }

   /*!
    * \brief setChannelWidth set value of the channelWidth
    * \return bool if value could be set
    */
   void setChannelWidthHydro(double value) { curCalibWidget->setChannelWidthHydro(value); }



signals:
   /*!
    * \brief calibrateChannelCalled
    */
   void calibrateChannelCalled();
   /*!
    * \brief saveButtonClicked
    */
   void saveButtonClicked();
private slots:
   /*!
    * \brief addCalibration creates a dialog and a new calibration profile
    *        Adapts internal data structures
    */
   bool addCalibration();

   /*!
    * \brief renameCalibration creates a dialog for renaming a calibration profile.
    *        Adapts keys of internal data structures
    */
   void renameCalibration();

   /*!
    * \brief deleteCalibration removes the current calibration profile and
    *        adapts internal data structures
    */
   void deleteCalibration();

   /*!
    * \brief switchCalibWidget switches to another calibration profile and
    *        shows its values
    * \param newWidgetKey
    */
   void switchCalibWidget(const QString calibName);

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;

private:   

   /*!
    * \brief createNewCalilbWidget
    * \param channelId
    * \param calibId
    * \param channelName
    * \param calibName
    * \return
    */
   AF4ChannelCalibWidget *createNewCalilbWidget(const int channelId, const int calibId,
                                                const QString &channelName, const QString &calibName);
   /*!
    * \brief adaptCalibWidgetIds
    * \param channelName
    * \param newChannelId
    * \param caller
    */
   void adaptCalibWidgetIds(const QString &oldName, int newChannelId = -1);

   /*!
    * \brief adaptCalibWidgetNames
    * \param channelName
    */
   void adaptCalibWidgetNames(const QString &channelName);

   /*!
    * \brief askCalibRenaming
    * \param newName
    * \param oldName
    * \return
    */
   bool askCalibRenaming(QString &newName, const QString &oldName);

   /*!
    * \brief calibNameDuplicated
    * \param newName
    * \return
    */
   bool calibNameDuplicated(const QString& newName);

   /*!
    * \brief askCalibAdding
    * \param newName
    * \return
    */
   bool askCalibAdding(const QString &channelName, QString &newName);

   /*!
    * \brief connectCtrlWithPlotWidget
    */
   void connectCtrlWithPlotWidget();

   void connectInputFileWithPlotWidget();

   QWeakPointer<QComboBox> channelSelection; // transitory solution for class split; replace by slot connnections later
   QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> >                  channelConfigWidgets;
   QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > channelCalibWidgets;
   QGridLayout *lay                                         = nullptr;
   AF4CalibPlotWidget *plotWidget                                              = nullptr;
   AF4ChannelCalibWidget     *curCalibWidget                               = nullptr; // replace by private getter!

   QMap<QString, QComboBox*> allCalibSelections;
   QComboBox                 *calibSelection                                  = nullptr;
   QToolButton               *addCalibButton                                   = nullptr; // replace by private getter!
   QToolButton *renameCalibButton                                              = nullptr;
   QToolButton *deleteCalibButton                                              = nullptr;

   NO_COPY_MOVE_CTORS(AF4CalibOrgFrame)
};

#endif // AF4CALIBORGFRAME_H
