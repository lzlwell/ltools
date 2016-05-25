#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv)
{
	int nkpts;
	double **kpts;
	int *nsegs;
	double **dk;
	double weight = 1.0; // the weights of all kpoints are set equal in band structure calculations
	int totkpts;
	int i, j;

	FILE *fp;

	if(argc <2)
	{
		printf("Please provide one input file name (+ vasp/pw) \n");
		exit(0);
	}

	fp = fopen(argv[1], "r");

	fscanf(fp, "%d", &nkpts);
	while(fgetc(fp) != '\n');
	
	kpts = (double**)malloc(nkpts * sizeof(double*));
	nsegs = (int*)malloc((nkpts-1) * sizeof(int));
	dk = (double**)malloc((nkpts-1) * sizeof(double*));

	for(i=0; i<nkpts; i++)
	{
		kpts[i] = (double*)malloc(3 * sizeof(double));
		if(i < nkpts - 1)
			dk[i] = (double*)malloc(3 * sizeof(double));
	}

// read n-segment in each path
	totkpts = 0;
	for(i=0; i < nkpts - 1; i++)
	{
		fscanf(fp, "%d", nsegs + i);
		totkpts += nsegs[i];
	}
	while(fgetc(fp) != '\n');
	totkpts += 1;

// read the kpoints
	for(i=0; i<nkpts; i++)
	{
		fscanf(fp, "%lf %lf %lf", &kpts[i][0], &kpts[i][1], &kpts[i][2]);
		while(fgetc(fp) != '\n');
	}

	fclose(fp);

// calculate dk of each path
	for(i=0; i < nkpts - 1; i++)
		for(j=0; j<3; j++)
			dk[i][j] = (kpts[i+1][j] - kpts[i][j]) / (double)nsegs[i];

// print out the heads for vasp or pw

	if(argc == 3)
	{	
		if(strcmp("vasp", argv[2]) == 0)
		{
			printf("Automatically generated mesh\n");
			printf("   %d\n", totkpts);
			printf("Reciprocal lattice\n");
		}
		else if(strcmp("pw", argv[2]) == 0)
		{
			printf("K_POINTS crystal\n");
			printf("   %d\n", totkpts);
		}
		else
		{
			printf("Not a valid program name\n");
			printf("   %d\n", totkpts);
		}
	}
	else if(argc == 2)
	{
		printf("No valid program name provided\n");
		printf("   %d\n", totkpts);
	}

// print out the calculated kpoints paths
	for(i=0; i < nkpts - 1; i++)
		for(j=0; j<nsegs[i]; j++)
			printf("  %.10f  %.10f  %.10f     %f\n", kpts[i][0]+j*dk[i][0], kpts[i][1]+j*dk[i][1], kpts[i][2]+j*dk[i][2], weight);
	printf("  %.10f  %.10f  %.10f     %f\n", kpts[nkpts-1][0], kpts[nkpts-1][1], kpts[nkpts-1][2], weight); // and the last kpoint


	for(i=0; i<nkpts; i++)
	{
		free(kpts[i]);
		if(i < nkpts - 1)
			free(dk[i]);
	}

	free(kpts);
	free(nsegs);
	free(dk);

	return 0;
}

