#include <math.h>

 void direct_gspline(l,bez_x,bez_y)
/* From given interior Bezier points,
   the junction Bezier points b3i are found from the G2 conditons.
	Input:	 l:	no of cubic pieces
		bez_x,bez_y:  interior Bezier points
	Output: bez_x,bez_y:  completed piecewise Bezier polygon
*/
	int l;
	float bez_x[],bez_y[];
{
	int i,i3;
	float h_minus,h_plus;
	float height();

	for(i=1; i<l; i++)
	{
		i3=i*3;
		h_minus=height(bez_x[i3-2],bez_y[i3-2],bez_x[i3-1],
			       bez_y[i3-1],bez_x[i3+1],bez_y[i3+1]);
		h_plus =height(bez_x[i3+2],bez_y[i3+2],bez_x[i3-1],
			       bez_y[i3-1],bez_x[i3+1],bez_y[i3+1]);
		h_minus=sqrt(fabs(h_minus));
		h_plus =sqrt(fabs(h_plus));

		bez_x[i3]=(h_plus*bez_x[i3-1]+h_minus*bez_x[i3+1])/
			  (h_minus + h_plus);
		bez_y[i3]=(h_plus*bez_y[i3-1]+h_minus*bez_y[i3+1])/
			  (h_minus + h_plus);
	}
}


	
