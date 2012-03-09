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
    std::vector<Vector3<GLuint> > triangles;
    std::vector<Vector3<GLfloat> > normals;
    Datafile(std::string path);

    void draw();

protected:
    void rebuildTriangles();
    void recalculateNormals();
    void splitTriangles();
};
