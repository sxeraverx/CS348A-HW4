#include "Tour.h"

#include "MST.h"
#include "Path.h"

#include <fstream>
#include <sstream>

using namespace std;

Tour::Tour(string filename)
    : points()
{
    ifstream file(filename.c_str(), ios::in | ios::binary);
    string line;
    while(!file.eof())
    {
        getline(file, line);
        istringstream iss(line);
        GLfloat x, y, z;
        iss >> x >> y >> z;
        points.push_back(Point4<GLfloat>(x/1000.0, y/1000.0, z/1000.0));
    }
    mst = new MST(points);
    path = new Path(*mst);
}

Tour::~Tour()
{
    delete path;
    delete mst;
}

void Tour::draw()
{
    mst->draw();
    path->draw();
}


void Tour::d(float d)
{
    path->d(d);
}

void Tour::m(float m)
{
    path->m(m);
}
