#include "Path.h"

using namespace std;

static void nurbsError(GLenum errorCode)
{
    cout << gluErrorString(errorCode) << endl;
    exit(0);
}

Path::Path(const MST &mst)
    : points(mst.orderedPoints()), mm(0)
{
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
    /*
    for(int j = 0; j < points.size()+1; j++)
    {
        deBoorPoints.push_back(points[0]);
    }

    deBoorPoints[0]=points[0];
    deBoorPoints.back()=points.back();

    for(int i = 1; i < deBoorPoints.size()-1; i++)
    {
        for(int j = 0; j < points.size(); j++)
        {
            GLfloat factor;
            if(i<=j)
            {
                factor = (14.0-j)/14.0;
                if((i+j)%2==1) factor = -factor;
            cout << factor << " ";
                factor *= (points[j]-points[j-1]).magnitude()+(points[j+1]-points[j]).magnitude();
                factor /= (points[i]-points[i-1]).magnitude();
            }
            else
            {
                factor = (j)/14.0;
                if((i+j)%2==0) factor = -factor;
            cout << factor << " ";
                factor *= (points[j]-points[j-1]).magnitude()+(points[j+1]-points[j]).magnitude();
                factor /= (points[i]-points[i-1]).magnitude();
            }
            deBoorPoints[i]+=(points[j]-points[0])*factor;
        }
        cout << endl;
    }

    for(int i = 1; i < deBoorPoints.size()-1; i++)
    {
        if(i%2==1)
            deBoorPoints[i] -= (points[1]-points[0])/(points[i]-points[i-1]).magnitude()*mm;
        else
            deBoorPoints[i] += (points[1]-points[0])/(points[i]-points[i-1]).magnitude()*mm;
    }
    */
}

void Path::drawCube(Point3<GLfloat> center, GLfloat halfsize)
{
    GLfloat d = halfsize;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_LOOP);
    {
        glVertex3fv(center+Vector3<GLfloat>( d, d, d));
        glVertex3fv(center+Vector3<GLfloat>( d,-d, d));
        glVertex3fv(center+Vector3<GLfloat>(-d,-d, d));
        glVertex3fv(center+Vector3<GLfloat>(-d, d, d));
    }
    glEnd();
    glBegin(GL_LINE_LOOP);
    {
        glVertex3fv(center+Vector3<GLfloat>( d, d,-d));
        glVertex3fv(center+Vector3<GLfloat>( d,-d,-d));
        glVertex3fv(center+Vector3<GLfloat>(-d,-d,-d));
        glVertex3fv(center+Vector3<GLfloat>(-d, d,-d));
    }
    glEnd();
    glBegin(GL_LINES);
    {
        glVertex3fv(center+Vector3<GLfloat>( d, d, d));
        glVertex3fv(center+Vector3<GLfloat>( d, d,-d));
        
        glVertex3fv(center+Vector3<GLfloat>( d,-d, d));
        glVertex3fv(center+Vector3<GLfloat>( d,-d,-d));

        glVertex3fv(center+Vector3<GLfloat>(-d,-d, d));
        glVertex3fv(center+Vector3<GLfloat>(-d,-d,-d));

        glVertex3fv(center+Vector3<GLfloat>(-d, d, d));
        glVertex3fv(center+Vector3<GLfloat>(-d, d,-d));
    }
    glEnd();
}

void Path::draw()
{
    glPointSize(5);
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    glColor3f(0, 0, 1);
    for(vector<Point3<GLfloat> >::iterator iter = points.begin(); iter!=points.end(); iter++)
    {
        glVertex3fv(*iter);
    }
    glColor3f(0, 1, 0);
    for(vector<Point3<GLfloat> >::iterator iter = deBoorPoints.begin(); iter!=deBoorPoints.end(); iter++)
    {
        glVertex3fv(*iter);
    }
    glEnd();
    glEnable(GL_LIGHTING);

    for(vector<Point3<GLfloat> >::iterator iter = points.begin(); iter!=points.end(); iter++)
    {
        drawCube(*iter, dd);
    }

    
    glDisable(GL_LIGHTING);
    
    vector<GLfloat> knots;
    knots.push_back(0.0);
    knots.push_back(0.0);
    int iprev = 0;
    for(int i = 0; i < deBoorPoints.size(); i++)
    {
        knots.push_back(knots.back()+1);
    }
    knots.push_back(knots.back());
    knots.push_back(knots.back());
            
    gluBeginCurve(theNurb);
    gluNurbsCurve(theNurb, knots.size(), &knots[0], sizeof(Point3<GLfloat>)/sizeof(GLfloat), &deBoorPoints[0][0], 3, GL_MAP1_VERTEX_3);
    gluEndCurve(theNurb);

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
