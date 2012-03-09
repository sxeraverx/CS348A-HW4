#pragma once

#include "Point.h"

#include <string>
#include <vector>

#include <GL/gl.h>

class MST;
class Path;

class Tour
{
public:
    MST *mst;
    Path *path;
    std::vector<Point3<GLfloat> > points;
public:
    Tour(std::string filename);
    ~Tour();
    void draw();
    void m(float m);
};
