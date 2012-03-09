
#include <stdio.h>


main(int argc, char **argv)

{

	FILE *f1, *f2;
	char *dname = argv[3];
	char unx[80], pc[80], cmd[256];
	int done = 0, nfiles = 0;

	f1 = fopen(argv[1], "r");
	f2 = fopen(argv[2], "r");
	 
	while(!done){
	 	if((fscanf(f1, "%s", unx)) == NULL)
			goto quit;
	 	fscanf(f2, "%s", pc);

		sprintf(cmd, "/usr/sbin/to_dos %s %s/%s\n", unx, dname, pc);
		system(cmd);
		nfiles++;

	}


quit:
	printf("Processed %d files\n", nfiles);

}
	 
