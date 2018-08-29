#ifndef FFFMULTIPLEFILEWIDGET_H
#define FFFMULTIPLEFILEWIDGET_H

#include <QList>
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QPushButton>
#include <QToolButton>
#include <QDialog>
#include <QString>
#include "./ffflog.h"
#include "../smallQDerivates/fffnumberedtoolbutton.h"

class FFFDeleteFileDialog : public QDialog
{
   Q_OBJECT
public:
   explicit FFFDeleteFileDialog(QWidget *parent = nullptr);
   ~FFFDeleteFileDialog();

    FFFDeleteFileDialog(const FFFDeleteFileDialog& src) = delete;
    FFFDeleteFileDialog& operator= (FFFDeleteFileDialog& src) = delete;
    FFFDeleteFileDialog(FFFDeleteFileDialog&& src) = delete;
    FFFDeleteFileDialog& operator= (FFFDeleteFileDialog&& src) = delete;

};

class FFFMultipleFileWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FFFMultipleFileWidget(QString identifier, QString description = QString("File"), QWidget *parent = 0);
   ~FFFMultipleFileWidget();

    FFFMultipleFileWidget(const FFFMultipleFileWidget& src) = delete;
    FFFMultipleFileWidget& operator= (FFFMultipleFileWidget& src) = delete;
    FFFMultipleFileWidget(FFFMultipleFileWidget&& src) = delete;
    FFFMultipleFileWidget& operator= (FFFMultipleFileWidget&& src) = delete;

   QList<QString> getFileNames;

   int numberOfPeaks() const;

   void writeSettings();
protected:
   const QString identifier;
   const QString description;

   QVBoxLayout* widgetLayout;
   QScrollArea* scrollArea;
   QWidget *scrolledWidget;
   QVBoxLayout *scrolledLayout;
   QPushButton *widgetAdder;

   QList<QWidget*> *fileWidgets;
   QList<QLabel*> *fileLabels;
   QList<QHBoxLayout*> *fileLayouts;
   QList<QLineEdit*> *fileNames;
   QList<FFFNumberedToolButton*> *chooseFileButtons;
   QList<FFFNumberedToolButton*> *removeFileButtons;

   QString getFilePath(int fileNumber, bool quoted = false);

   bool setFilePath(int fileNumber, QString path, bool quoted = true);

   /*!
    * \brief chopStringsQuotMarksToOne if a string has more than one Quotation mark, they are merged
    * \example """this string"""   =>    "this string"
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksToOne(QString &string) const;

   /*!
    * \brief chopStringsQuotMarksEntirely removes quotation marks of strings
    * \example """this string"""   =>    this string
    * \param string to be processed
    * \return processed string
    */
   QString chopStringsQuotMarksEntirely(QString &string) const;

signals:
   void fileAdded();
   void fileRemoved(int i);

public slots:
   void addFile();
   void removeFile(int fileNumber);
   void chooseFile(int fileNumber);

};

#endif // FFFMULTIPLEFILEWIDGET_H
