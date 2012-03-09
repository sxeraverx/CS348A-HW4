void bspl_to_bez_surf(bspl,lu,lv,knot_u, knot_v, bez,aux)
/* 	Converts B-spline control net into piecewise
 	Bezier control nets (bicubic).
Input:	bspl:   B-spline control net (one coordinate only)
	lu,lv:  no. of intervals in u- and v-direction
	knots_u, knots_v:   knot vectors in u- and v-direction
Output:	bez:    piecewise bicubic Bezier net.

Remark:	The Bezier net only stores each control point once,
	i.e., neighboring patches share the same boundary.
	Knots are simple. 
*/
	float bspl[20][20],bez[20][20],knot_u[],knot_v[],aux[20][20];
	int  lu,lv;   			/*dimensions might have */
				        /*	 to be changed! */
{
	int  k,i,j,ii,jj,lu2,lv2,lu3,lv3;
	float help[20];
	static float  help2[20];

	lu2=lu+2; lv2=lv+2;  lu3=lu*3; lv3=3*lv;
	/* convert all rows of bspl to bezier: */


	for(i=0;  i<= lu2; i++)
	{

		for(j=0; j<=lv2; j++)   help[j]=bspl[i][j];

	 	bspline_to_bezier(help,knot_v,lv,help2);

		for(j=0; j<=lv3; j++)   aux[i][j]=help2[j];

	}

	/* now convert all columns of aux to bezier: */
	for(j=0;j<=lv3; j++)
	{
		for(i=0; i<=lu2; i++)	help[i]=aux[i][j];

		bspline_to_bezier(help,knot_u,lu,help2);

		for(i=0; i<=lu3; i++)   bez[i][j]=help2[i];
	}
}
		
		
