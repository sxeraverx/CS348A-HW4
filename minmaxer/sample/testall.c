#include <stdio.h>
#include "triangulation.h"

int GLOBAL_INTERRUPT_ALGORITHM = 0;

readCoords (x, y, z, nofSites)

     int **x, **y, **z, *nofSites;

{
  int i;
  FILE *fp, *fopen();

  fp = fopen ("testfile", "r");

  fscanf (fp, "%d", nofSites);

  (*x) = (int*) malloc (sizeof (int) * *nofSites);
  (*y) = (int*) malloc (sizeof (int) * *nofSites);
  (*z) = (int*) malloc (sizeof (int) * *nofSites);

  for (i = 0; i < *nofSites; i++)
    fscanf (fp, "%d %d %d\n", &((*x)[i]), &((*y)[i]), &((*z)[i]));
}

printTriangleList (tl)

     triangleList *tl;

{
  int i, j;

  for (i = 0; i < tl->nofTriangles; i++)
    printf ("%d %d %d\n", tl->v[i][0], tl->v[i][1], tl->v[i][2]);

/*  
  for (i = 0; i < tl->nofTriangles; i++) 
    for (j = 0; j < 3; j++)
      if (tl->v[i][j % 3] < tl->v[i][(j+1) % 3])
	printf ("%d %d\n", tl->v[i][j % 3], tl->v[i][(j+1) % 3]);
      else
	printf ("%d %d\n", tl->v[i][(j+1) % 3], tl->v[i][j % 3]);
*/
}

main ()

{
  int *x, *y, *z, n;
  char *g;
  triangleList *list;

  readCoords (&x, &y, &z, &n);
  copyCoordinatesToGraph (n, x, y, NULL, 1, &g);
  planeSweep (g);
  delaunay1 (g);
  minmaxAngle (g);
  delaunay2 (g);
  maxminHeight (g);
  regular (g);
  minmaxSlope (g);
  saveTriangulation (g, "testout");
  copyGraphToListOfTriangles (g, &list);
  printTriangleList (list);
}