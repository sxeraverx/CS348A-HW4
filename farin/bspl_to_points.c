#include <stdio.h>
#include <math.h>


bspl_to_points(degree,l,coeff_x,knot,dense,
		points_x,point_num)
/*
	generates points on B-spline curve. (one coordinate)
input:	degree:	polynomial degree of each piece of curve
	l:	number of intervals
	coeff:	B-spline control points
	knot:	knot sequence: knot[0]...knot[l+2*degree-2]
	dense:	how many points per segment
Output:	points:	output array.
	point_num: how many points are generated.
*/

	float coeff_x[],knot[],points_x[];
	int degree,l,dense,*point_num;

{
	int i,j,ii,kk,l2;
	float u, mmbox[4];
	float deboor();

	l2=l+degree-1;

	*point_num=0;
	for (i=degree-1; i<l+degree-1; i++)
	{
	if(knot[i+1]>knot[i])
		for(ii=0; ii<=dense; ii++)
		{
		u=knot[i]+ii*(knot[i+1]-knot[i])/dense;
		points_x[*point_num]=deboor(degree,coeff_x,knot,u,i);

		*point_num= (*point_num)+1;
		}	

	}

}
