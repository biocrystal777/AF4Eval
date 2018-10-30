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
   explicit AF4ChannelDimsOrgFrame(QWidget *parent);

   auto getChannelConfigWidgets() const -> QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> >   {
      return channelConfigWidgets;
   }
   auto getChannelSelection() const -> QWeakPointer<QComboBox >   {
      return channelSelection;
   }
   ChannelDims getChannelDimensions() const {
      return currentChConfigWidget->getChannelDimensions();
   }


   void writeSettings() const;
signals:
   void configWidgetIdsAdapted();     // connect to calibOrgsFrame => adaptAllCalibWidgetIds
   void configWidgetNamesAdapted();   // connect to                => adaptAllCalibWidgetIds TO CHECK: or Names?
   void channelAdded(const QString newName);        // connect to calibOrgsFrame =>
   void channelRenamed(const QString oldName, const QString newName);
   void channelDeleted(const QString oldName, const QString newCurrentName);
   void channelSwitched(const QString newName);

   void saveButtonClicked();
public slots:



   /*!
    * \brief renameChannel creates a dialog for renaming a channel.
    *        Adapts keys of internal data structures
    */
   void renameChannel();  // <- can produce bugs

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
   void deleteChannel();   //   <- still kind of buggy

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

   QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > channelConfigWidgets;//  = nullptr;
   QGridLayout *lay                                             = nullptr;
   AF4ChannelDimsWidget *currentChConfigWidget                  = nullptr;
   QSharedPointer<QComboBox> channelSelection;
   QToolButton *addChButton                                     = nullptr;
   QToolButton *renameChButton                                  = nullptr;
   QToolButton *deleteChButton                                  = nullptr;

   NO_COPY_ASSIGNMENT_CTORS(AF4ChannelDimsOrgFrame)
};

#endif // AF4CHANNELDIMSORGFRAME_H
