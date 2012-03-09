#pragma once

#include <algorithm>
#include <ostream>
#include <iostream>
#include <cmath>

template <typename T>
struct Vector3
{
    T vec[3];
    
    Vector3() : vec() {vec[0] = 0; vec[1] = 0; vec[2] = 0;}

    Vector3(T x, T y, T z) { vec[0] = x; vec[1] = y; vec[2] = z; }
    Vector3(const T *vec) { std::copy(vec, vec+3, Vector3::vec); }
    operator T*() { return vec; }
    operator const T*() const { return vec; }
    T &operator[](int idx) { return vec[idx]; }
    T operator[](int idx) const { return vec[idx]; }

    Vector3<T> &operator+=(const Vector3<T> &v) { vec[0]+=v[0];
                                                  vec[1]+=v[1];
                                                  vec[2]+=v[2];
                                                  return *this; }
    Vector3<T> &operator-=(const Vector3<T> &v) { vec[0]-=v[0];
                                                  vec[1]-=v[1];
                                                  vec[2]-=v[2];
                                                  return *this; }
    Vector3<T> &operator*=(const T &i) { vec[0]*=i;
                                         vec[1]*=i;
                                         vec[2]*=i;
                                         return *this; }
    Vector3<T> &operator/=(const T &i) { vec[0]/=i;
                                         vec[1]/=i;
                                         vec[2]/=i;
                                         return *this; }
    T operator*(const Vector3<T> &v) const { return vec[0]*v[0]
                                                  + vec[1]*v[1]
                                                  + vec[2]*v[2]; }
    Vector3<T> operator*(const T &i) const { Vector3<T> ret(vec); return ret*=i;}
    Vector3<T> operator/(const T &i) const { Vector3<T> ret(vec); return ret/=i;}
    Vector3<T> operator+(const Vector3<T> &v) const { Vector3<T> ret(vec); return ret+=v;}
    Vector3<T> operator-(const Vector3<T> &v) const { Vector3<T> ret(vec); return ret-=v;}

    Vector3<T> cross(const Vector3<T> &v) const {
        Vector3<T> ret(0,0,0);
        ret[0] = vec[1]*v[2]-v[1]*vec[2];
        ret[1] = vec[2]*v[0]-v[2]*vec[0];
        ret[2] = vec[0]*v[1]-v[0]*vec[1];
        return ret;
    }

    T magnitude();

    Vector3<T> &normalize() {
        return (*this)/=magnitude();
    }

    Vector3<T> normalized() const {
        Vector3<T> ret(vec);
        ret.normalize();
        return ret;
    }
};

template <>
inline
float Vector3<float>::magnitude()
{
    return sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}


template <typename T>
std::ostream &operator << (std::ostream &out, const Vector3<T> &v)
{
    out << "V3: " << v.vec[0] << " " << v.vec[1] << " " << v.vec[2];
}
