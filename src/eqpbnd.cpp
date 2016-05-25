#include"eqpbnd.h"

int main(int argc, char **argv)
{
	const double eps = 0.001;
	int nkpts, nbnds, nkmesh; // nkpts: # of kpts in bandstructure, nbnds, nkmesh: # kpts of fine kgrid
	double kdir[3], kcar[3], kpt[3], klast[3], emf, eqp, efmf, efqp;
	double blat, **reclatt;
	double x;
	FILE *kpaths;
	FILE *bndstr;
	FILE *mf;
	FILE *qp;
	FILE *rl;
	int i, j, ik, ib;
	int spin, band;
	double de;

	printf("Please run reclatt.sh first to get reclatt file.\n");
	printf("Input format: eqpbnd nkpts nbnd nkmesh efmf efqp\n");
	printf("Required input files include:\n");
	printf("kpaths.out (in pw format), bandstructure.dat\n");
	printf("Output file contains:\n");
	printf("mfbnd.dat, qpbnd.dat\n");
	if(argc != 6)
	{
		printf("Please check input format.\n");
		exit(0);
	}

	nkpts = atoi(argv[1]);
	nbnds = atoi(argv[2]);
	nkmesh = atoi(argv[3]);
	efmf = atof(argv[4]);
	efqp = atof(argv[5]);

	reclatt = (double**)malloc(3*sizeof(double*));
	for(i=0; i<3; i++)
		reclatt[i] = (double*)malloc(3*sizeof(double));

	rl = fopen("reclatt", "r");
	fscanf(rl, "%lf", &blat);
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			fscanf(rl, "%lf", &reclatt[i][j]);
			reclatt[i][j] *= blat;
		}
	}
	fclose(rl);

	kpaths = fopen("kpaths.out", "r");
	mf = fopen("mfbnd.dat", "w");
	qp = fopen("qpbnd.dat", "w");

	while(fgetc(kpaths)!= '\n');
	while(fgetc(kpaths)!= '\n');

	x = 0.0;
	for(ik=0; ik<nkpts; ik++)
	{
		printf("Dealing with %d/%d ...\n", ik, nkpts);
		if(ik==0)
		{
			fscanf(kpaths, "%lf %lf %lf", kdir, kdir+1, kdir+2);
			dir2car(kdir, kcar, reclatt);
			while(fgetc(kpaths)!= '\n');
			klast[0] = kcar[0];
			klast[1] = kcar[1];
			klast[2] = kcar[2];
		}
		else
		{
			klast[0] = kcar[0];
			klast[1] = kcar[1];
			klast[2] = kcar[2];
			fscanf(kpaths, "%lf %lf %lf", kdir, kdir+1, kdir+2);
			dir2car(kdir, kcar, reclatt);
			while(fgetc(kpaths)!= '\n');
		}
		x += dist(kcar, klast);

		fprintf(mf, "%f  ", x);
		fprintf(qp, "%f  ", x);

		bndstr = fopen("bandstructure.dat", "r");
		while(fgetc(bndstr)!= '\n');
		while(fgetc(bndstr)!= '\n');

		ib = 0;
		for(i=0; i < nkmesh*nbnds; i++)
		{
			fscanf(bndstr, "%d %d %lf %lf %lf %lf %lf %lf", &spin, &band, kpt, kpt+1, kpt+2, &emf, &eqp, &de);
//			printf("%d %d %f %f %f %f %f %f\n", spin, band, kpt[0], kpt[1], kpt[2], emf, eqp, de);
			if((fabs(kcar[0]-kpt[0]) < eps)  &&  (fabs(kcar[1]-kpt[1]) < eps) && (fabs(kcar[2]-kpt[2]) < eps) )
			{
				ib++;
//	printf("kcar %f %f %f   kpt %f %f %f\n", kcar[0], kcar[1], kcar[2], kpt[0], kpt[1], kpt[2]);
				fprintf(mf, " %f", emf - efmf);
				fprintf(qp, " %f", eqp - efqp);
			}
		}
		fprintf(mf, "\n");
		fprintf(qp, "\n");

		fclose(bndstr);

		if(ib != nbnds)
		{
			printf("kpoint (%f, %f, %f) in two input files don't match\n", kcar[0], kcar[1], kcar[2]);
			printf("Input # of bands is %d, found %d in bandstructure.dat.\n", nbnds, ib);
			exit(0);
		}


	}

	fclose(kpaths);
	fclose(mf);
	fclose(qp);

	for(i=0; i<3; i++)
		free(reclatt[i]);
	free(reclatt);

	return 0;
}

