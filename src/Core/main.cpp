#include <QDebug>
#include <QGuiApplication>
#include "../Gui/af4mainwindow.h"
#include "./omp_qvector.h"

int main(int argc, char *argv[])
{
   qRegisterMetaType<omp_QVector<bool> >("omp_QVector<bool>");
   QApplication a(argc, argv);
   AF4MainWindow w;
   w.show();

   return a.exec();
}
