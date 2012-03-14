#pragma once

#include <GL/glu.h>

#include <vector>

#include "QuadraticSegment.h"

class Datafile;
class QuadraticSegment;

class Path
{
    std::vector<Point3<GLfloat> > deBoorPoints;
    std::vector<Vector3<GLfloat> > deBoorKnots;
    std::vector<Vector3<GLfloat> > normalsBelow;
    GLUnurbsObj *theNurb;
    float dd;
    const Datafile *mesh;
public:
    std::vector<QuadraticSegment> quadsegs;
public:
    std::vector<Point3<GLfloat> > points;
    Path(const std::vector<Point3<GLfloat> > &points, const Datafile *mesh);
    void draw();
    void d(float d);
    GLfloat curvature() const;
    GLfloat length() const;
    bool segmentHasPOI(GLfloat time) const;
    Point3<GLfloat> evaluate(GLfloat time) const;
    int duration() const;
    GLfloat distance() const;
    void recalcDeBoorPoints();
private:
    void drawCube(Point3<GLfloat> center, GLfloat halfsize);
    std::vector<QuadraticSegment> generatePOIQuadSegs();
    std::vector<QuadraticSegment> mergeQuadSegs(std::vector<QuadraticSegment> quadsegs);
    void updateTrianglesBelow();
    bool intersectsTerrain(QuadraticSegment qs);
};
