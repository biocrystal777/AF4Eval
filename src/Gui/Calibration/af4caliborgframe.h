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
#include "./a.out.h"
#include "./af4channeldimswidget.h"
#include "../../Core/af4calibrator.h"
#include "../../Core/af4csvwriter.h"

/*! **************************************************************************************************************
***
***  \class     AF4ChannelCalibsOrgFrame "src/Gui/Calibration/af4channelconfigurationwidget.h"
***  \brief
***  \details
***  \author    Benedikt Häusele
***  \version   1.1
***  \date      2018-08-31
***  \todo      Put Frames into single classes
***  \copyright CC CC BY-NC-ND 4.0
***
*****************************************************************************************************************/

class AF4CalibOrgFrame final : public QFrame {

   Q_OBJECT

public:

   explicit AF4CalibOrgFrame(QSharedPointer<QComboBox> channelSelection,
                             QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets,
                             QWidget *parent);
   ~AF4CalibOrgFrame();


   auto getChannelCalibWidgets() const -> QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > {
      return this->channelCalibWidgets;
   }

   ParametersForCalibration getParamsForCalibration() const;

   CalibModeSettings getCalibModes() const;


   void adaptAllCalibWidgetIds();
   void adaptAllCalibWidgetNames();
   void renameConnectedChannel(const QString &oldChName, const QString &newChName);
   //void renameChannelConnection(const QString &newChName, const QString &newChName);
   void addConnectedChannel(const QString &newChName);
   void deleteConnectedChannel(const QString &rmChName, const QString newChToDisplayName);
public slots:
   void adaptPlotData();

   void switchToFirstCalibWidget(const QString& channelName);

signals:
   void calibrateChannelCalled();

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
   void switchCalibWidget(const QString &calibName);

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;



private:
   QSharedPointer <QComboBox> channelSelection; // transitory solution for class split; replace by slot connnections later


   //QFrame *calibrationFrame                                                    = nullptr;
   QGridLayout *lay                                         = nullptr;

   //QFrame calibrationFrame = nullptr;
   AF4CalibPlotWidget *plotWidget                                              = nullptr;

   QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets;
   QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget*> > > channelCalibWidgets;
   QSharedPointer<QPushButton> settingsWriter;//                                  = nullptr;

   AF4ChannelCalibWidget     *currentCalibWidget                               = nullptr;
   QMap<QString, QComboBox*> *allCalibSelections                               = nullptr;
   QComboBox                 *calibSelection                            = nullptr;
   QToolButton               *addCalibButton                                   = nullptr;
   QToolButton *renameCalibButton                                              = nullptr;
   QToolButton *deleteCalibButton                                              = nullptr;


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
    * \brief askCalibAdding
    * \param newName
    * \return
    */
   bool askCalibAdding(QString &newName);

   /*!
    * \brief connectCtrlWithPlotWidget
    */
   void connectCtrlWithPlotWidget();

   /*!
    * \brief writeSettings writes Parameters to QSettings
    */
   void writeSettings() const;

   AF4CalibOrgFrame( const      AF4CalibOrgFrame& src)  = delete;
   AF4CalibOrgFrame& operator= (AF4CalibOrgFrame& src)  = delete;
   AF4CalibOrgFrame(            AF4CalibOrgFrame&& src) = delete;
   AF4CalibOrgFrame& operator= (AF4CalibOrgFrame&& src) = delete;
};

#endif // AF4CALIBORGFRAME_H
