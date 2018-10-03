#include "af4mainwindow.h"

AF4MainWindow::AF4MainWindow(QWidget *parent)
   : QWidget(parent)//, lastChosenTab(0)
{
   this->hide();
   QGuiApplication::setOverrideCursor(Qt::WaitCursor);
   QIcon icon(":/images/applIcon.svg");
   setWindowIcon(icon);

   //-//////////////////////////////////////
   // get settings and adjust window size //
   //-//////////////////////////////////////
   {
      QSettings settings("AgCoelfen", "FFFEval");
      settings.setIniCodec("UTF-8");

      bool ok;
      const QRect rec = QApplication::desktop()->availableGeometry();
      const uint screenWidth  = rec.width();
      const uint screenHeight = rec.height();


#define CHECK_SETTINGS_CONVERSION(keyName, defaultValueName) { \
   if(!ok){ \
   AF4Log::logWarning(tr("Could not read parameter %1 from iniFile. Value will be set to %2") \
   .arg(keyName).arg(defaultValueName)); \
   }\
   };

      this->setMinimumSize(screenWidth/10*9, screenHeight/10*9);
      this->setMaximumSize(screenWidth/10*9, screenHeight/10*9);
      this->resize(settings.value("window/width", 1000).toInt(&ok),
                   settings.value("window/height", 700).toInt(&ok));
      CHECK_SETTINGS_CONVERSION("window/width or window/size", "1000, 800 respectively");
      this->move(settings.value("window/xpos", 100).toInt(&ok),
                 settings.value("window/ypos", 100).toInt(&ok));
      CHECK_SETTINGS_CONVERSION("window/xpos or window/ypos", "10, 10 respectively");

#undef CHECK_SETTINGS_CONVERSION
   }
   //-//////////////////
   // log Frame first //
   //-//////////////////

   logFrame = new QFrame(this);   
   logFrame->setFrameStyle(0x1011);
   logFrameLayout = new QGridLayout(logFrame);
   logFrame->setMinimumSize(this->minimumSize().width()/5*4, this->minimumSize().width()/20/3);
   logFrame->setMinimumWidth(this->maximumSize().width()/20/3);

   logDevice = new AF4Log(this);   
   logFrameLayout->addWidget(new QLabel("<b>Log Messages</b>", logFrame), 0, 0, Qt::AlignLeft);
   logFrameLayout->addWidget(logDevice, 1, 0);

   //-////////////////////////
   // Functionality widgets //
   //-////////////////////////

   funcTabWidget = new QTabWidget(this);
   channelCalConfWidget = new AF4ChannelConfigurationWidget(funcTabWidget);
   funcTabWidget->addTab(channelCalConfWidget, "Channel Calibration");
   layout = new QGridLayout(this);
   layout->addWidget(funcTabWidget, 0, 0);
   layout->addWidget(logFrame, 1, 0);
   this->show();
   diffEvaluationWidget = new AF4DiffEvaluationWidget(channelCalConfWidget->getChannelConfigWidgets(),
                                                      channelCalConfWidget->getChannelCalibWidgets(),
                                                      funcTabWidget);
   funcTabWidget->addTab(diffEvaluationWidget, "Diffusion Coefficients");

   slsEvaluationWidget = new AF4SLSEvaluationWidget(funcTabWidget);
   funcTabWidget->addTab(slsEvaluationWidget, "SLS Signal Evaluation");

   this->show();
   QGuiApplication::restoreOverrideCursor();
}

AF4MainWindow::~AF4MainWindow()
{
   writeSettings();
}

void AF4MainWindow::writeSettings() const
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   settings.setValue("window/height", this->height());
   settings.setValue("window/width", this->width());
   settings.setValue("window/xpos", this->pos().x());
   settings.setValue("window/xpos", this->pos().y());
}

