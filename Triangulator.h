#pragma once

#include "Datafile.h"
#include "Point.h"

#include <string>
#include <vector>

#include "gl/GL.h"

extern "C" {
#include "minmaxer/source.triangulation/geometry.objects.h"
#include "minmaxer/source.triangulation/graph.h"
#include "minmaxer/source.triangulation/triangulation.h"
}

class Datafile;

class Triangulator : public Datafile
{
    graphType *graph;
    std::vector<int> xs, ys, zs;
    triangleList *trianglesList;
public:
    Triangulator(std::string filename);
};
