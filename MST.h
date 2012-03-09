#pragma once

#include "Point.h"

#include <vector>
#include <GL/gl.h>

class MST
{
private:
    std::vector<Point3<GLfloat> > points;
    std::vector<std::pair<int, int> > edges;
public:
    MST(const std::vector<Point3<GLfloat> > &points);
    std::vector<Point3<GLfloat> > orderedPoints() const;
    void draw();
};
