#ifndef AF4CHANNELDIMSORGFRAME_H
#define AF4CHANNELDIMSORGFRAME_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include "./af4caliborgframe.h"
#include "../Core/af4evalmacros.h"

class AF4ChannelDimsOrgFrame final : public QFrame {
   Q_OBJECT
public:
   /*!
    * \brief AF4ChannelDimsOrgFrame
    * \param parent
    */
   explicit AF4ChannelDimsOrgFrame(QWidget *parent);
   /*!
    * \brief getChannelConfigWidgets
    * \return
    */
   auto getChannelConfigWidgets() -> QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> >;
   /*!
    * \brief getChannelSelection
    * \return
    */
   auto getChannelSelection() -> QWeakPointer<QComboBox >;
   /*!
    * \brief getChannelDimensions
    * \return
    */
   ChannelDims getChannelDimensions() const;

signals:
   /*!
    * \brief configWidgetIdsAdapted
    */
   void configWidgetIdsAdapted();     // connect to calibOrgsFrame => adaptAllCalibWidgetIds   
   /*!
    * \brief configWidgetNamesAdapted
    */
   void configWidgetNamesAdapted();   // connect to                => adaptAllCalibWidgetIds TO CHECK: or Names?

   /*!
    * \brief channelAdded
    * \param newName
    */
   void channelAdded(const QString newName);        // connect to calibOrgsFrame =>
   /*!
    * \brief channelRenamed
    * \param oldName
    * \param newName
    */
   void channelRenamed(const QString oldName, const QString newName);
   /*!
    * \brief channelDeleted
    * \param oldName
    * \param newCurrentName
    */
   void channelDeleted(const QString channelToRemove, const QString newCurrentName);
   /*!
    * \brief channelSwitched
    * \param newName
    */
   void channelSwitched(const QString newName);
   /*!
    * \brief saveButtonClicked
    */
   void saveButtonClicked();
public slots:

   /*!
    * \brief renameChannel creates a dialog for renaming a channel.
    *        Adapts keys of internal data structures
    */
   void renameChannel();

   /*!
    * \brief addCalibration creates a dialog and a new channel.
    *        Adapts internal data structures.
    */
   bool addChannel(bool firstInit = false);

   /*!
    * \brief deleteCalibration removes the current channel
    *        and its assigned calibration profiles. Adapts
    *        internal data structures.
    *
    */
   void deleteChannel();

private slots:

   /*!
    * \brief saveParameters
    */
   void saveParameters() const;

   /*!
    * \brief switchCalibWidget switches to another channel and
    *        shows its values. Shows a comboBox with its assigned
    *        calibration profiles as well as the corresponding
    *        FFFChannelcalibWidgets
    * \param newWidgetKey
    */
   void switchChannelWidget(const QString &newWidgetKey);
private:
   /*!
    * \brief writeSettings
    */
   void writeSettings() const;
   /*!
    * \brief adaptConfigWidgetIds
    */
   void adaptConfigWidgetIds();

   /*!
    * \brief adaptConfigWidgetNames
    */
   void adaptConfigWidgetNames();

   /*!
    * \brief askChannelRenaming
    * \param newName
    * \param oldName
    * \return
    */
   bool askChannelRenaming(QString &newName, const QString &oldName);

   /*!
    * \brief askChannelAdding
    * \param newName
    * \return
    */
   bool askChannelAdding(QString &newName);

   /*!
    * \brief channelNameDuplicated
    * \param newName
    * \return
    */
   bool channelNameDuplicated(const QString &newName) const;

   QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets;//  = nullptr;
   QGridLayout *lay                                             = nullptr;
   AF4ChannelDimsWidget *currentChConfigWidget                  = nullptr;
   QSharedPointer<QComboBox> channelSelection;
   QToolButton *addChButton                                     = nullptr;
   QToolButton *renameChButton                                  = nullptr;
   QToolButton *deleteChButton                                  = nullptr;

   ~AF4ChannelDimsOrgFrame(){}
   NO_COPY_ASSIGNMENT_CTORS(AF4ChannelDimsOrgFrame)
};
#endif // AF4CHANNELDIMSORGFRAME_H
