#include "fffmainwindow.h"

FFFMainWindow::FFFMainWindow(QWidget *parent)
   : QWidget(parent), lastChosenTab(0)
{
   this->hide();
   QGuiApplication::setOverrideCursor(Qt::WaitCursor);
   QIcon icon(":/images/applIcon.svg");
   setWindowIcon(icon);
   QSettings settings("AgCoelfen", "FFFEval");   
   settings.setIniCodec("UTF-8");

   bool ok;

   const QRect rec = QApplication::desktop()->availableGeometry();
   const uint screenWidth  = rec.width();
   const uint screenHeight = rec.height();

   this->setMinimumSize(screenWidth/10*9, screenHeight/10*9);
   this->setMaximumSize(screenWidth/10*9, screenHeight/10*9);
   this->resize(settings.value("window/width", 1000).toInt(&ok),
                settings.value("window/height", 700).toInt(&ok));
   CHECK_SETTINGS_CONVERSION("window/width or window/size", "1000, 800 respectively");
   this->move(settings.value("window/xpos", 100).toInt(&ok),
              settings.value("window/ypos", 100).toInt(&ok));
   CHECK_SETTINGS_CONVERSION("window/xpos or window/ypos", "10, 10 respectively");

   /////////////////////
   // log Frame first //
   /////////////////////
   logFrame = new QFrame(this);   
   logFrame->setFrameStyle(0x1011);
   logFrameLayout = new QGridLayout(logFrame);
   logFrame->setMinimumSize(this->minimumSize().width()/5*4, this->minimumSize().width()/20/3);
   logFrame->setMinimumWidth(this->maximumSize().width()/20/3);

   logDevice = new FFFLog(this);   
   logFrameLayout->addWidget(new QLabel("<b>Log Messages</b>", logFrame), 0, 0, Qt::AlignLeft);
   logFrameLayout->addWidget(logDevice, 1, 0);

   ///////////////////////////
   // Functionality widgets //
   ///////////////////////////

   funcTabWidget = new QTabWidget(this);
   channelCalConfWidget = new FFFChannelCalConfWidget(funcTabWidget);
   funcTabWidget->addTab(channelCalConfWidget, "Channel Calibration");

   layout = new QGridLayout(this);
   layout->addWidget(funcTabWidget, 0, 0);
   layout->addWidget(logFrame, 1, 0);
   this->show();

   diffEvaluationWidget = new FFFDiffEvaluationWidget(channelCalConfWidget->getChannelConfigWidgets(),
                                                      channelCalConfWidget->getChannelCalibWidgets(),
                                                      funcTabWidget);
   funcTabWidget->addTab(diffEvaluationWidget, "Diffusion Coefficients");


   slsEvaluationWidget = new FFFSLSEvaluationWidget(funcTabWidget);
   funcTabWidget->addTab(slsEvaluationWidget, "SLS Signal Evaluation");

   this->show();
   QGuiApplication::restoreOverrideCursor();
}

FFFMainWindow::~FFFMainWindow()
{
   writeSettings();
}

void FFFMainWindow::writeSettings() const
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   settings.setValue("window/height", this->height());
   settings.setValue("window/width", this->width());
   settings.setValue("window/xpos", this->pos().x());
   settings.setValue("window/xpos", this->pos().y());
}

FFFChannelNameDialog::FFFChannelNameDialog(QString *name, bool first, const QString nameSuggestion, bool rename)
{
   channelName = name;
   setFixedSize(250, 120);
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
   QObject::connect(accepter, SIGNAL(clicked()), this, SLOT(acceptName()));
   layout->addWidget(accepter, 3, 0);
   decliner = new QPushButton("Close");
   QObject::connect(decliner, SIGNAL(clicked()), this, SLOT(reject()));
   layout->addWidget(decliner, 3, 1);

   channelNameInput->setFocus();
}
