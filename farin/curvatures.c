#include <usercore.h>
#include <stdio.h>
#include <math.h>
extern FILE *outfile;

void curvatures(coeffx,coeffy,weight,degree,dense)
/*	writes curvatures of rational  Bezier curve into 
	a file.
input:
	coeffx, coeffy:  2D Bezier polygon
	weight:	the weights
	degree:	the degree
	dense:	how many curvature values to compute
output:
	written into file outfile
*/

	float coeffx[], coeffy[], weight[];
	int degree, dense;
{
	float bleftx[10],blefty[10],brightx[10],brighty[10];
	float wleft[10], wright[10];
	float dist,t,delt,h;
	int i,j;
	float curvature_1(),curvature_0(),abs();

	delt=1.0/(float)dense;
	i=0;
	for(t=0.0; t<0.5; t=t+delt)
	{
		subdiv(degree,coeffx,weight,t,bleftx,brightx,wleft,wright);		subdiv(degree,coeffy,weight,t,blefty,brighty,wleft,wright);
		h=curvature_0(brightx,brighty,wright,degree);
		h=abs(h);
		fprintf(outfile,"%2.3f  %f\n",t,h);
		i++;

	}

	for(t=t; t<=1.00001; t=t+delt)
	{
		subdiv(degree,coeffx,weight,t,bleftx,brightx,wleft,wright);		subdiv(degree,coeffy,weight,t,blefty,brighty,wleft,wright);

		h=curvature_0(bleftx,blefty,wleft,degree);
		h=0.0-h;
		/* minus sign since order of polygon 
		   traversal is reversed!
		*/
		h=abs(h);
		fprintf(outfile,"%2.3f  %f\n",t,h);
		i++;
	}

}
