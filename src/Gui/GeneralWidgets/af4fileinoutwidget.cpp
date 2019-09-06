#include "af4fileinoutwidget.h"

AF4FileInOutWidget::AF4FileInOutWidget(const QString &identifier, const QString &suffix, QWidget *parent) :
   QFrame(parent), identifier(identifier), suffix(suffix)
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   QString stringValue;

   this->setFrameStyle(0x1011);
   fileLayout = new QGridLayout(this);

   fileLayout->addWidget(new QLabel("<b>Data Input File</b>", this), 0, 1, 1, 3, Qt::AlignBottom);
   inputFileChooser = new QToolButton(this);
   inputFileChooser->setText("..");
   inputFileChooser->setToolTip("Browse Files");
   connect(inputFileChooser, &QToolButton::clicked, this, &AF4FileInOutWidget::chooseInputFile);
   fileLayout->addWidget(inputFileChooser, 1, 0, 1, 1);
   inputFileName = new AF4FilenameLineEdit(this);
   inputFileName->setMinimumWidth(800);
   //inputFileName->setMaximumWidth(800);
   fileLayout->addWidget(inputFileName, 1, 1, 1, 12);
   stringValue = settings.value(tr("fileNames/%1/inputFileName").arg(this->identifier), "").toString();
   setInputFilePath(stringValue, true, true);

   fileLayout->addWidget(new QLabel("<b>Data Output File</b>", this), 2, 1, 1, 3, Qt::AlignBottom);
   outputFileChooser = new QToolButton(this);
   outputFileChooser->setText("..");
   outputFileChooser->setToolTip("Browse Files");
   connect(outputFileChooser, &QToolButton::clicked, this, &AF4FileInOutWidget::chooseOutputFile);
   fileLayout->addWidget(outputFileChooser, 3, 0, 1, 1);
   outputFileName = new AF4FilenameLineEdit(this);
   outputFileName->setMinimumWidth(800);
   //outputFileName->setMaximumWidth(800);
   fileLayout->addWidget(outputFileName, 3, 1, 1, 12);
   stringValue = settings.value(tr("fileNames/%1/outputFileName").arg(identifier), "").toString();
   setOutputFilePath(stringValue, true, true);

   nameGenButton = new QPushButton("gen.", this);
   connect(nameGenButton, &QPushButton::clicked, this, &AF4FileInOutWidget::generateOutputName);
   fileLayout->addWidget(nameGenButton, 3, 13, 1, 1, Qt::AlignLeft);

   autoGenName = new QCheckBox(QString("Autogenerate"), this);
   autoGenName->setChecked(settings.value(tr("fileNames/%1/autoGen").arg(identifier), false).toBool());
   connect(inputFileName, &AF4FilenameLineEdit::textChanged, this, &AF4FileInOutWidget::adoptOutputName);
   fileLayout->addWidget(autoGenName, 3, 14, 1, 3, Qt::AlignLeft);

   fileLayout->setColumnStretch(18, 1);
   fileLayout->setRowStretch(4, 1);


}

AF4FileInOutWidget::~AF4FileInOutWidget()
{
    writeSettings();
}

QString AF4FileInOutWidget::getOutputFilePath(bool quoted)
{
   if(quoted) return outputFileName->singleQuotMarkText();
   else       return outputFileName->noQuotMarkText();
}

QString AF4FileInOutWidget::getInputFilePath(bool quoted)
{
   if(quoted) return inputFileName->singleQuotMarkText();
   else       return inputFileName->noQuotMarkText();
}

bool AF4FileInOutWidget::setInputFilePath(QString path, bool quoted, bool silent)
{
   if(quoted) return inputFileName->setSingleQuotMarkText(path, true, silent);
   else       return inputFileName->setNoQuotMarkText(path, true, silent);
}

void AF4FileInOutWidget::setOutputFilePath(QString path, bool quoted, bool silent)
{
   if(quoted) outputFileName->setSingleQuotMarkText(path, false, silent);
   else       outputFileName->setNoQuotMarkText(path, false, silent);
}

void AF4FileInOutWidget::writeSettings()
{
   QSettings settings("AgCoelfen", "AF4Eval");
   settings.setIniCodec("UTF-8");
   settings.setValue(tr("fileNames/%1/inputFileName").arg(identifier), getInputFilePath(true));
   settings.setValue(tr("fileNames/%1/outputFileName").arg(identifier), getOutputFilePath(true));
   settings.setValue(tr("fileNames/%1/autoGen").arg(identifier), autoGenName->isChecked());
}

void AF4FileInOutWidget::chooseInputFile()
{
   // check for existing input file
   QString oldInputFile = inputFileName->noQuotMarkText();
   if(!QFile::exists(oldInputFile)){
      oldInputFile = QDir::homePath();
   }
   // open dialog in same directory as the old entry and set the file name
   QString s = QFileDialog::getOpenFileName(this, tr("Choose a File to evaluate"), oldInputFile,
                                            QString(), nullptr,
                                            ( QFileDialog::DontConfirmOverwrite |
                                              QFileDialog::ReadOnly |
                                              QFileDialog::HideNameFilterDetails )
                                            );
   setInputFilePath(s, true, false);
}

// check for simplification
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
   outputFileName->singleQuotMarks(s);
   outputFileName->setText(s);
}

void AF4FileInOutWidget::adoptOutputName()
{
   if(autoGenName->isChecked()) generateOutputName();
}

void AF4FileInOutWidget::generateOutputName()
{
   QString outName = inputFileName->noQuotMarkText();
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
   this->setOutputFilePath(outName, true, false);
}
