/*
 * *****************************************************************
 *
 * ROUTINE:  deboor_blossom
 *
 * FUNCTION: deBoor algorithm to evaluate a B-spline curve blossom.
 *           For polynomial or rational curves. 
 *            
 *
 * INPUT:    control[] ........... [0]: indicates type of input curve
 *                                      0 = polynomial
 *                                      1 = rational
 *                                 [1]: indicates if input/output is
 *                                      in R3 or R4; 
 *                                      3 = R3
 *                                      4 = R4
 *           degree .............. polynomial degree of each piece 
 *                                 of the input curve, must be <=20
 *           deboor[][3] ......... deboor control points
 *           deboor_wts[] ........ rational weights associated with
 *                                 the control points if control[0]=1;
 *                                 otherwise weights not used 
 *           knot[] .............. knot sequence with multiplicities
 *           knot[] .............. knot sequence with multiplicities
 *                                 entered explicitly
 *           uvec[]  ............. blossom (parameter) values 
 *                                 to evaluate
 *           interval ............ interval within knot sequence
 *                                 with which to evaluate wrt u
 *                                 (typically: i=interval then
 *                                 knot[i]<= u < knot[i+1])
 *
 * OUTPUT:   point[3] ............ evaluation point;
 *                                 depending on control[] values,
 *                                 this point will be in R3 or R4
 *           point_wt ............ if control[0]=1 then this is the
 *                                 rational weight associated with
 *                                 the point
 *
 * 
 * METHOD:   This routine uses a modified version of the deBoor
 *           algorithm. The only modification is that at the kth
 *           level of the algorithm, the kth input knot value is
 *           used. In the standard algoithm, the same knot value 
 *           is used for all k. The definition of a blossom may be 
 *           found in Farin's book.  
 *
 *
 * *****************************************************************
 */


#include <math.h>

deboor_blossom(control,degree,deboor,deboor_wts,
               knot,uvec,interval,point,point_wt)

    double deboor[][3];
    double deboor_wts[];
    double knot[];
    double uvec[];
    double point[3];
    double *point_wt;
    int    control[2];
    int    degree;
    int    interval;
{
    double t1;
    double t2;
    double zcheck;
    double db[20][3];
    double db_wts[20];
    double pp[3];
    double factor;
    double zero_divide = 0.0000005;  /*change if desired! */
    int    k;
    int    j;
    int    j0;
    int    ixyz;
    int    icount;
    int    istart;
    int    iend;

    /* ---------------------------------------------------- */
	
    /*
     * NUMBER OF CONTROL POINTS USED IN EVALUATION = 'degree+1'
     * STORE THESE POINTS IN A TEMPORARY ARRAY TO AVOID
     * CHANGES TO INPUT CONTROL POINTS
     */

    icount = -1;
    istart = interval-degree+1;
    iend   = interval+1;
    for (j=istart; j<=iend; j++) {
      icount++;
      if(control[0] == 0) 
        for(ixyz=0; ixyz<3; ixyz++)
          db[icount][ixyz]=deboor[j][ixyz];
      else {
        if(control[1] == 3) factor = deboor_wts[j];
        else factor = 1.0;
        for(ixyz=0; ixyz<3; ixyz++)
          db[icount][ixyz]=deboor[j][ixyz]*factor;
        db_wts[icount] = deboor_wts[j];
      }
    }


    /*
     * EXECUTE DEBOOR ALGORITHM, TAKING NOTE THAT THE DEBOOR POINTS
     * IN THE TEMPORARY ARRAY ARE INDEXED 0..degree
     */

    for(k=1; k<=degree; k++) {

      for(j=iend ;j>=istart+k; j--) {

        zcheck = knot[j+degree-k] - knot[j-1];
        if(zcheck < zero_divide) zcheck = zero_divide;

        /* NOTE: AT LEVEL k WE INSERT (K-1)th U-VALUE */

        t1 = (knot[j+degree-k] - uvec[k-1] )/zcheck;
        t2 = 1.0 - t1;

        /* ADJUST STANDARD INDEX FOR TEMP ARRAY */

        j0 = j - istart;

        for(ixyz=0; ixyz<3; ixyz++)
          db[j0][ixyz] = t1*db[j0-1][ixyz] + t2*db[j0][ixyz];

        if(control[0] == 1)
          db_wts[j0] = t1*db_wts[j0-1] + t2*db_wts[j0];
      }	
    }

    /* 
     * EXTRACT THE EVALUATION POINT FROM THE TEMP ARRAY
     */

    for(ixyz=0; ixyz<3; ixyz++)
      point[ixyz] = db[degree][ixyz];

    /* 
     * TAKE SPECIAL CARE FOR RATIONAL CURVES; BE SURE TO RETURN
     * POINT IN R3 OR R4 AS REQUIRED
     */

    if(control[0] == 1) {
      *point_wt = db_wts[degree];
      if(control[1] == 3) {
        if(fabs(*point_wt) < zero_divide) *point_wt = zero_divide;
        for(ixyz=0; ixyz<3; ixyz++)
          point[ixyz] = point[ixyz] / *point_wt;
      }
    }
}
