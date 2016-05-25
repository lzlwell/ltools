#include"decompwfc.h"

void decompwfc(int nthb)
{
	int nkpts, nbnds, nsts;
	int ik, ib, is;
	int ink, inb;
	int skip;  // how many lines to skip
	int i, j;
	int nslct, islct; // selected states
	int *slct;
	double **kpts, **kcar;
	double *vals, val;  // store and accumulate values at each kpoint and selected band
	int isst;
	double blat;
	double **reclatt;

	FILE *proj, *in, *out, *mm, *rl;

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
	fclose(rl);

	proj = fopen("filproj", "r");
	in = fopen("decomp.in", "r");
	out = fopen("decomp.out", "w");
	mm = fopen("decomp.mm", "w");

	fscanf(in, "%d", &skip);
	while(fgetc(in)!='\n');
	fscanf(in, "%d", &nslct);
	while(fgetc(in)!='\n');

	for(i=0; i<skip; i++)
		while(fgetc(proj)!='\n');

	fscanf(proj, "%d %d %d", &nsts, &nkpts, &nbnds);
	while(fgetc(proj)!='\n');
	while(fgetc(proj)!='\n');

	slct = (int*)malloc(nslct*sizeof(int));
	vals = (double*)malloc(nkpts*sizeof(double));
	kpts = (double**)malloc(nkpts*sizeof(double*));
	kcar = (double**)malloc(nkpts*sizeof(double*));
	for(ik=0; ik<nkpts; ik++)
	{
		vals[ik] = 0.0;
		kpts[ik] = (double*)malloc(3*sizeof(double));
		kcar[ik] = (double*)malloc(3*sizeof(double));
	}

	for(i=0; i<nslct; i++)
		fscanf(in, "%d", slct+i);
	while(fgetc(in)!='\n');
	while(fgetc(in)!='\n');
	while(fgetc(in)!='\n');
	for(ik=0; ik<nkpts; ik++)
	{
		fscanf(in, "%lf %lf %lf", &kpts[ik][0], &kpts[ik][1], &kpts[ik][2]);
		while(fgetc(in)!='\n');
		dir2car(kpts[ik], kcar[ik], reclatt);
	}

	fclose(in);

	for(is=0; is<nsts; is++)
	{
		isst=0;
		for(islct=0; islct<nslct; islct++)
		{
			if(slct[islct] == is + 1)  // is runs starting from 0
			{
				isst = 1;
				break;
			}
		}

		if(isst == 0)
		{
			for(i=0; i< nkpts*nbnds+1; i++)  // one more state info line
				while(fgetc(proj) != '\n');
		}

		else if(isst == 1)
		{
			while(fgetc(proj) != '\n');  // skip the info line
			for(ik=0; ik<nkpts; ik++)
			{
				for(ib=0; ib<nbnds; ib++)
				{
					if(nthb == ib+1)
					{
						fscanf(proj, "%d %d %lf", &ink, &inb, &val);
						if((ink != ik+1) || (inb != ib+1))
						{
							printf("Kpoints or bands not matching!\n");
							exit(0);
						}
					}
					while(fgetc(proj) != '\n');
				}
				vals[ik] += val;
			}
		}
	}

	for(ik=0; ik<nkpts; ik++)
	{
		fprintf(out, "%f %f %f\n", kcar[ik][0], kcar[ik][1], vals[ik]);
		fprintf(mm, "{%f, %f, %f}, ", kcar[ik][0], kcar[ik][1], vals[ik]);
	}

	fclose(proj);
	fclose(out);
	fclose(mm);

	for(ik=0; ik<nkpts; ik++)
	{
		free(kpts[ik]);
		free(kcar[ik]);
	}

	free(kpts);
	free(kcar);
	free(vals);
	free(reclatt);
	free(slct);

	return;
}


int main(int argc, char** argv)
{
	printf("Please run reclatt.sh first.\n");

	if(argc == 2)
		decompwfc(atoi(argv[1]));
	else
		printf("Please chack input format:\ndecompwfc nthb\n");

	return 0;
}

