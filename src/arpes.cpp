#include"arpes.h"

void arpes()
{
	printf("This program is used to simulate (spin-)ARPES band structures.\n");
	printf("Up to now, matrix element effect is not considered.\n");
	printf("This program should be used after arrangebands utility, i.e. with the format suited for Origin.\n");
	printf("The unit is in bohr^-1 until further change in reclatt.sh.\n");
	printf("This program can be applied to both spin polarized or spin-orbit calculations.\n");
	printf("In spinpol case, it reads upbnd.dat and dnbnd.dat.\n");
	printf("In spin3d case, filband.1(2 and 3) should be post-processed and renamed too.\n");
	printf("Four files will be read: bands.dat, bands.1.dat, bands.2.dat, bands.3.dat.\n");
	printf("A magnetic detection direction n needs to be assigned as three float numbers in spin3d.\n");
	printf("In spinpol case this is not required and spins are considered fully polarized as +1 or -1.\n");
	printf("Data closely around zone boundary may be a little bit away from real values becasue of the lack of periodic data.\n");
	printf("Resolutions are set to default by let kres = 0.0 and eres = 0.0.\n");

	printf("\n\n");

	int nk, nb, nkres, neres;
	double n1, n2, n3, nmod;
	double kres, eres;
	int ik, ib, ikres, ieres;
	double kmin, kmax, emin, emax; // output range
	double kcut, ecut; // avoid calculating too many gaussians
	int mode; // 1 for unpolarized, 2 for spinpol, 3 for spin3d
	int cutcb, iblist; // cut conduction bands; if use band list
	int listk, listb, listinc; // in the band list file, the k, band, and include or not

// spinpol: 0 for up, 1 for down; spin3d: 0 for bands, 1-3 for spin <Sx>, <Sy>, <Sz>
	double *kpaths;
	double **data0, **data1, **data2, **data3;
	FILE *file0, *file1, *file2, *file3;
	FILE *outdat, *outmm; // outdat: Origin plotting, outmm: Mathematica plotting
	FILE *bandlist;

	double **energies, **spins; // Before smearing, mode=1,2,3 should end in these two matrices[nk][nb]
	double **arpesdata; // The matrix stores resolutions, the final spin integrated results, arpesdata[nkres][neres]
	double *arpesk, *arpese; // two matrices store the corresponding k's and energies, which are evenly distributed

	printf("Please choose modes: 1 for unpolarized, 2 for spinpol, 3 for spin3d:\n");
	scanf("%d", &mode);
	if(mode != 1 && mode != 2 && mode != 3)
	{
		printf("mode can only be 1 or 2 or 3.\n");
		exit(0);
	}
	printf("Please input kres (in bohr^-1) and eres (in eV). 0.0  0.0 for default values:\n");
	scanf("%lf %lf", &kres, &eres);

	if(kres == 0.0)
		kres = 0.01*0.53; // 0.01 eV^-1 converted to bohr^-1, default kres = 0.01*0.53
	if(eres == 0.0)
		eres = 0.015; // two default values are for Lanzara group (spin-)ARPES, default eres= 0.015

	printf("Please input the lower and upper limit of k, based on your data:\n");
	scanf("%lf %lf", &kmin, &kmax);

	printf("Please input the energy range you want to plot, emin and emax:\n");
	scanf("%lf %lf", &emin, &emax);

	printf("Please input nk and nb in filband or either upbnd or dnbnd:\n");
	scanf("%d %d", &nk, &nb);

	printf("Do you want to cut (exclude) conduction bands? 0 for no cut, 1 for cut\n");
	scanf("%d", &cutcb);

	if(mode == 3)
	{
		printf("For spin3d case, please input the measured spin direction in float numbers n = (n1, n2, n3):\n");
		scanf("%lf %lf %lf", &n1, &n2, &n3);
	}

	iblist = 0;
	if(mode == 2 || mode == 3)
	{
		printf("Do you want to use only certain bands? 1 for yes with bandlist.dat file, 0 for no:\n");
		scanf("%d", &iblist);
	}

	if(iblist == 1)
		bandlist = fopen("bandlist.dat", "r");

	kcut = 3*kres; // normal distribution with 99.7% contribution
	ecut = 3*eres;

	nkres = floor((kmax-kmin)/kres);
	neres = floor((emax-emin)/eres);

	if(nkres <= 0 || neres <= 0)
	{
		printf("nkres or neres is negative. Please check input\n");
		exit(0);
	}

	if(nkres <= nk)
		printf("It is better to have nk > nkres to generate meaningful results. Program continues anyway.\n");

// read bandstructures data
	kpaths = (double*)malloc(nk*sizeof(double));
	data0 = (double**)malloc(nk*sizeof(double*));
	if(mode == 2 || mode == 3)
		data1 = (double**)malloc(nk*sizeof(double*));
	if(mode == 3)
	{
		data2 = (double**)malloc(nk*sizeof(double*));
		data3 = (double**)malloc(nk*sizeof(double*));
	}

	for(ik=0; ik<nk; ik++)
	{
		data0[ik] = (double*)malloc(nb*sizeof(double));
		if(mode == 2 || mode == 3)
			data1[ik] = (double*)malloc(nb*sizeof(double));
		if(mode == 3)
		{
			data2[ik] = (double*)malloc(nb*sizeof(double));
			data3[ik] = (double*)malloc(nb*sizeof(double));
		}
	}

	if(mode == 1)
		file0 = fopen("bands.dat", "r");
	if(mode == 2)
	{
		file0 = fopen("upbnd.dat", "r");
		file1 = fopen("dnbnd.dat", "r");
	}
	if(mode == 3)
	{
		file0 = fopen("bands.dat", "r");
		file1 = fopen("bands.1.dat", "r");
		file2 = fopen("bands.2.dat", "r");
		file3 = fopen("bands.3.dat", "r");
	}

	for(ik=0; ik<nk; ik++)
	{
		fscanf(file0, "%lf", &(kpaths[ik]));
		if(mode == 2 || mode == 3)
			fscanf(file1, "%lf", &kpaths[ik]);
		if(mode == 3)
		{
			fscanf(file2, "%lf", &kpaths[ik]);
			fscanf(file3, "%lf", &kpaths[ik]);
		}
		for(ib=0; ib<nb; ib++)
		{
			fscanf(file0, "%lf", &data0[ik][ib]);
			if(mode == 2 || mode == 3)
				fscanf(file1, "%lf", &data1[ik][ib]);
			if(mode == 3)
			{
				fscanf(file2, "%lf", &data2[ik][ib]);
				fscanf(file3, "%lf", &data3[ik][ib]);
			}

		}
	}

	fclose(file0);
	if(mode == 2 || mode == 3)
		fclose(file1);
	if(mode == 3)
	{
		fclose(file2);
		fclose(file3);
	}

// Now construct energies and spins matrices for three modes
	energies = (double**)malloc(nk*sizeof(double*));
	spins = (double**)malloc(nk*sizeof(double*));
	for(ik=0; ik<nk; ik++)
	{
		if(mode == 2)
		{
			energies[ik] = (double*)malloc(2*nb*sizeof(double));
			spins[ik] = (double*)malloc(2*nb*sizeof(double));
		}
		else
		{
			energies[ik] = (double*)malloc(nb*sizeof(double));
			spins[ik] = (double*)malloc(nb*sizeof(double));
		}
	}

//distributing energies, for spinpol, at each k, number of energies (bands) are doubled
	for(ik=0; ik<nk; ik++)
	{
		for(ib=0; ib<nb; ib++)
		{
			energies[ik][ib] = data0[ik][ib];
			if(mode == 2)
				energies[ik][ib+nb] = data1[ik][ib];
		}
	}

// constructing spins for unpolarized
	if(mode == 1)
	{
		for(ik=0; ik<nk; ik++)
			for(ib=0; ib<nb; ib++)
				spins[ik][ib] = 1.0;
	}

// constructing spins for spinpol
	if(mode == 2)
	{
		for(ik=0; ik<nk; ik++)
		{
			for(ib=0; ib<nb; ib++)
			{
				spins[ik][ib] = 1.0;
				spins[ik][ib+nb] = -1.0;
			}
		}

		nb *= 2; // for spinpol case, after constructing the enregies and spins, nb can be set to its real value
	}

// constructing spins for spin3d, spins = (Sx, Sy, Sz) dot (n1, n2, n3)
	if(mode == 3)
	{
		nmod = sqrt(n1*n1+n2*n2+n3*n3);
		n1 /= nmod;
		n2 /= nmod;
		n3 /= nmod;

		for(ik=0; ik<nk; ik++)
		{
			for(ib=0; ib<nb; ib++)
			{
				spins[ik][ib] = n1*data1[ik][ib] + n2*data2[ik][ib] + n3*data3[ik][ib];
			}
		}
	}

// modifying spins if iblist is true
	if(iblist == 1)
	{
		for(ik=0; ik<nk; ik++)
		{
			for(ib=0; ib<nb; ib++)
			{
				fscanf(bandlist, "%d %d %d", &listk, &listb, &listinc);
				if(listk != ik+1 || listb != ib+1)
				{
					printf("bandlist.dat file read error.\n");
					exit(0);
				}
				if(listinc == 0)
					spins[ik][ib] = 0.0;
			}
		}
	}

	if(iblist == 1)
		fclose(bandlist);

// if cutcb is true, set spins with energies>0 to be zeros, i.e. no contribution in the gaussian smearing
	if(cutcb == 1)
		for(ik=0; ik<nk; ik++)
			for(ib=0; ib<nb; ib++)
				if(energies[ik][ib] > 0.0)
					spins[ik][ib] = 0.0;


// free the memories for input data
	for(ik=0; ik<nk; ik++)
	{
		free(data0[ik]);
		if(mode == 2 || mode == 3)
			free(data1[ik]);
		if(mode == 3)
		{
			free(data2[ik]);
			free(data3[ik]);
		}
	}
	free(data0);
	if(mode == 2 || mode == 3)
		free(data1);
	if(mode == 3)
	{
		free(data2);
		free(data3);
	}


// Now for each mode, we have energies and spins matrices
// We will construct the simulated (spin-)ARPES bandstructures
// The results will be stored in arpesdata (accumulated spin gaussian-weighted values), arpesk (x-coordinates) and arpese (y-coordinates)
// arpesk (x) and arpese (y) will be smeared from the energies, and each point (arpesk, arpese) has a value arpesdata
// the arpesdata is derived from by adding up the spins from all input grids, weighted by gaussians (we take a cutoff distance for gaussians)

// evaluating arpesk and arpese
	arpesk = (double*)malloc(nkres*sizeof(double));
	arpese = (double*)malloc(neres*sizeof(double));
	arpesdata = (double**)malloc(nkres*sizeof(double*));
	for(ikres=0; ikres<nkres; ikres++)
		arpesdata[ikres] = (double*)malloc(neres*sizeof(double));

	for(ikres=0; ikres<nkres; ikres++)
		arpesk[ikres] = kres * (0.5 + ikres) + kmin;

	for(ieres=0; ieres<neres; ieres++)
		arpese[ieres] = eres * (0.5 + ieres) + emin;

// initialize arpesdata = 0
	for(ikres=0; ikres<nkres; ikres++)
		for(ieres=0; ieres<neres; ieres++)
			arpesdata[ikres][ieres] = 0.0;

	printf("Starting calculating (spin-)ARPES data...\n");
// looping 
	for(ikres=0; ikres<nkres; ikres++)
	{
		for(ieres=0; ieres<neres; ieres++)
		{
			for(ik=0; ik<nk; ik++)
			{
				if(fabs(arpesk[ikres] - kpaths[ik]) < kcut)
				{
					for(ib=0; ib<nb; ib++)
					{
						if(fabs(arpese[ieres] - energies[ik][ib]) < ecut)
						{
							arpesdata[ikres][ieres] += \
							spins[ik][ib] * gaussian(kpaths[ik], arpesk[ikres], kres) * gaussian(energies[ik][ib], arpese[ieres], eres);
						}
					}
				}
			}
		}
	}

// writing output files
	outdat = fopen("arpes.dat", "w");
	outmm = fopen("arpes.mm", "w");

	// Mathematica ListDensityPlot with InterpolationOrder -> 0
	fprintf(outmm, "ListDensityPlot[{");

	for(ikres=0; ikres<nkres; ikres++)
	{
		for(ieres=0; ieres<neres; ieres++)
		{
			// Origin XYZ three columns for 2D plotting
			fprintf(outdat, "%f %f %f\n", arpesk[ikres], arpese[ieres], arpesdata[ikres][ieres]);
			// Mathematica
			fprintf(outmm, "{%f, %f, %f}", arpesk[ikres], arpese[ieres], arpesdata[ikres][ieres]);
			if(ikres != nkres-1 || ieres != neres-1)
				fprintf(outmm, ",");
		}
	}

	fprintf(outmm, "}, Mesh->None, InterpolationOrder->0, ColorFunction->\"SouthwestColors\"]");

	fclose(outdat);
	fclose(outmm);

// free arrays
	free(kpaths);
	for(ik=0; ik<nk; ik++)
	{
		free(energies[ik]);
		free(spins[ik]);
	}
	free(energies);
	free(spins);

	free(arpesk);
	free(arpese);
	for(ikres=0; ikres<nkres; ikres++)
		free(arpesdata[ikres]);
	free(arpesdata);

	printf("Done!\n");

	return;
}

int main()
{
	arpes();

	return 0;
}
