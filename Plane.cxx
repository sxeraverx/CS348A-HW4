#include "Plane.h"

GLfloat Line::distance(const Point3<GLfloat> &op) const
{
    return (p-op).cross(d.normalized()).magnitude();
}

Point3<GLfloat> Line::intersect(const Line &ol) const
{
    Plane p1;
    p1.p = p;
    p1.n = d.cross(ol.d).cross(d);
    return p1.intersect(ol);
}

Line Plane::intersect(const Plane &other) const
{
    Point3<GLfloat> origin(0,0,0);
    Line l;
    Vector3<GLfloat> n1 = n.normalized();
    Vector3<GLfloat> n2 = other.n.normalized();
    GLfloat h1 = (p-origin)*n1;
    GLfloat h2 = (other.p-origin)*n2;
    GLfloat c1 = (h1-h2*(n1*n2))/(1-(n1*n2)*(n1*n2));
    GLfloat c2 = (h2-h1*(n1*n2))/(1-(n1*n2)*(n1*n2));
    l.p = origin+(c1*n1+c2*n2);
    l.d = n1.cross(n2);
    return l;
}

Point3<GLfloat> Plane::intersect(const Line &l) const
{
    GLfloat d = (p-l.p)*n.normalized();
    return l.p+l.d.normalized()*d;
}

GLfloat Plane::signedDistance(const Point3<GLfloat> &otherp) const
{
    GLfloat dist = n*(otherp-p);
    return dist;
}

GLfloat Plane::distance(const Point3<GLfloat> &otherp) const
{
    GLfloat dist = signedDistance(p);
    if(dist<0)
        return -dist;
    return dist;
}

void Plane::draw()
{
    n.normalize();

    glBegin(GL_LINES);
    glVertex3fv(p.vec);
    glVertex3fv((p+n).vec);
    glEnd();

    Vector3<GLfloat> v1(1,-1,0);
    Vector3<GLfloat> v2(0,1,-1);
    Vector3<GLfloat> v3(-1,0,1);
    v1 = v1.cross(n);
    v2 = v2.cross(n);
    v3 = v3.cross(n);
    glBegin(GL_TRIANGLES);
    glVertex3fv((p+v1).vec);
    glVertex3fv((p+v2).vec);
    glVertex3fv((p+v3).vec);
    glEnd();
    
}

Point3<GLfloat> Plane::project(const Point3<GLfloat> &otherp) const
{
    return otherp-n*signedDistance(otherp);
}

Point3<GLfloat> Line::project(const Point3<GLfloat> &otherp) const
{
    return p+(otherp-p)*(d.normalized()*d.normalized());
}
