// This program is used to plot bandstructures with projected 
// density of states or wavefunction components
// Read filband and filproj
// In filband (actually bands.dat so no need to read reclatt), MUST set no_overlap = .true.
// In filproj, values are actually coef^2 of wavefunctions
//
// Output format will be five colomns
// kpts band_energy some_intensity inten_1 inten_2
//
// Now only designed for continuously choosing few states
// Some combinations like only px-orbitals can be done with making some easy scripts

#include"projbands.h"

int main(int argc, char **argv)
{
	int nk, nb, ik, ib;
	int ns, is; // n total states, ith total states
	int ng, ig; // n groups, can only be 1 or 2
	int natom, ntyp;
	int tmpint;
	int i, j, k;

// for eqp corrections
	int eqp = 0;
	int startqp, endqp, iqp, nqp; // starting and ending qp bands
	double **eqpmf, **eqpqp;
	double epsmall = 0.001;
	double efmf, efqp; // Fermi levels
////////////////////////

	int start1, end1;
	int start2, end2;

	double **sump1, **sump2;
	double **bands;
	double *kpts;
	double tmpdbl;
	
	FILE *fbands, *fproj;
	FILE *fin;
	FILE *fpbands;

// for eqp corrections
	FILE *feqp, *fpqp;
//////////////////////


	printf("This program is used to plot decomposed band structures.\n");
	printf("Required inputs: filproj, bands.dat (which comes from filband).\n");
	printf("A input file pbands.in is also required.\n");
	printf("One or two components can be studied at one time. For more, you should use scripts.\n");
	printf("The 2nd component values will be set negative.\n");
	printf("If using eqp, then the file eqp.dat should present.\n");
	printf("Please check source codes for more details.\n\n");

	printf("Usage:  projbands (eqp)\n");


	if(argc > 2)
	{
		printf("Please check usage.\n");
		exit(0);
	}

// eqp
	if(argc == 2)
	{
		if(strcmp(argv[1], "eqp") == 0 )
		{
			eqp = 1; // set use_eqp = .true.
			printf("\nIncluding eqp corrections...\n");
		}
	}

	nqp = 0;

	if(eqp == 1)
	{
		printf("Starting # of band and ending # of band in eqp.dat file are:\n");
		scanf("%d  %d", &startqp, &endqp);

		nqp = endqp - startqp + 1;

		printf("\nThere are %d qp bands.\n", nqp);

		printf("\nFermi levels for MF and QP bands are:\n");
		scanf("%lf %lf", &efmf, &efqp);
	}
///////////////////////////

	fproj = fopen("filproj", "r");

	fscanf(fproj, "%d %d %d %d %d %d %d %d", &tmpint, &tmpint, &tmpint, &tmpint, &tmpint, &tmpint, &natom, &ntyp);

	for(i=0; i<6; i++)
		while(fgetc(fproj)!='\n');  // skip 6 lines

	for(i=0; i<ntyp; i++)
		while(fgetc(fproj)!='\n'); // skip ntyp lines
	
	for(i=0; i<natom; i++)
		while(fgetc(fproj)!='\n'); // skip natom lines

	fscanf(fproj, "%d %d %d", &ns, &nk, &nb);
	while(fgetc(fproj)!='\n');
	while(fgetc(fproj)!='\n'); // skip lines

// Finish reading all parameters. 
// Now starting allocate memories and do reading data

	fin = fopen("pbands.in", "r");
	fscanf(fin, "%d", &ng);
	while(fgetc(fin)!='\n');
	fscanf(fin, "%d %d", &start1, &end1);
	while(fgetc(fin)!='\n');
	if(ng == 2)
		fscanf(fin, "%d %d", &start2, &end2);
	fclose(fin);


	sump1 = (double**)malloc(nk*sizeof(double*));
	sump2 = (double**)malloc(nk*sizeof(double*));
	bands = (double**)malloc(nk*sizeof(double*));

	kpts = (double*)malloc(nk*sizeof(double));

	for(ik=0; ik<nk; ik++)
	{
		sump1[ik] = (double*)malloc(nb*sizeof(double));
		sump2[ik] = (double*)malloc(nb*sizeof(double));
		bands[ik] = (double*)malloc(nb*sizeof(double));

		for(ib=0; ib<nb; ib++)
		{
			sump1[ik][ib] = 0.0;
			sump2[ik][ib] = 0.0;
			bands[ik][ib] = 0.0;
		}
	}

//////////////// eqp ////////////////////
	if(eqp == 1)
	{
		eqpmf = (double**)malloc(nk*sizeof(double*));
		eqpqp = (double**)malloc(nk*sizeof(double*));

		for(ik=0; ik<nk; ik++)
		{
			eqpmf[ik] = (double*)malloc(nqp*sizeof(double));
			eqpqp[ik] = (double*)malloc(nqp*sizeof(double));
		}
	}
/////////////////////////////////////////


// reading band energies
	fbands = fopen("bands.dat", "r");
	for(ik=0; ik<nk; ik++)
	{
		fscanf(fbands, "%lf", &kpts[ik]);

		for(ib=0; ib<nb; ib++)
			fscanf(fbands, "%lf", &bands[ik][ib]);
	}
	fclose(fbands);

//////////////////// eqp reading /////////////////////
	if(eqp == 1)
	{
		feqp = fopen("eqp.dat", "r");
		for(ik=0; ik<nk; ik++)
		{
			while(fgetc(feqp)!='\n');

			for(iqp=0; iqp<nqp; iqp++)
			{
				fscanf(feqp, "%d %d %lf %lf", &tmpint, &tmpint, &eqpmf[ik][iqp], &eqpqp[ik][iqp]);

				eqpmf[ik][iqp] -= efmf;
				eqpqp[ik][iqp] -= efqp;

				while(fgetc(feqp)!='\n');
			}
		}

		fclose(feqp);
	}
////////////////////////////////////////////////////////////


// now reading filproj data

	if(ng == 1)
	{
		start2 = ns + 10;
		end2 = start2 + 10;
	} // Therefore the data for group 2 will always be 0, because program will not read at all

	start1 = start1 - 1;
	end1 = end1 - 1;
	start2 = start2 - 1;
	end2 = end2 -1;
	// Matching C standard that array index starts from 0


	for(is=0; is<ns; is++)
	{
		while(fgetc(fproj)!='\n'); // skip the info line

		for(ik=0; ik<nk; ik++)
		{
			for(ib=0; ib<nb; ib++)
			{
				fscanf(fproj, "%d %d %lf", &tmpint, &tmpint, &tmpdbl);
				while(fgetc(fproj)!='\n');

				if( is >= start1 && is <= end1 )
					sump1[ik][ib] += tmpdbl;
				else if( is>= start2 && is <= end2 )
					sump2[ik][ib] += tmpdbl;
			}
		}
	}

	fclose(fproj);

// finish reading

// now start writing

	fpbands = fopen("pbands.dat", "w");

/////////// eqp /////////////
	if(eqp == 1)
	{
		fpqp = fopen("pbands_eqp.dat", "w");
		iqp = 0;          // initializing for later loop
	}

/////////////////////////////

	for(ib=0; ib<nb; ib++)
	{
		for(ik=0; ik<nk; ik++)
			fprintf(fpbands, "%lf   %lf   %lf   %lf   %lf \n", kpts[ik], bands[ik][ib], sump1[ik][ib]-sump2[ik][ib], sump1[ik][ib], -sump2[ik][ib]);

		fprintf(fpbands, "\n");

///////////////////////////// eqp writing ///////////////////////////////////////////
		if(eqp == 1)
		{
			if(ib >= startqp-1 && ib <= endqp-1)
			{
				for(ik=0; ik<nk; ik++)
				{
					if( fabs(bands[ik][ib] - eqpmf[ik][iqp]) > epsmall)
					{
						printf("DFT energies in bands.dat and eqp.dat are not consistent.\n");
						printf("ik, ib, iqp = %d, %d, %d\n", ik, ib, iqp);
						printf("Two energies are  %.9f   %.9f\n\n", bands[ik][ib], eqpmf[ik][iqp]);
						exit(0);
					}

					fprintf(fpqp, "%lf   %lf   %lf   %lf   %lf \n", kpts[ik], eqpqp[ik][iqp], sump1[ik][ib]-sump2[ik][ib], sump1[ik][ib], -sump2[ik][ib]);
				}

				fprintf(fpqp, "\n");

				iqp ++;
			}
		}
/////////////////////////////////////////////////////////////////////////////////////

	}

	fclose(fpbands);

/////////////// eqp  ///
	if(eqp == 1)
		fclose(fpqp);
///////////////////////


	for(ik=0; ik<nk; ik++)
	{
		free(sump1[ik]);
		free(sump2[ik]);
		free(bands[ik]);
// eqp
		if(eqp == 1)
		{
			free(eqpmf[ik]);
			free(eqpqp[ik]);
		}
///////////
	}

	free(sump1);
	free(sump2);
	free(bands);
//eqp
	if(eqp == 1)
	{
		free(eqpmf);
		free(eqpqp);
	}
///////////////////////

	return 0;
}


