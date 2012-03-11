#include "MST.h"

#include <GL/glu.h>

#include <vector>

class Path
{
    std::vector<Point3<GLfloat> > deBoorPoints;
    std::vector<Vector3<GLfloat> > deBoorKnots;
    std::vector<Point3<GLfloat> > points;
    GLUnurbsObj *theNurb;
    float mm;
    float dd;
public:
    Path(const MST &mst);
    void draw();
    void m(float m);
    void d(float d);
private:
    void drawCube(Point3<GLfloat> center, GLfloat halfsize);
    void recalcDeBoorPoints();
};
