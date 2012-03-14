#include "Datafile.h"
#include "Vector.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <typeinfo>

#include <GL/glu.h>

using namespace std;

Datafile::Datafile() {}

Datafile::Datafile(string path)
    : points(), edges(), triangles()
{
    ifstream file(path.c_str(), ios::in | ios::binary);
    string line;
    while(!file.eof())
    {
        getline(file, line);
        istringstream iss(line);
        string test;
        iss >> test;
        if(test=="site") {
            GLfloat x, y, z;
            iss >> x >> y >> z;
            points.push_back(Point4<GLfloat>(x/10.0, y/10.0, z/10.0));
        }
        else if(test=="edge")
        {
            int a, b;
            iss >> a >> b;
            if(a<b)
                edges.insert(pair<int, int>(a, b));
            else
                edges.insert(pair<int, int>(b, a));
        }
    }
    rebuildTriangles();
    recalculateNormals();
}

void Datafile::rebuildTriangles()
{
    triangles.clear();
    for(set<pair<int, int> >::iterator iter = edges.begin(); iter != edges.end(); iter++)
    {
        for(set<pair<int, int> >::iterator iter2 = edges.begin(); iter2 != edges.end(); iter2++)
        {
            if(iter->first==iter2->first && edges.find(pair<int, int>(min(iter->second, iter2->second), max(iter->second, iter2->second)))!=edges.end())
            {
                int a = iter->first;
                int b = min(iter->second, iter2->second);
                int c = max(iter->second, iter2->second);
                triangles.push_back(Vector3<GLuint>(a, b, c));
            }
        }
    }
}

void Datafile::splitTriangles()
{
    edges.clear();
    for(vector<Vector3<GLuint> >::iterator iter = triangles.begin(); iter!=triangles.end(); iter++)
    {
        int a = (*iter)[0], b = (*iter)[1], c = (*iter)[2];
        int e=-1, f=-1, g=-1;
        Point3<GLfloat> p4 = Point3<GLfloat>(points[a]) + (Point3<GLfloat>(points[b])-Point3<GLfloat>(points[a]))/2;
        Point3<GLfloat> p5 = Point3<GLfloat>(points[b]) + (Point3<GLfloat>(points[c])-Point3<GLfloat>(points[b]))/2;
        Point3<GLfloat> p6 = Point3<GLfloat>(points[c]) + (Point3<GLfloat>(points[a])-Point3<GLfloat>(points[c]))/2;

        vector<Point4 <GLfloat> >::reverse_iterator p4i = find(points.rbegin(), points.rend(), p4);
        vector<Point4 <GLfloat> >::reverse_iterator p5i = find(points.rbegin(), points.rend(), p5);
        vector<Point4 <GLfloat> >::reverse_iterator p6i = find(points.rbegin(), points.rend(), p6);
        if(p4i!=points.rend())
            e = &(*p4i)-&(*points.begin());
        if(p5i!=points.rend())
            f = &(*p5i)-&(*points.begin());
        if(p6i!=points.rend())
            g = &(*p6i)-&(*points.begin());
        
        if(e==-1)
            e=points.size();
        points.push_back(p4);
        if(f==-1)
            f=points.size();
        points.push_back(p5);
        if(g==-1)
            g=points.size();
        points.push_back(p6);
        
        edges.insert(pair<int, int>(min(a, e), max(a, e)));
        edges.insert(pair<int, int>(min(b, e), max(b, e)));
        edges.insert(pair<int, int>(min(b, f), max(b, f)));
        edges.insert(pair<int, int>(min(c, f), max(c, f)));
        edges.insert(pair<int, int>(min(c, g), max(c, g)));
        edges.insert(pair<int, int>(min(a, g), max(a, g)));
        edges.insert(pair<int, int>(min(e, f), max(e, f)));
        edges.insert(pair<int, int>(min(f, g), max(f, g)));
        edges.insert(pair<int, int>(min(g, e), max(g, e)));
    }
}
/**/
void Datafile::recalculateNormals()
{
    normals.clear();
    for(int i = 0; i < points.size(); i++)
        normals.push_back(Vector3<GLfloat>());
    for(vector<Vector3<GLuint> >::iterator iter = triangles.begin(); iter != triangles.end(); iter++)
    {
        Point3<GLfloat> p1(points[(*iter)[0]]);
        Point3<GLfloat> p2(points[(*iter)[1]]);
        Point3<GLfloat> p3(points[(*iter)[2]]);
        Vector3<GLfloat> u(p2-p1);
        Vector3<GLfloat> v(p3-p1);
        Vector3<GLfloat> n(u.cross(v));
        if(n[2]<0)
            n*=-1;
        n.normalize();
        normals[(*iter)[0]] += n;
        normals[(*iter)[1]] += n;
        normals[(*iter)[2]] += n;
    }
    for(vector<Vector3<GLfloat> >::iterator iter = normals.begin(); iter != normals.end(); iter++)
    {
        iter->normalize();
    }
}

void Datafile::drawTrianglesBelow(vector<Point3<GLfloat> > ps)
{
    glBegin(GL_TRIANGLES);
    Vector3<GLfloat> z(0,0,1);
    for(vector<Point3<GLfloat> >::iterator iter = ps.begin(); iter!=ps.end(); iter++)
    {
        vector<Point3<GLfloat> > tri(triangleBelow(*iter));
        Vector3<GLfloat> n = (tri[1]-tri[0]).cross(tri[2]-tri[0]);
        if(n*z<0) n*=-1;
        glVertex3fv(tri[0].vec);
        glVertex3fv(tri[1].vec);
        glVertex3fv(tri[2].vec);
    }
    glEnd();
}

void Datafile::draw()
{
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_COLOR_MATERIAL);

    //glVertexPointer(4, GL_FLOAT, 0, &points[0][0]);
    //glNormalPointer(GL_FLOAT, 0, &normals[0][0]);

    glColor3f(0,0,1);
    glBegin(GL_TRIANGLES);
    Vector3<GLfloat> z(0,0,1);
    for(vector<Vector3<GLuint> >::iterator tri = triangles.begin(); tri != triangles.end(); tri++)
    {
        glColor3f(.55,.27,.08);
        //if( )
        if(points[tri->vec[0]][2]<=0 && points[tri->vec[1]][2]<=0 && points[tri->vec[2]][2]<=0 && normals[tri->vec[0]].normalized()*z>.95)
            glColor3f(0,0,1);
        if(points[tri->vec[0]][2]>60)
            glColor3f(1,1,1);
        glNormal3fv(normals[tri->vec[0]].vec);
        glVertex4fv(points[tri->vec[0]].vec);
        if(points[tri->vec[1]][2]>60)
            glColor3f(1,1,1);
        glNormal3fv(normals[tri->vec[1]].vec);
        glVertex4fv(points[tri->vec[1]].vec);
        if(points[tri->vec[2]][2]>60)
            glColor3f(1,1,1);
        glNormal3fv(normals[tri->vec[2]].vec);
        glVertex4fv(points[tri->vec[2]].vec);
    }
    glEnd();
    //glDrawElements(GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, triangles[0]);

    glDisable(GL_COLOR_MATERIAL);
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    
    /*
    glPointSize(5);
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for(set<pair<int, int> >::iterator edge = edges.begin(); edge != edges.end(); edge++)
    {
        glVertex3fv(points[edge->first]);
        glVertex3fv(points[edge->second]);
    }
    glEnd();
    glColor3f(1, 1, 0);
    glBegin(GL_POINTS);
    for(vector<Vector3<GLuint> >::iterator iter = triangles.begin(); iter != triangles.end(); iter++)
    {
        Point3<GLfloat> p1(points[(*iter)[0]]);
        Point3<GLfloat> p2(points[(*iter)[1]]);
        Point3<GLfloat> p3(points[(*iter)[2]]);
        glVertex3fv(p1);
        glVertex3fv(p2);
        glVertex3fv(p3);
        //assert(points[(*iter)[0]][2]>0);
        //assert(points[(*iter)[1]][2]>0);
        //assert(points[(*iter)[2]][2]>0);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    */
}

GLfloat Datafile::distance(Point3<GLfloat> p) const
{
    Point3<GLfloat> pp(p);
    vector<Point3<GLfloat> > tri = triangleBelow(p);
    /*Convert into Point3*/
    Point3<GLfloat> pa(tri[0]);
    Point3<GLfloat> pb(tri[1]);
    Point3<GLfloat> pc(tri[2]);
    /*Project onto z=0*/
    pa[2] = 0;
    pb[2] = 0;
    pc[2] = 0;
    pp[2] = 0;
    //compute all edges
    Vector3<GLfloat> B = pb-pa;
    Vector3<GLfloat> C = pc-pa;
    //compute vector from each vertex
    Vector3<GLfloat> a = pp-pa;
    
    GLfloat dotCC = C*C;
    GLfloat dotCB = C*B;
    GLfloat dotCP = C*a;
    GLfloat dotBB = B*B;
    GLfloat dotBP = B*a;
    
    GLfloat den = (dotCC*dotBB-dotCB*dotCB);
    GLfloat u = (dotBB*dotCP-dotCB*dotBP) / den;
    GLfloat v = (dotCC*dotBP-dotCB*dotCP) / den;
    GLfloat dist =  p[2] - ((1-u-v)*tri[0][2] + u*tri[1][2] + v*tri[2][2]);
    
    return dist;
}

vector<Point3<GLfloat> > Datafile::triangleBelow(Point3<GLfloat> p, bool alreadyTried) const
{
    Point3<GLfloat> pp(p);
    for(vector<Vector3<GLuint> >::reverse_iterator iter = triangles.rbegin(); iter < triangles.rend(); iter++)
    {
        /*Convert into Point3*/
        Point3<GLfloat> pa(points[(*iter)[0]]);
        Point3<GLfloat> pb(points[(*iter)[1]]);
        Point3<GLfloat> pc(points[(*iter)[2]]);
        /*Project onto z=0*/
        pa[2] = 0;
        pb[2] = 0;
        pc[2] = 0;
        pp[2] = 0;
        //compute all edges
        Vector3<GLfloat> B = pb-pa;
        Vector3<GLfloat> C = pc-pa;
        //compute vector from each vertex
        Vector3<GLfloat> a = pp-pa;

        GLfloat dotCC = C*C;
        GLfloat dotCB = C*B;
        GLfloat dotCP = C*a;
        GLfloat dotBB = B*B;
        GLfloat dotBP = B*a;

        GLfloat den = (dotCC*dotBB-dotCB*dotCB);
        GLfloat u = (dotBB*dotCP-dotCB*dotBP);
        GLfloat v = (dotCC*dotBP-dotCB*dotCP);

        if (u>=0 && v>=0 && u+v<=den)
        {
            vector<Point3<GLfloat> > pts;
            pts.push_back(points[(*iter)[0]]);
            pts.push_back(points[(*iter)[1]]);
            pts.push_back(points[(*iter)[2]]);
            int idx = triangles.size()-1-(iter-triangles.rbegin());
            if(idx<19.0/20.0*triangles.size())
            {
                Vector3<GLuint> triangleIndices = *iter;
                assert(triangleIndices==*(triangles.begin()+idx));
                triangles.erase(triangles.begin()+idx);
                triangles.push_back(triangleIndices);
            }
            return pts;
        }
    }
    if(!alreadyTried)
        return triangleBelow(p+Vector3<GLfloat>(1.0e-7,1.0e-7,0), true);
    vector<Point3<GLfloat> > tri;
    throw("No triangle found");
}
