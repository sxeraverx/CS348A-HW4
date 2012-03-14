#pragma once

#include "Point.h"
#include "Vector.h"

#include <GL/gl.h>

class Line
{
public:
    Point3<GLfloat> p;
    Vector3<GLfloat> d;
    
    GLfloat distance(const Point3<GLfloat> &op) const;
    Point3<GLfloat> intersect(const Line &ol) const;
};

class Plane
{
private:
    GLfloat signedDistance(const Point3<GLfloat> &otherp) const;
public:
    Point3<GLfloat> p;
    Vector3<GLfloat> n;

    Line intersect(const Plane &other) const;
    Point3<GLfloat> intersect(const Line &other) const;

    GLfloat distance(const Point3<GLfloat> &otherp) const;

    Point3<GLfloat> project(const Point3<GLfloat> &otherp) const;

    void draw();
};
