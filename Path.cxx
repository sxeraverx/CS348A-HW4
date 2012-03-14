#include "Datafile.h"
#include "MST.h"
#include "Path.h"
#include "Plane.h"
#include "QuadraticSegment.h"

#include <cassert>

#include <gl/glut.h>

using namespace std;

static void nurbsError(GLenum errorCode)
{
    cout << gluErrorString(errorCode) << endl;
    exit(0);
}

Path::Path(const MST &mst, const Datafile *mesh)
    : points(mst.orderedPoints()),
      mm(0),
      mesh(mesh)
{
    for(int i = 0; i < points.size(); i++)
    {
        std::vector<Point3<GLfloat> > tri = mesh->triangleBelow(points[i]);
        Vector3<GLfloat> norm = (tri[1]-tri[0]).cross(tri[2]-tri[0]);
        if(norm[2]<0) norm*=-1;
        normalsBelow.push_back(norm.normalized());
    }
    recalcDeBoorPoints();
    
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
    gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);

}

void Path::recalcDeBoorPoints()
{
    deBoorPoints.clear();
    deBoorPoints.push_back(points[0]);
    deBoorPoints.push_back(points[0]);
    float d = (points[1]-points[0]).magnitude()/2;
    deBoorPoints.push_back(points[0]+(points[1]-points[0])/2+Vector3<GLfloat>(
                               d*float(rand())/float(RAND_MAX),
                               d*float(rand())/float(RAND_MAX),
                               d*float(rand())/float(RAND_MAX))
        );
    deBoorPoints.push_back(points[1]);
    deBoorPoints.push_back(points[1]);
}

void Path::drawCube(Point3<GLfloat> center, GLfloat halfsize)
{
    GLfloat d = halfsize;
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glDisable(GL_LIGHTING);
    glutWireCube(2*d);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    /*
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_LOOP);
    {
        glVertex3fv((center+Vector3<GLfloat>( d, d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>( d,-d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>(-d,-d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>(-d, d, d)).vec);
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    {
        glVertex3fv((center+Vector3<GLfloat>( d, d,-d)).vec);
        glVertex3fv((center+Vector3<GLfloat>( d,-d,-d)).vec);
        glVertex3fv((center+Vector3<GLfloat>(-d,-d,-d)).vec);
        glVertex3fv((center+Vector3<GLfloat>(-d, d,-d)).vec);
    }
    glEnd();
    glBegin(GL_LINES);
    {
        glVertex3fv((center+Vector3<GLfloat>( d, d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>( d, d,-d)).vec);
        
        glVertex3fv((center+Vector3<GLfloat>( d,-d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>( d,-d,-d)).vec);

        glVertex3fv((center+Vector3<GLfloat>(-d,-d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>(-d,-d,-d)).vec);

        glVertex3fv((center+Vector3<GLfloat>(-d, d, d)).vec);
        glVertex3fv((center+Vector3<GLfloat>(-d, d,-d)).vec);
    }
    glEnd();
    */
}

vector<QuadraticSegment> Path::generatePOIQuadSegs()
{

    vector<QuadraticSegment> quadsegs;

    for(int i = 1; i < points.size()-1; i++)
    {
        Plane p1;
        p1.p = points[i];
        p1.n = Vector3<GLfloat>(((points[i-1]-points[i]).normalized()+(points[i+1]-points[i]).normalized())/2);
        p1.n.normalize();

        Plane p2;
        p2.p = p1.p;
        p2.n = normalsBelow[i];
        
        QuadraticSegment qs(p1, p2, points[i], points[i-1], points[i+1]);
        bool intersected = false;
        while(intersectsTerrain(qs))
        {
            intersected = true;
            cerr << "Intersects terrain" << endl;
            Vector3<GLfloat> v = qs.cp[2]-qs.cp[0];
            qs.cp[0]+=v/4;
            qs.cp[2]-=v/4;
        }
        if(intersected)
        {
            cerr << "ok" << endl;
            qs.color = Vector3<GLfloat>(1,0,1);
        }
        quadsegs.push_back(qs);
    }
    return quadsegs;
}

bool Path::intersectsTerrain(QuadraticSegment qs)
{
    bool intersectsTerrain = false;
    for(int i = 0; i <=100; i++)
    {
        GLfloat t = i/100.0;
        Point3<GLfloat> o(0,0,0);
        
        Point3<GLfloat> p = o
            + (1-t)*(1-t)*(qs.cp[0]-o)
            + (1-t)*t*(qs.cp[1]-o)
            + t*t*(qs.cp[2]-o);
        try
        {
            vector<Point3<GLfloat> > triangle = mesh->triangleBelow(p);
            Vector3<GLfloat> n = (triangle[1]-triangle[0]).cross(triangle[2]-triangle[0]);
            if(n[2]<0) n*=-1;
            n.normalize();
            if(n*(p-triangle[0]) < 0)
                intersectsTerrain = true;
        } catch(const char *s) {}
    }
    return intersectsTerrain;
}

vector<QuadraticSegment> Path::mergeQuadSegs(vector<QuadraticSegment> quadsegs)
{
    vector<QuadraticSegment> newQuadSegs(quadsegs);
    static bool addedPoints = false;
    bool ap = false;
    
    for(vector<QuadraticSegment>::iterator qs = quadsegs.begin(); qs!=quadsegs.end()-1; qs++)
    {

        Line l = qs->nextPlane().intersect((qs+1)->prevPlane());
        //Line l = qs->osculatingPlane().intersect((qs+1)->osculatingPlane());

        Line thisForward, nextBackward;
        thisForward.p = qs->cp[2];
        thisForward.d = qs->cp[2]-qs->cp[1];
        nextBackward.p = (qs+1)->cp[0];
        nextBackward.d = (qs+1)->cp[0]-(qs+1)->cp[1];

        Point3<GLfloat> pp1 = l.intersect(thisForward);
        Point3<GLfloat> pp2 = l.intersect(nextBackward);
        glColor3f(0,0,1);
        glBegin(GL_POINTS);
        glVertex3fv(pp1.vec);
        glVertex3fv(pp2.vec);
        glEnd();

        int cd = 0;
        if((qs->cp[1]-qs->cp[2]).magnitude() > (qs->cp[1]-pp1).magnitude()
            || (pp1-qs->cp[1]).magnitude() < (pp1-qs->cp[2]).magnitude())
        {
            glBegin(GL_POINTS);
            glColor3f(1,1,1);
            glVertex3fv(qs->cp[2].vec);
            glEnd();
            cerr << "cusp detected" << endl;
            cd++;
        }
        if(((qs+1)->cp[1]-(qs+1)->cp[0]).magnitude() > ((qs+1)->cp[1]-pp2).magnitude()
            || (pp2-(qs+1)->cp[1]).magnitude() < (pp2-(qs+1)->cp[0]).magnitude())
        {
            glBegin(GL_POINTS);
            glColor3f(1,1,1);
            glVertex3fv((qs+1)->cp[0].vec);
            glEnd();
            cerr << "cusp detected" << endl;
            cd++;
        }
        


        Point3<GLfloat> mp = pp1+(pp2-pp1)/2;
        if(cd && !addedPoints)
        {
            vector<Point3<GLfloat> >::iterator pt = points.begin();
            for(pt = points.begin(); !((*pt)==qs->poi) && pt!=points.end(); pt++)
                ;
            if(pt==points.end())
            {
                cerr << qs->cp[1] << endl;
                for(pt = points.begin(); !((*pt)==qs->poi) && pt!=points.end(); pt++)
                    cerr << *pt << endl;
                exit(0);
            }

            //cerr << "pushing back " << mp << " between " << *(pt-1) << " and " << *pt << endl;
            //int i = pt-points.begin();
            //points.insert(pt, mp);
            //normalsBelow.insert(normalsBelow.begin()+i, Vector3<GLfloat>(0,0,-1));
            //return newQuadSegs;
            ap = true;
            
        }
        QuadraticSegment qs1, qs2;
        qs1.cp[0] = qs->cp[2];
        qs1.cp[1] = pp1;
        qs1.cp[2] = mp;
        qs2.cp[0] = mp;
        qs2.cp[1] = pp2;
        qs2.cp[2] = (qs+1)->cp[0];
        newQuadSegs.push_back(*qs);
        newQuadSegs.push_back(qs1);
        newQuadSegs.push_back(qs2);
        if(intersectsTerrain(qs1))
            cerr << "Intersects terrain" << endl;
        if(intersectsTerrain(qs2))
            cerr << "Intersects terrain" << endl;
    }
    newQuadSegs.push_back(quadsegs.back());

    addedPoints = addedPoints || ap;
    return newQuadSegs;

}

void Path::draw()
{
    glPointSize(5);
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    glColor3f(0, 0, 1);
    for(vector<Point3<GLfloat> >::iterator iter = points.begin(); iter!=points.end(); iter++)
    {
        glVertex3fv(iter->vec);
    }
    glEnd();
    if(!quadsegs.size())
    {
        quadsegs = generatePOIQuadSegs();
        quadsegs = mergeQuadSegs(quadsegs);
    }
    for(vector<QuadraticSegment>::iterator qs = quadsegs.begin(); qs!=quadsegs.end(); qs++)
    {
        qs->draw(theNurb);
        glColor3f(0,0,1);
        qs->nextPlane().draw();
        glColor3f(1,0,1);
        qs->prevPlane().draw();
        glColor3f(1,1,1);
        qs->osculatingPlane().draw();
    }

    


    glEnable(GL_LIGHTING);

    for(vector<Point3<GLfloat> >::iterator iter = points.begin(); iter!=points.end(); iter++)
    {
        drawCube(*iter, dd);
    }

    
    glDisable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
}

void Path::d(float d)
{
    dd = d;
}

void Path::m(float m)
{
    mm = m;
    recalcDeBoorPoints();
}
