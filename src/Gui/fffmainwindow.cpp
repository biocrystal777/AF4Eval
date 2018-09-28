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

   logDevice = new AF4Log(this);   
   logFrameLayout->addWidget(new QLabel("<b>Log Messages</b>", logFrame), 0, 0, Qt::AlignLeft);
   logFrameLayout->addWidget(logDevice, 1, 0);

   ///////////////////////////
   // Functionality widgets //
   ///////////////////////////

   funcTabWidget = new QTabWidget(this);
   qDebug() << "0";
   channelCalConfWidget = new AF4ChannelConfigurationWidget(funcTabWidget);
   funcTabWidget->addTab(channelCalConfWidget, "Channel Calibration");
   qDebug() << "0";
   layout = new QGridLayout(this);
   layout->addWidget(funcTabWidget, 0, 0);
   layout->addWidget(logFrame, 1, 0);
   this->show();
qDebug() << "0";
   diffEvaluationWidget = new AF4DiffEvaluationWidget(channelCalConfWidget->getChannelConfigWidgets(),
                                                      channelCalConfWidget->getChannelCalibWidgets(),
                                                      funcTabWidget);
   funcTabWidget->addTab(diffEvaluationWidget, "Diffusion Coefficients");

   qDebug() << "1";
   slsEvaluationWidget = new AF4SLSEvaluationWidget(funcTabWidget);
   qDebug() << "2";
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

