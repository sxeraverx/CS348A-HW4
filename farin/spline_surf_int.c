 void spline_surf_int(data_x,data_y,bspl_x,bspl_y,lu,lv,knot_u, knot_v,aux_x,aux_y)
/* 	Interpolates to an array of size [0,lu+2]x[0,lv+2]
Input:	data_x,data_y: data points
	lu,lv:  no. of intervals in u- and v-direction
	knot_u, knot_v:   knot vectors in u- and v-direction
Output:	bspl_x,bspl_y:   B-spline control net.

Remark:	On input, it is assumed that data_x and data_y have rows
	1 and lu+1 and colums 1 and lv+1 empty, i.e., they are
	not filled with data points. Example for lu=4, lv=7:

		x0xxxxxx0x
		0000000000
		x0xxxxxx0x 		here, x=data coordinate,
		x0xxxxxx0x 		      0=unused  input array 
		x0xxxxxx0x 		        element.
		0000000000
		x0xxxxxx0x
*/
	float bspl_x[20][20],bspl_y[20][20],
	      data_x[20][20],data_y[20][20],
	      knot_u[],knot_v[],
	      aux_x[20][20],aux_y[20][20];
	int  lu,lv;
{
	int  k,i,j,ii,jj,lu2,lv2;
	float help_x[20],help_y[20],alpha[20],
	      beta[20],gamma[20],up[20],low[20];
	float  help2_x[20],help2_y[20];

	lu2=lu+2; lv2=lv+2;  
	/* interpolate all rows of data: */

	/*set up system in v-direction:  */

	set_up_system(knot_v,lv,alpha,beta,gamma);

for(i=0;i<=lv;i++)printf("%3.1f, %3.1f, %3.1f, %3.1f\n",
knot_v[i],alpha[i],beta[i],gamma[i]);


	l_u_system(alpha,beta,gamma,lv,up,low);
	for(i=0;  i<= lu2; i++)
	{

		for(j=0; j<=lv2; j++)   
			{
			help_x[j]=data_x[i][j];
			help_y[j]=data_y[i][j];
			}
			
			bessel_ends(help_x,knot_v,lv);
			bessel_ends(help_y,knot_v,lv);
			solve_system(up,low,gamma,lv,help_x,help2_x);
			solve_system(up,low,gamma,lv,help_y,help2_y);

		for(j=0; j<=lv2; j++)
			{
			aux_x[i][j]=help2_x[j];
			aux_y[i][j]=help2_y[j];
			}
	}

	for(ii=0; ii<=lu2; ii++)
	for(jj=0; jj<=lv2; jj++)

	/* now interpolate all columns of aux: */

	set_up_system(knot_u,lu,alpha,beta,gamma);
	l_u_system(alpha,beta,gamma,lu,up,low);


	for(j=0;j<=lv2; j++)
	{
		for(i=0; i<=lu2; i++)
			{
			help_x[i]=aux_x[i][j];
			help_y[i]=aux_y[i][j];
			}
	
			bessel_ends(help_x,knot_u,lu);
			bessel_ends(help_y,knot_u,lu);
			solve_system(up,low,gamma,lu,help_x,help2_x);
			solve_system(up,low,gamma,lu,help_y,help2_y);


		for(i=0; i<=lu2; i++)
			{
			bspl_x[i][j]=help2_x[i];
			bspl_y[i][j]=help2_y[i];
			}
	}
}
		
		
