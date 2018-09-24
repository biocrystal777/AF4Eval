#ifndef AF4DATATYPES_H
#define AF4DATATYPES_H

#include <array>
#include <vector>
#include <QVector>

typedef unsigned int uint;
typedef long double lDouble;
typedef std::vector<double> vecD;
typedef std::vector<std::vector<double> > matD;
typedef std::array<double,8> arrD8;


template<typename T>
class QVecMatrix : public QVector<QVector<T> >
{
};

#endif // AF4DATATYPES_H
