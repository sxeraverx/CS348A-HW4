#pragma once

#include "Plane.h"

#include <GL/glu.h>

#include <algorithm>
#include <vector>

class QuadraticSegment
{
public:
    Point3<GLfloat> cp[3];
    Point3<GLfloat> poi;
    Vector3<GLfloat> color;
    Plane p1;
    bool hasPoi;

    QuadraticSegment() : color(0,1,0), hasPoi(false) {}

    QuadraticSegment(Plane p1, Plane p2, Point3<GLfloat> point, Point3<GLfloat> prevPoint, Point3<GLfloat> nextPoint, GLfloat modifier = 1)
        : color(0,1,0),
        poi(point),
        hasPoi(true)
    {
        poi=point;
        //if(p2.n[2]==-1)
        //    p2=p2;
        p1.n.normalize();
        p2.n.normalize();
        Line l = p1.intersect(p2);
        l.d.normalize();
        Vector3<GLfloat> npd = nextPoint-point, ppd = prevPoint-point;
        npd[2] = 0;
        ppd[2] = 0;
        GLfloat d4 = std::min((npd).magnitude(), (ppd).magnitude());
        GLfloat d = d4/10;
        cp[0] = point-l.d.normalized()*d/modifier+(p1.n+p2.n).normalized()*d;
        cp[1] = point-(p1.n+p2.n).normalized()*d;
        cp[2] = point+l.d.normalized()*d/modifier+(p1.n+p2.n).normalized()*d;
    }
    
    Plane osculatingPlane() const
    {
        Plane p;
        Point3<GLfloat> p1 = cp[0]+(cp[2]-cp[0])/2;
        p.p = cp[1]+(p1-cp[1])/2;
        p.n = (cp[0]-cp[1]).cross(cp[2]-cp[1]);
        if(p.n*(Vector3<GLfloat>(0,0,1)) < 0)
            p.n*= -1;
        return p;
    }

    void draw(GLUnurbsObj *theNurb)
    {
        glColor3fv(color.vec);
        std::vector<GLfloat> knots;
        knots.push_back(0);
        knots.push_back(0);

        knots.push_back(0);
        knots.push_back(1);

        knots.push_back(1);
        knots.push_back(1);
        
        gluBeginCurve(theNurb);
        gluNurbsCurve(theNurb, knots.size(), &knots[0], sizeof(Point3<GLfloat>)/sizeof(GLfloat), &cp[0][0], 3, GL_MAP1_VERTEX_3);
        gluEndCurve(theNurb);
    }

    Plane prevPlane() const
    {
        Plane p;
        p.n = (cp[0]-cp[1]).cross((cp[0]-cp[1]).cross(poi-cp[1])).normalized();
        p.p = cp[0];
        p.n += osculatingPlane().n.normalized();
        p.n.normalize();
        //p.n = osculatingPlane().n.normalized();
        return p;
    }
    Plane nextPlane() const
    {
        Plane p;
        p.n = (cp[2]-cp[1]).cross((cp[2]-cp[1]).cross(poi-cp[1])).normalized();
        p.p = cp[2];
        p.n += osculatingPlane().n.normalized();
        p.n.normalize();
        //p.n = osculatingPlane().n.normalized();
        return p;
    }

    GLfloat length() const
    {
        GLfloat len = 0;
        for(int i = 0; i < 100; i++)
        {
            
            GLfloat s = (i+0)/100.0;
            GLfloat t = (i+1)/100.0;
            
            Point3<GLfloat> ps = evaluate(s);
            Point3<GLfloat> pt = evaluate(t);
            len+=(pt-ps).magnitude();
        }
        return len;
    }

    GLfloat curvature() const
    {
        GLfloat curvature = 0;
        for(int i = 1; i < 99; i++)
        {
            
            GLfloat q = (i-1)/100.0;
            GLfloat r = (i+0)/100.0;
            GLfloat s = (i+1)/100.0;
            GLfloat t = (i+2)/100.0;
            
            Point3<GLfloat> pq = evaluate(q);
            Point3<GLfloat> pr = evaluate(r);
            Point3<GLfloat> ps = evaluate(s);
            Point3<GLfloat> pt = evaluate(t);

            Vector3<GLfloat> gamma_prime = (ps-pr);
            Vector3<GLfloat> gamma_doubleprime = (pr-pq)-(pt-ps);
            GLfloat curv = gamma_prime.cross(gamma_doubleprime).magnitude()
                / pow(gamma_prime.magnitude(), 3);
            curvature = std::max(curv, curvature);
        }
        std::cout << cp[0] << std::endl;
        return curvature;
    }

    Point3<GLfloat> evaluate(GLfloat t) const {
        Point3<GLfloat> o(0,0,0);
        Point3<GLfloat> pt = o
            + (1-t)*(1-t)*(cp[0]-o)
            + (1-t)*t*(cp[1]-o)*2
            + t*t*(cp[2]-o);
        return pt;
    }
};
