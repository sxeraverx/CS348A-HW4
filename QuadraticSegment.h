#pragma once

#include "Plane.h"

#include <GL/glu.h>

#include <vector>

class QuadraticSegment
{
public:
    Point3<GLfloat> cp[3];
    Point3<GLfloat> poi;
    Vector3<GLfloat> color;

    QuadraticSegment() : color(0,1,0){}

    QuadraticSegment(Plane p1, Plane p2, Point3<GLfloat> point, Point3<GLfloat> prevPoint, Point3<GLfloat> nextPoint, GLfloat modifier = 1)
        : color(0,1,0)
    {
        //std::cout << p2.n[2] << std::endl;
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
        GLfloat d = d4/4;
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
        /*
        glBegin(GL_LINE_STRIP);
        {
            glVertex3fv(cp[0].vec);
            glVertex3fv(cp[1].vec);
            glVertex3fv(cp[2].vec);
        }
        glEnd();
        */
    }

    Plane prevPlane() const
    {
        Plane p;
        //p.n = (cp[0]-cp[1]).cross((cp[0]-cp[1]).cross(poi-cp[1])).normalized();
        p.p = cp[0];
        //p.n += osculatingPlane().n.normalized();
        //p.n.normalize();
        p.n = osculatingPlane().n.normalized();
        return p;
    }
    Plane nextPlane() const
    {
        Plane p;
        //p.n = (cp[2]-cp[1]).cross((cp[2]-cp[1]).cross(poi-cp[1])).normalized();
        p.p = cp[2];
        //p.n += osculatingPlane().n.normalized();
        //p.n.normalize();
        p.n = osculatingPlane().n.normalized();
        return p;
    }
};
