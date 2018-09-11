#include "af4fileinoutwidget.h"

AF4FileInOutWidget::AF4FileInOutWidget(const QString &identifier, const QString &suffix, QWidget *parent) :
   QFrame(parent), identifier(identifier), suffix(suffix)
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   QString stringValue;

   this->setFrameStyle(0x1011);
   fileLayout = new QGridLayout(this);
   fileLayout->addWidget(new QLabel("<b>Data Input File</b>", this), 0, 1, 1, 3, Qt::AlignBottom);
   inputFileChooser = new QToolButton(this);
   inputFileChooser->setText("..");
   inputFileChooser->setToolTip("Browse Files");
   QObject::connect(inputFileChooser, SIGNAL(clicked()), this, SLOT(chooseInputFile()));
   fileLayout->addWidget(inputFileChooser, 1, 0, 1, 1);
   inputFileName = new QLineEdit(this);
   inputFileName->setMinimumWidth(300);
   inputFileName->setMaximumWidth(800);
   fileLayout->addWidget(inputFileName, 1, 1, 1, 12);
   stringValue = settings.value(tr("fileNames/%1/inputFileName").arg(this->identifier), "").toString();
   setInputFilePath(stringValue, true);


   fileLayout->addWidget(new QLabel("<b>Data Output File</b>", this), 2, 1, 1, 3, Qt::AlignBottom);
   outputFileChooser = new QToolButton(this);
   outputFileChooser->setText("..");
   outputFileChooser->setToolTip("Browse Files");
   QObject::connect(outputFileChooser, SIGNAL(clicked()), this, SLOT(chooseOutputFile()));
   fileLayout->addWidget(outputFileChooser, 3, 0, 1, 1);
   outputFileName = new QLineEdit(this);
   outputFileName->setMinimumWidth(300);
   outputFileName->setMaximumWidth(800);
   fileLayout->addWidget(outputFileName, 3, 1, 1, 12);
   stringValue = settings.value(tr("fileNames/%1/outputFileName").arg(identifier), "").toString();
   setOutputFilePath(stringValue, true);

   nameGenButton = new QPushButton("имя", this);
   QObject::connect(nameGenButton, SIGNAL(clicked()), this, SLOT(autoGenOutputName()));
   fileLayout->addWidget(nameGenButton, 3, 13, 1, 1, Qt::AlignLeft);

   autoGenName = new QCheckBox(QString("Autogenerate"), this);
   autoGenName->setChecked(settings.value(tr("fileNames/%1/autoGen").arg(identifier), false).toBool());
   QObject::connect(inputFileName, SIGNAL(textChanged(QString)), this, SLOT(adoptOutputName()));
   fileLayout->addWidget(autoGenName, 3, 14, 1, 3, Qt::AlignLeft);
}

AF4FileInOutWidget::~AF4FileInOutWidget()
{
    writeSettings();
}


QString AF4FileInOutWidget::getOutputFilePath(bool quoted)
{
      QString path = outputFileName->text();
      if(quoted)
         chopStringsQuotMarksToOne(path);
      else
         chopStringsQuotMarksEntirely(path);
      return path;
}

QString AF4FileInOutWidget::getInputFilePath(bool quoted)
{
   QString path = inputFileName->text();
   if(quoted)
      chopStringsQuotMarksToOne(path);
   else
      chopStringsQuotMarksEntirely(path);
   return path;
}

bool AF4FileInOutWidget::setInputFilePath(QString path, bool quoted)
{
   bool ok(true);
   QString testPath = path;
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

void AF4FileInOutWidget::setOutputFilePath(QString path, bool quoted)
{
   if(quoted) chopStringsQuotMarksToOne(path);
   else chopStringsQuotMarksEntirely(path);
   outputFileName->setText(path);
}

void AF4FileInOutWidget::writeSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("fileNames/%1/inputFileName").arg(identifier), getInputFilePath(true));
   settings.setValue(tr("fileNames/%1/outputFileName").arg(identifier), getOutputFilePath(true));
   settings.setValue(tr("fileNames/%1/autoGen").arg(identifier), autoGenName->isChecked());
}

void AF4FileInOutWidget::chooseInputFile()
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

void AF4FileInOutWidget::chooseOutputFile()
{
   QString oldOutputFile = outputFileName->text();

   if(!QFile::exists(oldOutputFile) || QFileInfo( oldOutputFile ).isDir() ) oldOutputFile = QFileInfo( inputFileName->text() ).path();
   QFileDialog dialog(this, tr("Choose an file Name for your results,"), oldOutputFile);
   dialog.setFileMode(QFileDialog::AnyFile);
   dialog.setOption(QFileDialog::DontConfirmOverwrite, false);
   QObject::connect(&dialog, SIGNAL(fileSelected(QString)), outputFileName, SLOT(setText(QString)));
   dialog.exec();
   QString s = outputFileName->text();
   chopStringsQuotMarksToOne(s);
   outputFileName->setText(s);
}

void AF4FileInOutWidget::adoptOutputName()
{
   if(autoGenName->isChecked()) autoGenOutputName();
}

void AF4FileInOutWidget::autoGenOutputName()
{
   QString outName = inputFileName->text();
   chopStringsQuotMarksEntirely(outName);
   QStringList l = outName.split(".", QString::SkipEmptyParts);

   if(l.empty())
      return;
   else if(l.size() == 1 )
      outName.append(suffix);
   else if (l.size() == 2)
      outName = l.first().append(suffix).append(".").append(l.last());
   else {
      outName.clear();
      for(int i = 0; i < l.size() - 2; ++i) outName.append(l[i]).append(".");
      outName.append(l[l.size() - 2]);
      outName.append(suffix).append(".").append(l.last());
   }
   this->setOutputFilePath(outName, true);
}

QString AF4FileInOutWidget::chopStringsQuotMarksToOne(QString & string) const
{
   chopStringsQuotMarksEntirely(string);
   string.prepend('\"');
   string.append('\"');
   return string;
}

QString AF4FileInOutWidget::chopStringsQuotMarksEntirely(QString & string) const
{
   const QChar quotMark('\"');
   while(!string.isEmpty() && string.at(0) == quotMark)
      string.remove(0, 1);
   while(!string.isEmpty() && string.at(string.length()-1) == quotMark)
      string.remove(string.length()-1, 1);

   return string;
}
