#pragma once

#include "Point.h"
#include "Vector.h"

#include <GL/gl.h>

class Line
{
public:
    Point3<GLfloat> p;
    Vector3<GLfloat> d;
    Line() : p(), d() {}
    Line(Point3<GLfloat> p, Vector3<GLfloat> d) : p(p), d(d.normalized()) {}
    Line(Point3<GLfloat> p1, Point3<GLfloat> p2) : p(p1), d((p2-p1).normalized()) {}
    
    GLfloat distance(const Point3<GLfloat> &op) const;
    Point3<GLfloat> intersect(const Line &ol) const;
    Point3<GLfloat> project(const Point3<GLfloat> &opt) const;
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
