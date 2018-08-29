#include "fffmultiplefilewidget.h"

FFFMultipleFileWidget::FFFMultipleFileWidget(QString identifier, QString description, QWidget *parent) :
   QWidget(parent), identifier(identifier), description(description)
{

   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");
   bool ok;
   int numberOfPeaks = settings.value(tr("/fileNames/%1/numberOfFiles").arg(this->identifier), 1).toInt(&ok);

   if(!ok) FFFLog::logWarning(tr("Could not read number Of Peaks."));

   widgetLayout = new QVBoxLayout(this);


   scrollArea = new QScrollArea(this);
   scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   scrollArea->setMinimumWidth(720);
   scrollArea->setMinimumHeight(250);
   scrollArea->setMaximumHeight(250);

   widgetLayout->addWidget(scrollArea, 0, Qt::AlignCenter);
   scrolledWidget = new QWidget(scrollArea);
   scrollArea->setWidget(scrolledWidget);
   scrollArea->setWidgetResizable(true);



   scrolledLayout = new QVBoxLayout(scrolledWidget);
   scrolledLayout->addWidget(new QLabel(tr("<b>%1 Files</b>").arg(description), this));

   fileWidgets = new QList<QWidget*>();
   fileLayouts = new QList<QHBoxLayout*>();
   chooseFileButtons = new QList<FFFNumberedToolButton*>();
   removeFileButtons = new QList<FFFNumberedToolButton*>();
   fileNames = new QList<QLineEdit*>();
   fileLabels = new QList<QLabel*>();

   QString stringValue;
   widgetAdder = new QPushButton(tr("Add %1").arg(description), scrolledWidget);
   widgetAdder->setMinimumHeight(25);
   widgetAdder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

   QObject::connect(widgetAdder, SIGNAL(clicked()), this, SLOT(addFile()));

   scrolledLayout->addWidget(widgetAdder, 0, Qt::AlignCenter);

   for(int i = 0; i < numberOfPeaks; ++i){
      addFile();
      stringValue = settings.value(tr("/fileNames/%1/no%2").arg(this->identifier).arg(i), "").toString();
      setFilePath(i, stringValue, true);
   }

   emit fileAdded();
}

FFFMultipleFileWidget::~FFFMultipleFileWidget()
{
   writeSettings();
   delete chooseFileButtons;
   delete removeFileButtons;
   delete fileNames;
   delete fileWidgets;
   delete fileLayouts;
}

int FFFMultipleFileWidget::numberOfPeaks() const
{
   return fileWidgets->size();
}

void FFFMultipleFileWidget::writeSettings()
{
   QSettings settings("AgCoelfen", "FFFEval");
   settings.setIniCodec("UTF-8");

   settings.setValue(tr("/fileNames/%1/numberOfFiles").arg(this->identifier), fileNames->size());
   for(int i = 0 ; i < fileNames->size(); ++i)
      settings.setValue(tr("/fileNames/%1/no%2").arg(this->identifier).arg(i), getFilePath(i, true));

}

QString FFFMultipleFileWidget::getFilePath(int fileNumber, bool quoted)
{
   QString path = fileNames->at(fileNumber)->text();
   if(quoted)
      chopStringsQuotMarksToOne(path);
   else
      chopStringsQuotMarksEntirely(path);
   return path;
}

bool FFFMultipleFileWidget::setFilePath(int fileNumber, QString path, bool quoted)
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
      fileNames->at(fileNumber)->setText(path);
   }

   return ok;
}

void FFFMultipleFileWidget::addFile()
{
   int i = fileNames->size();


   fileWidgets->append(new QWidget(scrolledWidget));

   fileLayouts->append(new QHBoxLayout(fileWidgets->at(i)));


   chooseFileButtons->append(new FFFNumberedToolButton(i, scrolledWidget));
   chooseFileButtons->at(i)->setText("..");
   chooseFileButtons->at(i)->setToolTip("Browse Files");
   chooseFileButtons->at(i)->setMinimumHeight(25);
   QObject::connect(chooseFileButtons->at(i), SIGNAL(clickedID(int)), this, SLOT(chooseFile(int)));

   fileNames->append(new QLineEdit(fileWidgets->at(i)));
   fileNames->at(i)->setMinimumWidth(560);
   fileNames->at(i)->setMaximumWidth(720);
   fileNames->at(i)->setMinimumHeight(25);

   removeFileButtons->append(new FFFNumberedToolButton(i, fileWidgets->at(i)));
   removeFileButtons->at(i)->setText("-");
   removeFileButtons->at(i)->setToolTip("Remove this File");
   QObject::connect(removeFileButtons->at(i), SIGNAL(clickedID(int)), this, SLOT(removeFile(int)));
   removeFileButtons->at(i)->setMinimumHeight(25);

   fileLabels->append(new QLabel(tr("%1 %2:").arg(description).arg(i+1), fileWidgets->at(i)));

   fileLayouts->at(i)->addWidget(fileLabels->at(i), 1, Qt::AlignRight);
   fileLayouts->at(i)->addWidget(chooseFileButtons->at(i));
   fileLayouts->at(i)->addWidget(fileNames->at(i));
   fileLayouts->at(i)->addWidget(removeFileButtons->at(i));
   scrolledLayout->insertWidget(i + 1, fileWidgets->at(i), 0, Qt::AlignCenter);



   emit fileAdded();
}

void FFFMultipleFileWidget::chooseFile(int fileNumber)
{
   QString s;
   QString oldInputFile = fileNames->at(fileNumber)->text();
   if(!QFile::exists(oldInputFile)) oldInputFile = QDir::homePath();
   s = QFileDialog::getOpenFileName(this, tr("Choose a File to evaluate"), oldInputFile,
                                    QString(), nullptr,
                                    ( QFileDialog::DontConfirmOverwrite |
                                      QFileDialog::ReadOnly |
                                      QFileDialog::HideNameFilterDetails )
                                    );
   if(QFile::exists(s)) fileNames->at(fileNumber)->setText(chopStringsQuotMarksToOne(s));
   else FFFLog::logWarning(tr("Chosen input file does not exist."));

}

void FFFMultipleFileWidget::removeFile(int fileNumber)
{
   if(fileWidgets->size() <= 1){
      FFFLog::logWarning(tr("You cannot remove the last file."));
      return;
   }
   delete fileWidgets->at(fileNumber);
   fileWidgets->removeAt(fileNumber);
   fileLayouts->removeAt(fileNumber);
   fileNames->removeAt(fileNumber);
   chooseFileButtons->removeAt(fileNumber);
   removeFileButtons->removeAt(fileNumber);
   fileLabels->removeAt(fileNumber);
   for(int i = fileNumber; i < fileWidgets->size(); ++i){
      fileLabels->at(i)->setText(tr("%1 %2").arg(description).arg(i+1));
      chooseFileButtons->at(i)->changeIdTo(i);
      removeFileButtons->at(i)->changeIdTo(i);
   }
   emit fileRemoved(fileNumber);
}

QString FFFMultipleFileWidget::chopStringsQuotMarksToOne(QString & string) const
{
   chopStringsQuotMarksEntirely(string);
   string.prepend('\"');
   string.append('\"');
   return string;
}

QString FFFMultipleFileWidget::chopStringsQuotMarksEntirely(QString & string) const
{
   const QChar quotMark('\"');
   while(!string.isEmpty() && string.at(0) == quotMark)
      string.remove(0, 1);
   while(!string.isEmpty() && string.at(string.length()-1) == quotMark)
      string.remove(string.length()-1, 1);

   return string;
}


FFFDeleteFileDialog::~FFFDeleteFileDialog()
{
}


FFFDeleteFileDialog::FFFDeleteFileDialog(QWidget *parent) :
   QDialog(parent)
{
}


