void ratbspline_to_bezier(bspl_x,bspl_y,bspl_w,knot,l,bez_x,bez_y,bez_w)
/* converts  planar rational  cubic B-spline polygon into piecewise
rational Bezier polygon 
Input:	bspl: B-spline control polygon 
	bspl_w: B-spline weights
	knot: knot sequence
	l:    no. of intervals

Output:	bez_x,bez_y:  piecewise Bezier polygon 
	bez_w: Bezier weights (not neceess. in piecewise standard form)
*/


	float bspl_x[],bspl_y[], bspl_w[], knot[];
	float bez_x[], bez_y[], bez_w[];
	int l;
{
	int i, i3, l3, l2;
	float hbez_x[300], hbez_y[300], hbez_w[300];
	float hbspl_x[100], hbspl_y[100];


	l2=l+2; l3=l*3;

	for(i=0; i<= l2; i++) 
	{
		hbspl_x[i]=bspl_x[i]*bspl_w[i];
		hbspl_y[i]=bspl_y[i]*bspl_w[i];
	}

	bspline_to_bezier(bspl_w,knot,l,hbez_w);
	bspline_to_bezier(hbspl_x,knot,l,hbez_x);
	bspline_to_bezier(hbspl_y,knot,l,hbez_y);


	for(i=0; i<= l3; i++) 
	{
		bez_x[i]=hbez_x[i]/hbez_w[i];
		bez_y[i]=hbez_y[i]/hbez_w[i];
		bez_w[i]=hbez_w[i];
	}


}
