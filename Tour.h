#pragma once
#include "Point.h"

#include <string>
#include <vector>

#include <GL/gl.h>

class Datafile;
class MST;
class Path;

class Tour
{
public:
    MST *mst;
    Path *path;
    std::vector<Point3<GLfloat> > points;
public:
    Tour(std::string filename, Datafile *mesh);
    ~Tour();
    void draw();
    void d(float d);
    int length() const;
    Point3<GLfloat> evaluate(GLfloat t) const;
};
