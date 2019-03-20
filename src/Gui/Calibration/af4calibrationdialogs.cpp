#include "af4calibrationdialogs.h"

AF4ChannelNameDialog::AF4ChannelNameDialog(QString *const name, bool first, const QString nameSuggestion, bool rename) :
   channelName(name)
{
   layout = new QGridLayout(this);
   if(first)
      layout->addWidget(new QLabel("Specify Channel Name:"), 0, 0, 1, 2, Qt::AlignLeft);
   else{
      layout->addWidget(new QLabel(tr("<font color=\"#FF4400\"><i>\"%1\" exists already.</i></font>").arg(*channelName)),
                        0, 0, 1, 2, Qt::AlignLeft);
      layout->addWidget(new QLabel(tr("Please specify another name:")), 1, 0, 1, 2, Qt::AlignLeft);
   }
   QString defText;
   if(nameSuggestion == 0) defText = QString("New Channel");
   else defText = nameSuggestion;
   channelNameInput = new QLineEdit(defText, this);
   layout->addWidget(channelNameInput, 2, 0, 1, 2);

   if(rename) accepter = new QPushButton("Rename Channel", this);
   else accepter = new QPushButton("Add Channel", this);
   connect(accepter, &QPushButton::clicked,
           this, [this]() {
      *channelName = QString(channelNameInput->text());
      accept();
   });
   layout->addWidget(accepter, 3, 0);
   decliner = new QPushButton("Close");
   connect(decliner, &QPushButton::clicked, this, &AF4ChannelNameDialog::reject);
   layout->addWidget(decliner, 3, 1);

   channelNameInput->setFocus();
}

AF4CalibNameDialog::AF4CalibNameDialog(QString *name, bool first, const QString nameSuggestion, bool rename)
{
   calibName = name;
   layout = new QGridLayout(this);
   if(first)
      layout->addWidget(new QLabel("Specify Calibration Name:"), 0, 0, 1, 2, Qt::AlignLeft);
   else{
      layout->addWidget(new QLabel(tr("<font color=\"#FF4400\"><i>\"%1\" exists already.</i></font>").arg(*calibName)),
                        0, 0, 1, 2, Qt::AlignLeft);
      layout->addWidget(new QLabel(tr("Please specify another name:")), 1, 0, 1, 2, Qt::AlignLeft);
   }
   QString defText;
   if(nameSuggestion == 0) defText = QString("New Calibration");
   else defText = nameSuggestion;
   calibNameInput = new QLineEdit(defText, this);
   layout->addWidget(calibNameInput, 2, 0, 1, 2);

   if(rename) accepter = new QPushButton("Rename Calibration", this);
   else accepter = new QPushButton("Add Calibration", this);
   connect(accepter, &QPushButton::clicked,
           this, [this](){
      *calibName = QString(calibNameInput->text());
      accept();
   } );
   layout->addWidget(accepter, 3, 0);
   decliner = new QPushButton("Close");
   connect(decliner, &QPushButton::clicked, this, &AF4CalibNameDialog::reject);
   layout->addWidget(decliner, 3, 1);

   calibNameInput->setFocus();
}

AF4DeleteCalibDialog::AF4DeleteCalibDialog()
{
   layout = new QGridLayout(this);
   layout->addWidget(new QLabel("Do you really want to delete the current Calibration?"), 0, 0, 1, 2, Qt::AlignLeft);
   accepter = new QPushButton("Yes", this);
   connect(accepter, &QPushButton::clicked, this, &QDialog::accept);
   layout->addWidget(accepter, 1, 0);
   decliner = new QPushButton("Close");
   connect(decliner, &QPushButton::clicked, this, &QDialog::reject);
   layout->addWidget(decliner, 1, 1);
}

AF4DeleteChannelDialog::AF4DeleteChannelDialog()
{
   layout = new QGridLayout(this);
   layout->addWidget(new QLabel("Do you really want to delete the displayed channel\n and its assigned calibrations?"), 0, 0, 1, 2, Qt::AlignLeft);
   accepter = new QPushButton(" I know, what I am doing!", this);
   connect(accepter, &QPushButton::clicked, this, &QDialog::accept);
   layout->addWidget(accepter, 1, 0);
   decliner = new QPushButton("Close");
   connect(decliner, &QPushButton::clicked, this, &QDialog::reject);
   layout->addWidget(decliner, 1, 1);
}
