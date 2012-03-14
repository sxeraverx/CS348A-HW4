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
    void m(float m);
    void d(float d);
};
