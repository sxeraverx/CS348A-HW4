#pragma once

#include "Point.h"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <GL/gl.h>

class Datafile
{
protected:
    Datafile();
public:
    std::vector<Point4<GLfloat> > points;
    std::set<std::pair<int,int> > edges;
    mutable
    std::vector<Vector3<GLuint> > triangles;
    std::vector<Vector3<GLfloat> > normals;
    Datafile(std::string path);

    void draw();
    void drawTrianglesBelow(std::vector<Point3<GLfloat> > ps);

    GLfloat distance(Point3<GLfloat> p) const;
    std::vector<Point3<GLfloat> > triangleBelow(Point3<GLfloat> p, bool alreadyTried = false) const;

protected:
    void rebuildTriangles();
    void recalculateNormals();
    void splitTriangles();
};
