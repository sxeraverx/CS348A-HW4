 void ratbspl_to_bez_surf(bspl_x,bspl_y,bspl_w,lu,lv,knot_u, knot_v,bez_x,bez_y,bez_w,aux_x,aux_y,aux_w)
/* 	Converts B-spline control net into piecewise
 	Bezier control nets (rat bicubic).
Input:	bspl:   B-spline control net 
	lu,lv:  no. of intervals in u- and v-direction
	knots_u, knots_v:   knot vectors in u- and v-direction
Output:	bez:    piecewise rat. bicubic Bezier net.

Remark:	The Bezier net only stores each control point once,
	i.e., neighboring patches share the same boundary.
	Knots are simple. 
*/
	float bspl_x[20][20],bspl_y[20][20],
	      bspl_w[20][20],bez_x[20][20],
	      bez_y[20][20],bez_w[20][20],
		knot_u[],knot_v[],
	      aux_x[20][20],aux_y[20][20],aux_w[20][20];
	int  lu,lv;
{
	int  i,j,lu2,lv2,lu3,lv3;
	float help_x[20],help_y[20], help_w[20];
	static float  help2_x[20],help2_y[20],help2_w[20];

	lu2=lu+2; lv2=lv+2;  lu3=lu*3; lv3=3*lv;
	/* convert all rows of bspl to bezier: */

	for(i=0;  i<= lu2; i++)
	{

		for(j=0; j<=lv2; j++)   
			{
			help_x[j]=bspl_x[i][j];
			help_y[j]=bspl_y[i][j];
			help_w[j]=bspl_w[i][j];
			}
	 	ratbspline_to_bezier(help_x,help_y,help_w,
		knot_v,lv,help2_x,help2_y,help2_w);

		for(j=0; j<=lv3; j++)
			{
			aux_x[i][j]=help2_x[j];
			aux_y[i][j]=help2_y[j];
			aux_w[i][j]=help2_w[j];
			}
	}

	/* now convert all columns of aux to bezier: */

	for(j=0;j<=lv3; j++)
	{

		for(i=0; i<=lu2; i++)
			{
			help_x[i]=aux_x[i][j];
			help_y[i]=aux_y[i][j];
			help_w[i]=aux_w[i][j];
			}
		ratbspline_to_bezier(help_x,help_y,help_w,
		knot_u,lu,help2_x,help2_y,help2_w);

		for(i=0; i<=lu3; i++)
			{
			bez_x[i][j]=help2_x[i];
			bez_y[i][j]=help2_y[i];
			bez_w[i][j]=help2_w[i];
			}
	}
}
		
		
