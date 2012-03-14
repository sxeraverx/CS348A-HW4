#include "Tour.h"

#include "Datafile.h"
#include "MST.h"
#include "Path.h"

#include <fstream>
#include <sstream>

using namespace std;

Tour::Tour(string filename, Datafile *mesh)
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
        if(file.eof())
            break;
        points.push_back(Point4<GLfloat>(x/10.0, y/10.0, z/10.0));
    }
    mst = new MST(points);
    path = new Path(*mst, mesh);
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

int Tour::length() const
{
    return path->length();
}

Point3<GLfloat> Tour::evaluate(GLfloat t) const
{
    return path->evaluate(t);
}
