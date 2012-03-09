#pragma once

#include <algorithm>
#include <ostream>
#include <iostream>

#include "Vector.h"

template <typename T>
struct Point4;

template <typename T>
struct Point3
{
    T vec[3];

    Point3() : vec() {vec[0] = 0; vec[1] = 0; vec[2] = 0;}

    Point3(T x, T y, T z) { vec[0] = x;
                            vec[1] = y;
                            vec[2] = z; }
    Point3(const T *vec) { std::copy(vec, vec+3, Point3::vec); }
    Point3(const Point4<T> &p);
    operator T*() { return vec; }
    operator const T*() const { return vec; }
    Vector3<T> operator-(const Point3<T> &p) const { Vector3<T> ret(vec);
                                                     ret[0]-=p[0];
                                                     ret[1]-=p[1];
                                                     ret[2]-=p[2];
                                                     return ret;}
    Point3<T> operator+=(const Vector3<T> &v) { vec[0]+=v[0];
                                                vec[1]+=v[1];
                                                vec[2]+=v[2]; }
    Point3<T> operator-=(const Vector3<T> &v) { vec[0]-=v[0];
                                                vec[1]-=v[1];
                                                vec[2]-=v[2]; }
    Point3<T> operator+(const Vector3<T> &v) const { Point3<T> ret(vec);
                                                     ret+=v;
                                                     return ret;}
    Point3<T> operator-(const Vector3<T> &v) const { Point3<T> ret(vec);
                                                     ret-=v;
                                                     return ret;}
    bool operator==(const Point3<T> &p) const { return (*this-p).magnitude()==0.0f; }
};

template <typename T>
struct Point4
{
    T vec[4];

    Point4() : vec() {vec[0] = 0; vec[1] = 0; vec[2] = 0; vec[3] = 1;}

    Point4(T x, T y, T z) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = 1; }
    Point4(T x, T y, T z, T w) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = w; }
    Point4(Point3<T> p) { std::copy(p.vec, p.vec+3, vec); vec[3] = 1; }
    Point4(T *vec) { std::copy(vec, vec+4, Point4::vec); }
    operator T*() { return vec; }
    operator const T*() const { return vec; }
    bool operator==(const Point4<T> &other) const {return Point3<T>(*this)==Point3<T>(other); }
};

template <typename T>
Point3<T>::Point3(const Point4<T> &p) { for(int i = 0; i < 3; i++) vec[i] = p.vec[i]/p.vec[3]; }

template <typename T>
std::ostream &operator << (std::ostream &out, const Point3<T> &p)
{
    out << "P3: " << p.vec[0] << " " << p.vec[1] << " " << p.vec[2];
}

template <typename T>
std::ostream &operator << (std::ostream &out, const Point4<T> &p)
{
    out << "P4: " << p.vec[0] << " " << p.vec[1] << " " << p.vec[2] << " (" << p.vec[3] << ")";
}
