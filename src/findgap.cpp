// ALthough band.x gives the order of bands already depends on the wavefunction characters
// We sort w.r.t. energies again here because gaps are only depened on energies orders

#include"findgap.h"

int main(int argc, char **argv)
{
	int ik, ib, jb;
	double dgap, indgap; // direct and indirect gap
	int ind; // index
	int nk, nb, nocc; // nocc is roughly defined, because in metals it's not the real one
	// this nocc is only defined to be nelec/2 or nelec

	double **vals; // vals[nk][nb]
	double *cbm, *vbm; // cbm and vbm are not the real definitions. they are just cbm and vbm at each k from all bands at the same k
	// or precisely, they are cbminimum, vbmaximum, up to nocc
	double *gap; // gap = cbm - vbm

	FILE *fp;
	double dnum;

	printf("Find gap. For pw nosoc (i.e. no degenerate bands) only for now. Read filband (or equivalent) file.\n");
	printf("Usage: findgap filename nocc\n");

	if(argc != 3)
	{
		printf("Please check usage and see instructions in source code.\n");
		exit(0);
	}

	nocc = atoi(argv[2]);

	fp = fopen(argv[1], "r");

	while(fgetc(fp)!= '=');
	fscanf(fp, "%d", &nb);
	while(fgetc(fp)!= '=');
	fscanf(fp, "%d", &nk);
	while(fgetc(fp)!= '\n');

	printf("There are %d kpoints and %d bands.\n", nk, nb);
	printf("There are %d occupied (not rigorously) bands.\n", nocc);


	vals = (double**)malloc(nk*sizeof(double*));
	for(ik=0; ik<nk; ik++)
		vals[ik] = (double*)malloc(nb*sizeof(double));

	cbm = (double*)malloc(nk*sizeof(double));
	vbm = (double*)malloc(nk*sizeof(double));
	gap = (double*)malloc(nk*sizeof(double));


	for(ik=0; ik<nk; ik++)
	{
		fscanf(fp, "%lf %lf %lf", &dnum, &dnum, &dnum);

		for(ib=0; ib<nb; ib++)
			fscanf(fp, "%lf", &vals[ik][ib]);


		for(ib=0; ib<nb; ib++)
		{
			for(jb=ib+1; jb<nb; jb++)
			{
				if(vals[ik][ib] > vals[ik][jb])
				{
					dnum = vals[ik][ib];
					vals[ik][ib] = vals[ik][jb];
					vals[ik][jb] = dnum;
				}
			}
		}

		vbm[ik] = vals[ik][nocc-1];
		cbm[ik] = vals[ik][nocc];
		gap[ik] = cbm[ik] - vbm[ik];
	}

	fclose(fp);


	dgap = findmin(gap, nk, &ind);
	printf("\nDirect gap is  %f  at %dth kpoint.\n", dgap, ind+1);
	printf("CB energy is %f , VB energy is %f .\n", cbm[ind], vbm[ind]);

	indgap = findmin(cbm, nk, &ind);
	printf("\nThroughout BZ, CBM is at %dth kpoint with energy %f .\n", ind+1, cbm[ind]); // here VBM and CBM are real common definitions.

	indgap = indgap - findmax(vbm, nk, &ind);
	printf("Throughout BZ, VBM is at %dth kpoint with energy %f .\n", ind+1, vbm[ind]);

	printf("Indirect gap therefore is %f according to CBM and VBM.\n", indgap);

	printf("\nNotice: if direct and indirect gaps are almost equal, it might be due to symmetry that there are\n");
	printf("some equivalent direct gaps in BZ.\n");
	
	return 0;

}



