#pragma once

#include <GL/glu.h>

#include <vector>

#include "QuadraticSegment.h"

class Datafile;
class MST;
class QuadraticSegment;

class Path
{
    std::vector<Point3<GLfloat> > deBoorPoints;
    std::vector<Vector3<GLfloat> > deBoorKnots;
    std::vector<Point3<GLfloat> > points;
    std::vector<Vector3<GLfloat> > normalsBelow;
    GLUnurbsObj *theNurb;
    float mm;
    float dd;
    const Datafile *mesh;
    std::vector<QuadraticSegment> quadsegs;
public:
    Path(const MST &mst, const Datafile *mesh);
    void draw();
    void m(float m);
    void d(float d);
private:
    void drawCube(Point3<GLfloat> center, GLfloat halfsize);
    void recalcDeBoorPoints();
    std::vector<QuadraticSegment> generatePOIQuadSegs();
    std::vector<QuadraticSegment> mergeQuadSegs(std::vector<QuadraticSegment> quadsegs);
    void updateTrianglesBelow();
    bool intersectsTerrain(QuadraticSegment qs);
};
