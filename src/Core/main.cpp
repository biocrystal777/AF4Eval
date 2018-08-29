#include <QDebug>
#include <QGuiApplication>
#include "../Gui/fffmainwindow.h"
#include "./omp_qvector.h"

int main(int argc, char *argv[])
{
   qRegisterMetaType<omp_QVector<bool> >("omp_QVector<bool>");
   QApplication a(argc, argv);
   FFFMainWindow w;
   w.show();

   return a.exec();
}
