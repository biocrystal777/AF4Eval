#ifndef FFFDATATYPES_H
#define FFFDATATYPES_H

#include <vector>
#include <QVector>

typedef long double lDouble;
typedef std::vector<double> vecD;
typedef std::vector<std::vector<double> > matD;

template<typename T>
class QVecMatrix : public QVector<QVector<T> >
{
};

#endif // FFFDATATYPES_H
