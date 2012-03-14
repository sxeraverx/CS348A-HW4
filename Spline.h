#pragma once

#include <vector>

#include "ControlPoint.h"

template <typename T, int order>
class Spline
{
    std::vector<ControlPoint<T> > control_points;
public:
    Point3<T> operator()(float t)
    {
        int l = int(t * (control_points.size()-1));
        Vector3<T> d[order+1][order+1];
        for(int ii = 0; ii <= order; ii++)
        {
            int i = l-order+ii;
            if(i<0) i = 0;
            if(i>=control_points.size()) i = control_points.size()-1;
            d[0][ii] = control_points[i]-control_points[0];
        }
        for(int k = 1; k <= order; k++)
        {
            for(int ii = k+1; ii <= order; ii++)
            {
                int i = l-order+k+ii;
                float u_i = 1.0*i/(control_points.size()-1);
                float u_in1k = (1.0*(i+order+1-k)/(control_points.size()-1));
                float alpha_k_i = t-u_i/(u_in1k-u_i);
                d[k][ii] = d[k-1][ii-1]*(1-alpha_k_i) + d[k-1][ii]*alpha_k_i;
            }
        }
        return control_points[0]+d[order][order];
    }
    Spline &operator<<(const ControlPoint<T> &p)
    {
        control_points.push_back(p);
        return *this;
    }
};
