#pragma once

#include "Point.h"

template <typename T>
class ControlPoint : public Point3<T>
{
public:
ControlPoint(const Point3<T> &p) : Point3<T>(p) {}
ControlPoint(const Point4<T> &p) : Point3<T>(p) {}
};
