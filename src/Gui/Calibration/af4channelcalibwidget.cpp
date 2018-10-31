#include "af4channelcalibwidget.h"

AF4ChannelCalibWidget::AF4ChannelCalibWidget(const int channelId,
                                             const int calibId,
                                             const QString channelName,
                                             const QString calibName,
                                             //QWeakPointer<QPushButton> saveButton,
                                             QWidget *parent) :
   QWidget(parent), channelId(channelId), calibId(calibId),
   channelName(channelName), calibName(calibName)//, saveButton(saveButton)
{
   widgetLayout = new QGridLayout(this);
   widgetFrame = new QFrame(this);
   widgetFrame->setFrameStyle(0x0023);
   widgetLayout->addWidget(widgetFrame, 0, 0, 1, 1);
   frameLayout = new QGridLayout(widgetFrame);

   frameLayout->addWidget(new QLabel("<b>Calibration File</b>", this), 0, 1, 1, 3, Qt::AlignBottom);
   inputFileChooser = new QToolButton(this);
   inputFileChooser->setText("..");
   inputFileChooser->setToolTip("Browse Files");
   connect(inputFileChooser, &QToolButton::clicked, this, &AF4ChannelCalibWidget::chooseInputFile);
   frameLayout->addWidget(inputFileChooser, 1, 0, 1, 1, Qt::AlignRight);
   inputFileName = new QLineEdit(this);

   frameLayout->addWidget(inputFileName, 1, 1, 1, 14);
   QLabel *labelPtr = new QLabel(tr("<b>Notes</b>"), this);
   //labelPtr->setMaximumHeight(50);
   frameLayout->addWidget(labelPtr, 2, 1, Qt::AlignCenter);

   frameLayout->addWidget(new QLabel("Date:", this),   3, 0);
   dateDescr = new QLineEdit(this);
   frameLayout->addWidget(dateDescr,                   3, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Sample:", this), 4, 0);
   sampleDescr = new QLineEdit(this);
   frameLayout->addWidget(sampleDescr,                 4, 1, 1, 3);

   frameLayout->addWidget(new QLabel("Buffer:", this), 5, 0);
   bufferDescr = new QLineEdit(this);
   frameLayout->addWidget(bufferDescr,                 5, 1, 1, 3);
   frameLayout->addWidget(new QLabel("Additional Notes:", this), 7, 0, Qt::AlignTop);
   notesDescr = new QTextEdit(this);
   frameLayout->addWidget(notesDescr, 8, 0, 1, 14);

   loadSettings();
   //-
   // Second column
   //--
   innerCalibFrame = new AF4InnerCalibrationFrame(channelId, calibId, channelName, calibName,  this);
   connect(innerCalibFrame, &AF4InnerCalibrationFrame::calibrateChannelCalled, this, &AF4ChannelCalibWidget::calibrateChannelCalled);
   connect(this, &AF4ChannelCalibWidget::saveButtonClicked, innerCalibFrame, &AF4InnerCalibrationFrame::saveButtonClicked);

   frameLayout->addWidget(innerCalibFrame, 3, 5, 3, 4);
   /**************************************
    *
    * third column
    *numberOfCalibrations
    *************************************/
   calibParFrame = new AF4CalibParametersFrame(channelId, calibId, channelName, calibName, this);
   frameLayout->addWidget(calibParFrame, 3, 9, 3, 4);
   connect(this, &AF4ChannelCalibWidget::saveButtonClicked, this, &AF4ChannelCalibWidget::saveParameters);
}

AF4ChannelCalibWidget::~AF4ChannelCalibWidget()
{
   saveParameters();
}


ChannelDimsFromCalib AF4ChannelCalibWidget::getChannelDimsFromCalib() const
{      
   return ChannelDimsFromCalib {
      getClassicalChannelWidth(),
            getHydrodynVolume(),
            getGeometVolume()
   };
}
bool AF4ChannelCalibWidget::setInputFileName(QString path, bool quoted)
{
   QString testPath = path;
   if(testPath.isEmpty()) return true;
   bool ok(true);
   chopStringsQuotMarksEntirely(testPath);

   if(!QFile::exists(testPath)){
      path = QDir::homePath();
      path.prepend('"');
      path.append('"');
      ok = false;
   }
   else {
      if(quoted) chopStringsQuotMarksToOne(path);
      else chopStringsQuotMarksEntirely(path);
      inputFileName->setText(path);
   }
   return ok;
}

ParametersForCalibration AF4ChannelCalibWidget::getParametersForCalibration()
{
   return ParametersForCalibration {
      calibParFrame->getElutionFlow(),
            calibParFrame->getCrossFlow(),
            calibParFrame->getRelFocusPoint(),
            calibParFrame->getVoidPeakTime(),
            calibParFrame->getElutionTime(),
            calibParFrame->getDiffCoefficient()
   };
}

AllCalibrationParameters AF4ChannelCalibWidget::getAllCalibrationParameters()
{   
   return AllCalibrationParameters {
      calibParFrame->getTemperature(),
            calibParFrame->getElutionFlow(),
            calibParFrame->getCrossFlow(),
            calibParFrame->getRelFocusPoint(),
            calibParFrame->getVoidPeakTime(),
            calibParFrame->getElutionTime(),
            calibParFrame->getDiffCoefficient(),
            innerCalibFrame->getChannelWidth(),
            innerCalibFrame->getHydrodynVolume(),
            innerCalibFrame->getGeometVolume(),
            getDateDescr(),
            getSampleDescr(),
            getBufferDescr(),
            getNotesDescr()
   };
}

void AF4ChannelCalibWidget::setAllCalibrationParameters(const AllCalibrationParameters &p)
{
   calibParFrame->setTemperature(p.temperature);
   calibParFrame->setElutionFlow(p.elutionFlow);
   calibParFrame->setCrossFlow(p.crossFlow);
   calibParFrame->setRelFocusPoint(p.relFocusPoint);
   calibParFrame->setVoidPeakTime(p.voidPeakTime);
   calibParFrame->setElutionTime(p.elutionTime);
   calibParFrame->setDiffCoefficient(p.diffCoeff);
   innerCalibFrame->setChannelWidthClassical(p.chWidth);
   innerCalibFrame->setHydrodynVolume(p.hydrodynVolume);
   innerCalibFrame->setGeometVolume(p.geometVolume);
   this->setDateDescr(p.date);
   this->setSampleDescr(p.sample);
   this->setBufferDescr(p.buffer);
   this->setNotesDescr(p.additionalNotes);
}

void AF4ChannelCalibWidget::loadSettings()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   //double calibValue;
   QString calibString;
   //bool ok;
   //settings.setValue(tr("channels/%1/calib/%2/name").arg(channelId).arg(calibId), calibName);
   calibString = settings.value(tr("channels/%1/calib/%2/name").arg(channelId).arg(calibId), "").toString();
   this->setCalibName(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/dateDescr").arg(channelId).arg(calibId), "").toString();
   dateDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/bufferDescr").arg(channelId).arg(calibId), "").toString();
   bufferDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/sampleDescr").arg(channelId).arg(calibId), "").toString();
   sampleDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/notesDescr").arg(channelId).arg(calibId), "").toString();
   notesDescr->setText(calibString);
   calibString = settings.value(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), "").toString();
   if(!(this->setInputFileName(calibString))) AF4Log::logWarning(tr("Error while setting calib Value \"calib FileName\" in Constructor."));
}

void AF4ChannelCalibWidget::saveParameters()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("channels/%1/calib/%2/name").arg(channelId).arg(calibId), calibName);
   settings.setValue(tr("channels/%1/calib/%2/sampleDescr").arg(channelId).arg(calibId), QVariant(getSampleDescr()));
   settings.setValue(tr("channels/%1/calib/%2/bufferDescr").arg(channelId).arg(calibId), QVariant(getBufferDescr()));
   settings.setValue(tr("channels/%1/calib/%2/dateDescr").arg(channelId).arg(calibId), QVariant(getDateDescr()));
   settings.setValue(tr("channels/%1/calib/%2/notesDescr").arg(channelId).arg(calibId), QVariant(getNotesDescr()));
   settings.setValue(tr("channels/%1/calib/%2/calibFileName").arg(channelId).arg(calibId), QVariant(this->getInputFilePath(false)));
}

QString AF4ChannelCalibWidget::getInputFilePath(bool quoted)
{
   QString path = inputFileName->text();
   if(quoted)
      chopStringsQuotMarksToOne(path);
   else
      chopStringsQuotMarksEntirely(path);
   return path;
}

void AF4ChannelCalibWidget::chooseInputFile()
{
   QString s;
   QString oldInputFile = inputFileName->text();
   chopStringsQuotMarksEntirely(oldInputFile);
   if(!QFile::exists(oldInputFile)){
      oldInputFile = QDir::homePath();
   }
   s = QFileDialog::getOpenFileName(this, tr("Choose a File to evaluate"), oldInputFile,
                                    QString(), nullptr,
                                    ( QFileDialog::DontConfirmOverwrite |
                                      QFileDialog::ReadOnly |
                                      QFileDialog::HideNameFilterDetails )
                                    );
   if(QFile::exists(s)) inputFileName->setText(chopStringsQuotMarksToOne(s));
   else AF4Log::logWarning(tr("Chosen input file does not exist."));
}

QString AF4ChannelCalibWidget::chopStringsQuotMarksToOne(QString & string) const
{
   chopStringsQuotMarksEntirely(string);
   string.prepend('\"');
   string.append('\"');
   return string;
}

QString AF4ChannelCalibWidget::chopStringsQuotMarksEntirely(QString & string) const
{
   const QChar quotMark('\"');
   while(!string.isEmpty() && string.at(0) == quotMark)
      string.remove(0, 1);
   while(!string.isEmpty() && string.at(string.length()-1) == quotMark)
      string.remove(string.length()-1, 1);

   return string;
}
