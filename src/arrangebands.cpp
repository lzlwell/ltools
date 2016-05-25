#include"arrangebands.h"

int main(int argc, char **argv)
{
	int nkpts, nbnds;
	double x, y, y2;
	double kthis[3], klast[3], kdir[3];
	double **reclatt, blat;
	double Efermi;
	int i, j;
	int arb;
	double wght;

	FILE *fp, *fp2, *fp3;
	FILE *gnufp2, *gnufp3;
	char gnuname[30];
	FILE *rl;

	printf("Please run reclatt.sh first to generate reclatt file.\n");
	printf("Input format: arrangebands input output pw/vasp (spinpol)\n");
	printf("Optional spinpol is for vasp only. When using spinpol, output name is not used.\n");

	if(argc < 4)
	{
		printf("Please check the input format.\n");
		exit(0);
	}

	printf("Please input E-fermi:\n");
	scanf("%lf", &Efermi);

	fp = fopen(argv[1], "r");
	if( (argc == 5) && (strcmp(argv[4], "spinpol") == 0) && (strcmp(argv[3], "vasp") == 0))
	{
		fp2 = fopen("upbnd.dat", "w");
		fp3 = fopen("dnbnd.dat", "w");
		gnufp2 = fopen("upbnd.dat.gnu", "w");
		gnufp3 = fopen("dnbnd.dat.gnu", "w");
	}
	else
	{
		fp2 = fopen(argv[2], "w");
		sprintf(gnuname, "%s.gnu", argv[2]);
		gnufp2 = fopen(gnuname, "w");
	}

	rl = fopen("reclatt", "r");

	fscanf(rl, "%lf", &blat);
	reclatt = (double**)malloc(3*sizeof(double*));
	for(i=0; i<3; i++)
		reclatt[i] = (double*)malloc(3*sizeof(double));

	for(i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			fscanf(rl, "%lf", &reclatt[i][j]);
			reclatt[i][j] *= blat;
		}
	}

	if(strcmp(argv[3], "pw") == 0)
	{
		while(fgetc(fp) != '=');
		fscanf(fp, "%d", &nbnds);
		while(fgetc(fp) != '=');
		fscanf(fp, "%d", &nkpts);
		while(fgetc(fp) != '\n');
	}
	else if(strcmp(argv[3], "vasp") == 0)
	{
		while(fgetc(fp) != '\n');
		while(fgetc(fp) != '\n');
		while(fgetc(fp) != '\n');
		while(fgetc(fp) != '\n');
		while(fgetc(fp) != '\n');
		fscanf(fp, "%d %d %d", &arb, &nkpts, &nbnds);
		while(fgetc(fp) != '\n');
		while(fgetc(fp) != '\n');
	}
	else
	{
		printf("Not a valid program name.\n");
		exit(0);
	}


	x = 0.0;
	for(i=0; i<nkpts; i++)
	{
		if(i == 0)
		{
			fscanf(fp, "%lf %lf %lf", kdir, kdir+1, kdir+2);
			if(strcmp(argv[3], "vasp") == 0)
				dir2car(kdir, kthis, reclatt);
			else if(strcmp(argv[3], "pw") == 0)
			{
				kthis[0] = kdir[0] * blat;
				kthis[1] = kdir[1] * blat;
				kthis[2] = kdir[2] * blat;
			}
			klast[0] = kthis[0];
			klast[1] = kthis[1];
			klast[2] = kthis[2];
		}
		else
		{
                        klast[0] = kthis[0];
                        klast[1] = kthis[1];
                        klast[2] = kthis[2];
			fscanf(fp, "%lf %lf %lf", kdir, kdir+1, kdir+2);
			if(strcmp(argv[3], "vasp") == 0)
				dir2car(kdir, kthis, reclatt);
			else if(strcmp(argv[3], "pw") == 0)
			{
				kthis[0] = kdir[0] * blat;
				kthis[1] = kdir[1] * blat;
				kthis[2] = kdir[2] * blat;
			}
		}

		if(strcmp(argv[3], "vasp") == 0)
			fscanf(fp, "%lf", &wght);

		x += dist(kthis, klast);
		fprintf(fp2, "%f   ", x);
		if( (argc == 5) && (strcmp(argv[4], "spinpol") == 0) && (strcmp(argv[3], "vasp") == 0))
			fprintf(fp3, "%f   ", x);

		for(j=0; j<nbnds; j++)
		{
			if(strcmp(argv[3], "vasp") == 0)
				fscanf(fp, "%d", &arb);
			fscanf(fp, "%lf", &y);
			if( (argc == 5) && (strcmp(argv[4], "spinpol") == 0) && (strcmp(argv[3], "vasp") == 0))
				fscanf(fp, "%lf", &y2);
			fprintf(fp2, "  %f", y - Efermi);
			fprintf(gnufp2, "%f   %f\n", x, y - Efermi);
			if( (argc == 5) && (strcmp(argv[4], "spinpol") == 0) && (strcmp(argv[3], "vasp") == 0))
			{
				fprintf(fp3, "  %f", y2 - Efermi);
				fprintf(gnufp3, "%f   %f\n", x, y2 - Efermi);
			}
		}
		fprintf(fp2, "\n");
		if( (argc == 5) && (strcmp(argv[4], "spinpol") == 0) && (strcmp(argv[3], "vasp") == 0))
			fprintf(fp3, "\n");
	}

	fclose(rl);
	fclose(fp);
	fclose(fp2);
	fclose(gnufp2);
	if( (argc == 5) && (strcmp(argv[4], "spinpol") == 0) && (strcmp(argv[3], "vasp") == 0))
	{
		fclose(fp3);
		fclose(gnufp3);
	}

	for(i=0; i<3; i++)
		free(reclatt[i]);
	free(reclatt);

	return 0;

}

