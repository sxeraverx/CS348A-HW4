#include "MST.h"

#include "Vector.h"

#include <algorithm>
#include <limits>
#include <map>

using namespace std;

MST::MST(const std::vector<Point3<GLfloat> > &points) : points(points)
{
    vector<int> connectedPoints;
    vector<int> unconnectedPoints;
    for(int i = 0; i < points.size(); i++)
        unconnectedPoints.push_back(i);
    connectedPoints.push_back(0);
    remove(unconnectedPoints.begin(), unconnectedPoints.end(), 0);
    bool hasFirstEdge = false;
    bool hasLastEdge = false;
    while(unconnectedPoints.size()>0)
    {
        GLfloat minWeight=numeric_limits<GLfloat>::infinity();
        int u, v;
        for(vector<int>::iterator connectedIter = connectedPoints.begin(); connectedIter < connectedPoints.end(); connectedIter++)
        {
            for(vector<int>::iterator unconnectedIter = unconnectedPoints.begin(); unconnectedIter < unconnectedPoints.end(); unconnectedIter++)
            {
                GLfloat weight = (points[*connectedIter]-points[*unconnectedIter]).magnitude();
                if( weight < minWeight )
                {
                    u = *connectedIter;
                    v = *unconnectedIter;
                    minWeight = weight;
                }
            }
        }
        unconnectedPoints.erase(find(unconnectedPoints.begin(), unconnectedPoints.end(), v));
        connectedPoints.push_back(v);
        edges.push_back(make_pair(u, v));
        if(u==0)
            hasFirstEdge=true;
        if(v==points.size()-1)
            hasLastEdge=true;
    }
}

vector<Point3<GLfloat> > MST::orderedPoints() const
{
    return points;
    vector<int> pointList;
    
    int current_node = 0;
    for(current_node = 0; current_node < points.size(); current_node++)
    {
        int numEdges = 0;
        for(std::vector<std::pair<int, int> >::const_iterator iter = edges.begin(); iter != edges.end(); iter++)
        {
            if(iter->first==current_node)
                numEdges++;
            if(iter->second==current_node)
                numEdges++;
        }
        if(numEdges==1)
            break;
    }
    pointList.push_back(current_node);
    while(pointList.size() < points.size())
    {
        for(vector<std::pair<int, int> >::const_iterator edge = edges.begin(); edge!=edges.end(); edge++)
        {
            float minWeight = numeric_limits<float>::infinity();
            int nextNode;
            if(edge->first==current_node &&
               (points[current_node]-points[edge->second]).magnitude() < minWeight)
            {
                current_node = edge->second;
            }
            else if(edge->second==current_node &&
                    (points[current_node]-points[edge->first]).magnitude() < minWeight)
            {
                current_node = edge->first;
            }
        }
        pointList.push_back(current_node);
    }
    

    vector<Point3<GLfloat> > retval;
    for(vector<int>::iterator iter = pointList.begin(); iter != pointList.end(); iter++)
    {
        retval.push_back(points[*iter]);
    }

    return retval;
}


void MST::draw()
{
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    /*
    for(vector<pair<int, int> >::iterator iter = edges.begin(); iter!=edges.end(); iter++)
    {
        glVertex3fv(points[iter->first]);
        glVertex3fv(points[iter->second]);
    }*/
    for(vector<Point3<GLfloat> >::iterator iter = points.begin(); iter < points.end()-1; iter++)
    {
        glVertex3fv(iter->vec);
        glVertex3fv((iter+1)->vec);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}
