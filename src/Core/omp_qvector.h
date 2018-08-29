#ifndef OMP_QVECTOR_H
#define OMP_QVECTOR_H

#include <QVector>
#include "omp.h"
#include "./vector"

template<typename T>
class omp_QVector : private QVector<T>
{

public:
   inline omp_QVector() : QVector<T>() { omp_init_lock(&lock); }
   inline explicit omp_QVector(int size) : QVector<T>(size) { omp_init_lock(&lock); }
   inline omp_QVector(int size, const T &value) : QVector<T>(size, value) { omp_init_lock(&lock); }
   inline omp_QVector(const omp_QVector<T> &other) : QVector<T>(other), lock(other.lock) {}
   inline omp_QVector(omp_QVector<T> && other) : QVector<T>(other), lock(other.lock) {qDebug() << "called move vec";}
   inline omp_QVector(std::initializer_list<T> args) : QVector<T>(args) { omp_init_lock(&lock); }
   virtual ~omp_QVector(){}

   omp_QVector<T> operator=( omp_QVector<T> &&v){      
      omp_QVector<T> tmp = v;
      tmp.swap(*this);      
      return *this;
   }

   inline omp_QVector<T> operator=(const omp_QVector<T> &other){
      set_lock();
      omp_QVector<T> t(other);
      unset_lock();
      return t;
   }

   inline omp_QVector(const QVector<T> &other) : QVector<T>(other) { omp_init_lock(&lock); }
   omp_QVector<T> &operator=(const QVector<T> &v){
      set_lock();
      omp_QVector<T> tmp(v);
      tmp.swap(*this);
      unset_lock();
      return *this;
   }

   inline void moveAsQVector( QVector<T> &destiny){
      set_lock();
      destiny.swap(*this);
      unset_lock();
   }

   static inline omp_QVector<T> omp_fromStdVector(const std::vector<T>& vector){
      omp_QVector<T> qVec(QVector<T>::fromStdVector(vector));
      return qVec;
   }

   inline void omp_append(const T &value){
      set_lock();
      this->append(value);
      unset_lock();
   }

   inline const T & omp_at(int i) ;
   inline void setAt(int i, const T &value);
   inline T &operator[](int i) ;



   inline void omp_resize(int size){
      set_lock();
      this->resize(size);
      unset_lock();
   }

   inline void omp_reserve(int size){
      set_lock();
      this->reserve(size);
      unset_lock();
   }

   inline void omp_push_back(const T & value){
      set_lock();
      this->push_back(value);
      unset_lock();
   }

   inline void omp_push_front(const T & value){
      set_lock();
      this->push_front(value);
      unset_lock();
   }

   inline int omp_size() const { return this->size(); }

protected:
   inline void set_lock(){ omp_set_lock(&lock); }
   inline void unset_lock(){ omp_unset_lock(&lock); }

private:
      omp_lock_t lock;

};

template <typename T>
inline const T &omp_QVector<T>::omp_at(int i)
{
   set_lock();
   Q_ASSERT_X(i >= 0 && i < this->size(), "QVector<T>::at", "index out of range");
   const T t = this->at(i);
   unset_lock();
   return t;
}

template <typename T>
inline T &omp_QVector<T>::operator[](int i)
{
   set_lock();
   Q_ASSERT_X(i >= 0 && i < this->size(), "QVector<T>::operator[]", "index out of range");
   T& val = const_cast<T&>(this->at(i));
   unset_lock();
   return val;
}

template <typename T>
inline void omp_QVector<T>::setAt(int i, const T &value){
   set_lock();
   const_cast<T&>(this->at(i)) = value;
   unset_lock();
}

#endif // OMP_QVECTOR_H

