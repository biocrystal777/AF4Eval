#include "af4channeldimsorgframe.h"

AF4ChannelDimsOrgFrame::AF4ChannelDimsOrgFrame(QWidget *parent) : QFrame(parent)
{
   this->setFrameStyle(0x1011);
   lay = new QGridLayout(this);
   lay->addWidget(new QLabel("<b>Channel Configurations</b>", this), 0, 0, Qt::AlignLeft);

   channelSelection = QSharedPointer<QComboBox>(new QComboBox(this));
   channelSelection->setToolTip("");
   lay->addWidget(channelSelection.data(), 0, 4);

   auto makeButton = [this](QToolButton *&button, QString text, QString toolTip, int row, int column){
      button = new QToolButton(this);
      button->setText(text);
      button->setToolTip(toolTip);
      lay->addWidget(button, row, column);
   };

   makeButton(renameChButton, "R", "Rename the current channel", 0, 5);
   connect(renameChButton, &QPushButton::clicked, this, &AF4ChannelDimsOrgFrame::renameChannel);
   makeButton(addChButton,    "+", "Add new channel",            0, 6);
   connect(addChButton,    &QPushButton::clicked, this, &AF4ChannelDimsOrgFrame::addChannel);
   makeButton(deleteChButton, "-", "Delete current channel",     0, 7);
   connect(deleteChButton, &QPushButton::clicked, this, &AF4ChannelDimsOrgFrame::deleteChannel);

   ////////////////////////////////////////////////////
   // initialize channels with values from QSettings //
   ////////////////////////////////////////////////////
   channelConfigWidgets = QSharedPointer<QMap<QString, AF4ChannelDimsWidget*> > (new QMap<QString, AF4ChannelDimsWidget*>() );
   // make all channel widgets, add them to the QMap
   // of ChannelWidgets and insert them
   // into the ComboBox

   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   bool ok;
   uint numberOfChannels = settings.value("channels/number", 0).toInt(&ok);
   if(!ok) AF4Log::logWarning(tr("Could not read parameter channels/number from iniFile."), true);
   for(uint i = 0; i < numberOfChannels; i++){
      // double channelValue;
      QString newChannelName = settings.value(tr("channels/%1/name").arg(i), tr("New Channel %1").arg(i)).toString();
      currentChConfigWidget = new AF4ChannelDimsWidget(i, newChannelName, true, this);
      channelConfigWidgets->insert(newChannelName, currentChConfigWidget);
      channelSelection->addItem(newChannelName);
      channelSelection->setCurrentIndex(channelSelection->count() - 1);
      currentChConfigWidget->hide();
      lay->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   }
   if(numberOfChannels == 0)
      while (!addChannel(true));

   //currentChConfigWidget->show();
   channelSelection->setCurrentIndex(0);
   QString channelName = channelSelection->currentText();
   currentChConfigWidget = channelConfigWidgets->value(channelName);
   currentChConfigWidget->show();
   connect(channelSelection.data(), qOverload<const QString&>(&QComboBox::currentIndexChanged),
           this, &AF4ChannelDimsOrgFrame::switchChannelWidget );

}


auto AF4ChannelDimsOrgFrame::getChannelConfigWidgets()
-> QSharedPointer<QMap<QString, AF4ChannelDimsWidget *> >
{
   return channelConfigWidgets;
}

QWeakPointer<QComboBox> AF4ChannelDimsOrgFrame::getChannelSelection()
{
   return channelSelection;
}

ChannelDims AF4ChannelDimsOrgFrame::getChannelDimensions() const {
   return currentChConfigWidget->getChannelDimensions();
}


void AF4ChannelDimsOrgFrame::adaptConfigWidgetIds()
{
   int i = 0;
   for(const QString &key : channelConfigWidgets->keys()){
      AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
      configWidget->setConfigId(i);
      ++i;
   }
   emit configWidgetIdsAdapted();
}

void AF4ChannelDimsOrgFrame::adaptConfigWidgetNames()
{
   for(const QString &key : channelConfigWidgets->keys()){
      AF4ChannelDimsWidget *configWidget = channelConfigWidgets->value(key);
      configWidget->setChannelName(key);
   }
   emit configWidgetNamesAdapted();
}

bool AF4ChannelDimsOrgFrame::askChannelRenaming(QString &newName, const QString &oldName)
{
   bool firstDialog = true;
   do {
      AF4ChannelNameDialog chNameDialog(&newName, firstDialog, oldName, true);
      firstDialog = false;
      if(! chNameDialog.exec()) return false;
   } while(! ( channelNameDuplicated(newName) || newName.isEmpty() ) );
   return true;
}


void AF4ChannelDimsOrgFrame::renameChannel()
{

   QString oldName = QString(channelSelection->currentText());
   QString newName;
   if(askChannelRenaming(newName, oldName)){
      // add a new Channel here:
      AF4ChannelDimsWidget *oldConfWidget = channelConfigWidgets->value(oldName, nullptr);
      if(!oldConfWidget){
         AF4Log::logError(tr("The channel profile %1 could not be renamed; Old Name not found!").arg(oldName), true);
         return;
      }
      channelConfigWidgets->remove(oldName);
      channelConfigWidgets->insert(newName, oldConfWidget);
      channelSelection->setItemText(channelSelection->currentIndex(), newName);
      oldConfWidget->setChannelName(newName);
      // remove and reinsert the calibSelection and the FFFChannelselection
      // under the new key;     
      emit channelRenamed(oldName, newName);      
      saveParameters();
   }
}

bool AF4ChannelDimsOrgFrame::askChannelAdding(QString &newName){

   bool firstDialog = true;
   do {
      if(!firstDialog) AF4Log::logWarning(tr("Other Name has to be entered"));
      AF4ChannelNameDialog chNameDialog(&newName, firstDialog);
      firstDialog = false;
      if(!chNameDialog.exec()) return false;
   } while(!channelNameDuplicated(newName));
   return true;
}

bool AF4ChannelDimsOrgFrame::channelNameDuplicated(const QString &newName) const {
   bool nameIsOk = true;
   if(!channelSelection.isNull())
      for(int i=0; i < channelSelection->count() ; i++){
         QString compName = channelSelection->itemText(i);
         if(!(QString::compare(newName, compName))){
            nameIsOk = false;
            break;
         }
      }
   else
      AF4Log::logWarning(tr("New name was not checked for duplication."),true);
   return nameIsOk;
}

bool AF4ChannelDimsOrgFrame::addChannel(bool firstInit)
{
   QApplication::restoreOverrideCursor();
   QString newName;
   if(!askChannelAdding(newName)) return false;
   // add a new Channel here:
   QString oldName = channelSelection->currentText();
   AF4ChannelDimsWidget* newChannel = new AF4ChannelDimsWidget(channelConfigWidgets->size(), newName, true, this);
   channelConfigWidgets->insert(newName, newChannel);
   if(currentChConfigWidget) currentChConfigWidget->hide();
   currentChConfigWidget = newChannel;
   // add new assigned channelCalibWidget, Comboboxes etc.

   channelSelection->blockSignals(true);
   lay->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   currentChConfigWidget->show();
   channelSelection->addItem(newName);
   channelSelection->setCurrentIndex(channelSelection->count() - 1);
   channelSelection->blockSignals(false);
   AF4Log::logText(tr("New Channel \"%1\" added.").arg(newName));
   emit channelAdded(newName);
   if(!firstInit){
      adaptConfigWidgetIds();
      adaptConfigWidgetNames();
   }
      saveParameters();
   return true;
}


void AF4ChannelDimsOrgFrame::deleteChannel()
{
   if(channelSelection->count() <= 1){
      AF4Log::logWarning(tr("You cannot remove the last Channel!"));
      return;
   }
   AF4DeleteChannelDialog deleteChDialog;
   if(!deleteChDialog.exec()) return;
   const QString channelToRemove = channelSelection->currentText();
   int indexToRemove = channelSelection->currentIndex();
   channelSelection->removeItem(indexToRemove);
   delete (channelConfigWidgets->value(channelToRemove));
   channelConfigWidgets->remove(channelToRemove);
   AF4Log::logText(tr("Channel deleted."));
   QString channelToDisplay = channelSelection->currentText();
   emit channelDeleted(channelToRemove, channelToDisplay);
   adaptConfigWidgetIds();
   adaptConfigWidgetNames();
   saveParameters();
}

void AF4ChannelDimsOrgFrame::switchChannelWidget(const QString &channelName)
{
   currentChConfigWidget->hide();
   currentChConfigWidget = channelConfigWidgets->value(channelName);
   //lay->addWidget(currentChConfigWidget, 2, 0, 7, 7);
   emit channelSwitched(channelName);
   currentChConfigWidget->show();
}

void AF4ChannelDimsOrgFrame::saveParameters() const
{
   AF4Log::logText(tr("Parameters saved of Channel Calibrations saved."));
   writeSettings();
   for(const QString &configWidgetKey : channelConfigWidgets->keys()){
      channelConfigWidgets->value(configWidgetKey)->writeSettings();
   }
}

void AF4ChannelDimsOrgFrame::writeSettings() const
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   settings.remove("channels");
   int numberOfChannels = channelSelection->count();
   settings.setValue("channels/number", numberOfChannels);
}
