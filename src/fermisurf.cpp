#include"fermisurf.h"

// This program generates a Fermi surface plot file read by XCrySDen.

// shell is used to generate a ganeral grid from a periodic grid, see:
// http://www.xcrysden.org/doc/XSF.html
// General vs. Periodic grids
// Baiscally it adds a shell out of the matrix then stretch to 1D
// to accomodate the format used by XCrySDen
//
// p: periodic, g: general

void shell(double *pval, double *gval, int n1, int n2, int n3) // input n1, n2, n3 are dimensions of pval
{
	int i, j, k;
	int ip, ig;

	double ***pgrid, ***ggrid;

	pgrid = (double***)malloc(n1*sizeof(double**));
	for(i=0; i<n1; i++)
	{
		pgrid[i] = (double**)malloc(n2*sizeof(double*));
		for(j=0; j<n2; j++)
			pgrid[i][j] = (double*)malloc(n3*sizeof(double));
	}

	ggrid = (double***)malloc((n1+1)*sizeof(double**));
	for(i=0; i<n1+1; i++)
	{
		ggrid[i] = (double**)malloc((n2+1)*sizeof(double*));
		for(j=0; j<n2+1; j++)
			ggrid[i][j] = (double*)malloc((n3+1)*sizeof(double));
	}

	ip = 0;
	//decompose pval
	for(i=0; i<n1; i++)
	{
		for(j=0; j<n2; j++)
		{
			for(k=0; k<n3; k++)
			{
				pgrid[i][j][k] = pval[ip]; //values inside a bandgrid are specified in row-major (i.e. C) order
				ip++;
			}
		}
	}

	// start shell step
	for(i=0; i<n1; i++)
	{
		for(j=0; j<n2; j++)
		{
			for(k=0; k<n3; k++)
				ggrid[i][j][k] = pgrid[i][j][k];
			ggrid[i][j][n3] = pgrid[i][j][0];
		}

		for(k=0; k<n3+1; k++)
			ggrid[i][n2][k] = ggrid[i][0][k];
	}

	for(j=0; j<n2+1; j++)
		for(k=0; k<n3+1; k++)
			ggrid[n1][j][k] = ggrid[0][j][k];
	// finish shell step
	
	ig = 0;
	// construct fval
	for(i=0; i<n1+1; i++)
	{
		for(j=0; j<n2+1; j++)
		{
			for(k=0; k<n3+1; k++)
			{
				gval[ig] = ggrid[i][j][k];
				ig++;
			}
		}
	}

	for(i=0; i<n1; i++)
	{
		for(j=0; j<n2; j++)
		{
			free(pgrid[i][j]);
			free(ggrid[i][j]);
		}
		free(ggrid[i][n2]);

		free(pgrid[i]);
		free(ggrid[i]);
	}
	free(ggrid[n1]);

	free(pgrid);
	free(ggrid);

	return;
}

void oneband(double **vout, double **vone, int nb, int nk, double efermi) // vout[nb][nk+shell], vone[1][nk+shell], nk = nk+shell
{
	int ib, ik;
	double ediff;

	for(ik=0; ik<nk; ik++)
	{
		ediff = fabs(vout[0][ik] - efermi);
		vone[0][ik] = vout[0][ik];

		for(ib=1; ib<nb; ib++)
		{
			if(fabs(vout[ib][ik] - efermi) < ediff)
			{
				ediff = fabs(vout[ib][ik] - efermi);
				vone[0][ik] = vout[ib][ik];

			}
		}
	}

	return;
}

void wbxsf(char *name, double efermi, int n1, int n2, int n3, int begin, int end, int nk, double **rlat, double **vout, int idhva)
{
	int i, j;
	int ik, ib;
	FILE *bxsf;
	double ev2ry = 0.073498618;
	double bohrinv2Ainv = 1.889725989;

	FILE *mm; // for mathematica ListContourPlot3D
	FILE *ml; // for matlab
	char mmname[100];
	int imm, jmm, kmm;
	double dk[3], ck[3];
	double **invlat;
	int count;

	if(idhva != 1)
	{
		ev2ry = 1.0;
		bohrinv2Ainv = 1.0;
	}

	bohrinv2Ainv = 1.0;


// Begin writing bxsf file
	bxsf = fopen(name, "w");
	fprintf(bxsf, "BEGIN_INFO\n");
	fprintf(bxsf, "  # This is the 3D Fermi surface plot generated from fermisurf in ltools package.\n");
	fprintf(bxsf, "  Fermi Energy: %f\n", efermi*ev2ry);
	fprintf(bxsf, "END_INFO\n");
	fprintf(bxsf, "\n");
	fprintf(bxsf, "BEGIN_BLOCK_BANDGRID_3D\n");
	fprintf(bxsf, "  this_is_a_one_word_comment_line\n");
	fprintf(bxsf, "  BEGIN_BANDGRID_3D\n");
	fprintf(bxsf, "    %d\n", end-begin+1);
	fprintf(bxsf, "    %d %d %d\n", n1, n2, n3);
	fprintf(bxsf, "    0.0 0.0 0.0\n"); // origin of kgrid
	for(i=0; i<3; i++)
		fprintf(bxsf, "    %f %f %f\n", rlat[i][0]*bohrinv2Ainv, rlat[i][1]*bohrinv2Ainv, rlat[i][2]*bohrinv2Ainv);
	for(ib=begin-1; ib<end; ib++)
	{
		fprintf(bxsf, "    BAND:   %d\n", ib+1);

		i=0;
		j=0;
		for(ik=0; ik<nk; ik++)
		{
			fprintf(bxsf, "    %f", vout[ib][ik]*ev2ry);
			i++;
			if(i==6)   // This line controls how many data to put in one line, default n3
			{
				j++;
				fprintf(bxsf, "\n");
				i=0;
			}
			if(j==-1)   // This line controls where to have a blank line between those lines, default n2
			{
				fprintf(bxsf, "\n");
				j=0;
			}
		}

		sprintf(mmname, "%s-%d.mm", name, ib+1);
		mm=fopen(mmname, "w");
		fprintf(mm, "ListContourPlot3D[{");
		count=0;
		for(imm=0; imm<n1; imm++)  // note: shell step has been performed
		{
			for(jmm=0; jmm<n2; jmm++)
			{
				for(kmm=0; kmm<n3; kmm++)
				{
					dk[0] = ((double)imm)/(n1-1); // note that this is general grid not pbc
					dk[1] = ((double)jmm)/(n2-1);
					dk[2] = ((double)kmm)/(n3-1);
					dir2car(dk, ck, rlat);

					fprintf(mm, "{%f,%f,%f,%f}",ck[0],ck[1],ck[2],vout[ib][count]);
					count++;

					if(count != nk)
						fprintf(mm, ",");
				}
			}
		}

		fprintf(mm, "}, Contours->{%f}, Mesh->None]",efermi);

		fclose(mm);

	}
	fprintf(bxsf, "\n  END_BANDGRID_3D\n");
	fprintf(bxsf, "END_BLOCK_BANDGRID_3D\n");
	fclose(bxsf);
// End writing bxsf file

	return;
}

void sortbands(double **vals, int nb, int nk) //vals[nb][nk]
{
	int ib, ib2, ik;
	double ex; // used in exchange

	for(ik=0; ik<nk; ik++)
	{
		for(ib=0; ib<nb; ib++)
		{
			for(ib2=ib+1; ib2<nb; ib2++)
			{
				if(vals[ib][ik] > vals[ib2][ik])
				{
					ex = vals[ib][ik];
					vals[ib][ik] = vals[ib2][ik];
					vals[ib2][ik] = ex;
				}
			}
		}
	
	}

	return;
}


int main(int argc, char** argv)
{
	int nb, nk;
	int ib, ik, i, j;
	int begin, end;
	int n1, n2, n3;
	double efermi, blat, **rlat;
	double **vals, **vout, **vone; // vals[nb][nk], vout[nb][nk+shell], vone[1][nk+shell]
	char name[30];
	int pwvasp; // =1 for pw, =2 for vasp
	int anum; // a number used for any purposes
	int idhva = 0;

	
	FILE *bands, *bxsf;
	FILE *rl;

	rlat = (double**)malloc(3*sizeof(double*));
	for(i=0; i<3; i++)
		rlat[i] = (double*)malloc(3*sizeof(double));

	printf("Please run reclatt.sh first.\n\n");
	printf("Usage: fermisurf filband/EIGENVAL pw/vasp (dhva)\n");

	if(argc != 3 && argc != 4)
	{
		printf("Please check input format.\n");
		exit(0);
	}

	if(strcmp(argv[2], "pw") == 0)
		pwvasp = 1;
	else if(strcmp(argv[2], "vasp") == 0)
		pwvasp = 2;
	else
		pwvasp = 0;

	if(argc == 4)
		if(strcmp(argv[3], "dhva") == 0)
			idhva = 1;


	if(pwvasp == 0)
	{
		printf("Wrong DFT code name. It should be pw or vasp.\n");
		exit(0);
	}

	printf("This program rearranges the filband file to plot 3D Fermi surface by XCrySDen.\n");
	printf("It reads filband or EIGENVAL and gives fermi.bxsf out.\n");
	printf("Note that the order of the kpints mesh should follow\n");
	printf("http://www.xcrysden.org/doc/XSF.html#__toc__14\n");
	printf("The best way is to generate the mesh before the calculation.\n");
	printf("2D plot is supported, however only for duplicating z values.\n");
	printf("\nPlease input the kpoints mesh:\n");
	scanf("%d %d %d", &n1, &n2, &n3);
	printf("Please input the Fermi energy:\n");
	scanf("%lf", &efermi);
	printf("Please input the begining and ending bands (start from 1, include all cross Fermi level):\n");
	scanf("%d %d", &begin, &end);

	if(n3==1)
		printf("2D plot recognized. z values will be duplicated.\n");

	bands = fopen(argv[1], "r");
	rl = fopen("reclatt", "r");
	fscanf(rl, "%lf", &blat);
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			fscanf(rl, "%lf", &rlat[i][j]);
			rlat[i][j] *= blat;
		}
	}
	fclose(rl);

	if(pwvasp == 1)
	{
		while(fgetc(bands)!='=');
		fscanf(bands, "%d", &nb);
		while(fgetc(bands)!='=');
		fscanf(bands, "%d", &nk);
	}
	else if(pwvasp == 2)
	{
		for(i=0; i<5; i++)
			while(fgetc(bands)!='\n'); // skip first five lines
		fscanf(bands, "%d %d %d", &anum, &nk, &nb);
	}

	if(nk != n1*n2*n3)
	{
		printf("# kpoints from filband/EIGENVAL does not match with input mesh.\n");
		exit(0);
	}

	if(n3==1)
		nk *= 2;

	vals = (double**)malloc(nb*sizeof(double*));
	for(ib=0; ib<nb; ib++)
		vals[ib] = (double*)malloc(nk*sizeof(double));

	for(ik=0; ik<nk;ik++)
	{
		while(fgetc(bands)!='\n');
		while(fgetc(bands)!='\n'); // Be very careful here!
		if(pwvasp == 2)
			while(fgetc(bands)!='\n'); // EIGENVAL has one more line to skip
		for(ib=0; ib<nb; ib++)
		{
			if(pwvasp == 2)
				fscanf(bands, "%d", &anum); // in EIGENVAL, there is band # before band value
			fscanf(bands,"%lf", &vals[ib][ik]);
			if(n3==1)
				vals[ib][ik+1] = vals[ib][ik];
		}
		if(n3==1)
			ik++;
	}

	if(n3==1)
		n3=2;

	fclose(bands);

	sortbands(vals, nb, nk);

	vout = (double**)malloc(nb*sizeof(double*));
	for(ib=0; ib<nb; ib++)
	{
		vout[ib] = (double*)malloc((n1+1)*(n2+1)*(n3+1)*sizeof(double)); // include a shell
		shell(vals[ib], vout[ib], n1, n2, n3); // calculate the shell values
	}

	n1++;
	n2++;
	n3++;
	nk = n1*n2*n3;
	vone = (double**)malloc(sizeof(double*));
	vone[0] = (double*)malloc(nk*sizeof(double));

// write general bxsf file
	strcpy(name, "fermi.bxsf");
	wbxsf(name, efermi, n1, n2, n3, begin, end, nk, rlat, vout, idhva);

// write one band bxsf file
	oneband(vout, vone, nb, nk, efermi);
	strcpy(name, "fermi-one.bxsf");
//	wbxsf(name, efermi, n1, n2, n3, 1, 1, nk, rlat, vone); // It always generates unphysical results

	for(ib=0; ib<nb; ib++)
	{
		free(vals[ib]);
		free(vout[ib]);
	}
	free(vals);
	free(vout);

	for(i=0; i<3; i++)
		free(rlat[i]);
	free(rlat);

	free(vone[0]);
	free(vone);

	return 0;
}
