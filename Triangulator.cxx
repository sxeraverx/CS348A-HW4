#include "Triangulator.h"

#include "Point.h"

#include <fstream>
#include <sstream>

using namespace std;

extern "C" void copyCoordinatesToGraph(int n, int *x, int *y, int *z, int elimDups, char **gExternal);
extern "C" void delaunay2(char *gExternal);
extern "C" void copyGraphToListOfTriangles(char *gExternal, triangleList **triangles);

int GLOBAL_INTERRUPT_ALGORITHM = 0;

Triangulator::Triangulator(string filename)
{
    graph = newGraph();
    ifstream file(filename.c_str(), ios::in | ios::binary);
    string line;
    while(!file.eof())
    {
        getline(file, line);
        istringstream iss(line);
        string test;
        iss >> test;
        if(test=="site")
        {
            int x, y, z;
            iss >> x >> y >> z;
            xs.push_back(x);
            ys.push_back(y);
            zs.push_back(z);
            points.push_back(Point3<GLfloat>(x/10.0, y/10.0, z/10.0));
        }
    }
    
    copyCoordinatesToGraph(xs.size(), &xs[0], &ys[0], &zs[0], 1, reinterpret_cast<char**>(&graph));
    delaunay2(reinterpret_cast<char *>(graph));
    copyGraphToListOfTriangles(reinterpret_cast<char *>(graph), &trianglesList);
    for(int i = 0; i < trianglesList->nofTriangles; i++)
        triangles.push_back(Vector3<GLuint>(trianglesList->v[i][0],
                                            trianglesList->v[i][1],
                                            trianglesList->v[i][2]));
    recalculateNormals();
}

