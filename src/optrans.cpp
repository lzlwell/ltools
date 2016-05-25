/* This piece of code is used to post-process the optical transition matrix element
calculated from bands.x (by setting lp = .true.), <ck|p|vk>, all direct transitoins.

This program reads band energies, the matrix elements pairs, order the bands w.r.t.
energies, output the calculated matrix element or so-called oscillator strength 
for a given polarization in whole BZ.

It is found that all bands outputed by bands.x have already been ordered according
to wavefunction components. Things becom much easier. But checking w.r.t. energies
are recommended before working further on optical transition matrix elements.
*/

#include"optrans.h"

#define PI 3.14159265359

double Isqr(double complex p1, double complex p2, double theta)
{
	double complex p;

	p = p1 * cos(theta) + p2 * sin(theta);

	return creal( conj(p) * p );
}

int main(int argc, char **argv)
{
	int ic, iv, ik, ib, ip; // index for cond. bands, valence bands, kpts, total bands, polarization directions from 1-3 in cartesian
	int nk, nb, *nc, *nv; // n kpts, n bands, nc[nk] n cond. bands, nv[nk] n valence bands
	double complex ****pmat; // pmat[nk][3][nc][nv], nkpts, 3 polarizations, n cond. bands, n valence bands.
	double complex *pout;
	double complex p1, p2;  // p1 = px, p2 = py
	double theta, tantop, tanbot;
	double **plinout;
	double **kpts; // kpts[nk][3], cartetian coordinates of kpts

	int inum;
	double dnum;
	double real, imag;
	double complex cnum;

	double eps = 0.001;

	FILE *fabsqr, *ffull; // p_avg_absqr.dat and p_avg_full.dat, first file is only for check data
	FILE *fin, *fout;

	double complex pol[3];
	double polnorm;
	int transvb, transcb;


	double **reclatt;
	double blat;
	int i, j;
	int is2d;
	FILE *rl;

	printf("Usage: optrans (linpol)\n");
	printf("Please run reclatt.sh first.\n");

	reclatt = (double**)malloc(3*sizeof(double*));
	for(i=0; i<3; i++)
		reclatt[i] = (double*)malloc(3*sizeof(double));
	
	rl = fopen("reclatt", "r");
	fscanf(rl, "%lf", &blat);

	for(i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			fscanf(rl, "%lf", &reclatt[i][j]);
			reclatt[i][j] *= blat;
		}
	}
	fclose(rl);



//	fabsqr = fopen("p_avg_absqr.dat", "r");
	ffull = fopen("p_avg_full.dat", "r");

//	while(fgetc(fabsqr) != '\n');

	while(fgetc(ffull) != '=');
	fscanf(ffull, "%d", &nb);
	while(fgetc(ffull) != '=');
	fscanf(ffull, "%d", &nk);
	while(fgetc(ffull) != '\n');


	pmat = (double complex ****)malloc(nk * sizeof(double complex ***));
	kpts = (double**)malloc(nk * sizeof(double*));
	nc = (int*)malloc(nk * sizeof(int));
	nv = (int*)malloc(nk * sizeof(int));
// Now entering main reading loop
	for(ik=0; ik<nk; ik++)
	{
		pmat[ik] = (double complex ***)malloc(3 * sizeof(double complex **));
		kpts[ik] = (double*)malloc(3 * sizeof(double));

		fscanf(ffull, "%lf %lf %lf %d", &kpts[ik][0], &kpts[ik][1], &kpts[ik][2], &nv[ik]);
		kpts[ik][0] *= blat;
		kpts[ik][1] *= blat;
		kpts[ik][2] *= blat;

		while(fgetc(ffull) != '\n');
//		while(fgetc(fabsqr) != '\n');

		nc[ik] = nb - nv[ik];

		for(ip=0; ip<3; ip ++)
		{
			pmat[ik][ip] = (double complex **)malloc(nc[ik] * sizeof(double complex *));

			fscanf(ffull, "%d", &inum);
			if(inum != ip + 1)
			{
				printf("ip looping error.\n");
				exit(0);
			}

			while(fgetc(ffull) != '\n');
//			while(fgetc(fabsqr) != '\n');

			for(ic=0; ic<nc[ik]; ic++)
			{
				pmat[ik][ip][ic] = (double complex *)malloc(nv[ik] * sizeof(double complex));

				for(iv=0; iv<nv[ik]; iv++)
				{
					fscanf(ffull, "%lf %lf", &real, &imag);
//					fscanf(fabsqr, "%lf", &dnum);

//					if(fabs(dnum - real*real - imag*imag) > eps)  // dnum has already been squared in bands.x
//					{
//						printf("p_avg_absqr and p_avg_full files don't match.\n");
//						printf("ik+1, %d, ip+1, %d, ic+1, %d, iv+1, %d.\n", ik+1, ip+1, ic+1, iv+1);
//						printf("real %f, imag %f, dnum %f.\n", real, imag, dnum);
//						exit(0);
//					}

					pmat[ik][ip][ic][iv] = real + imag * I;
				}
			}
			while(fgetc(ffull) != '\n');
//			while(fgetc(fabsqr) != '\n');
		}
	}

//	fclose(fabsqr);
	fclose(ffull);
	printf("Done reading data!\n");
// Done reading

	
	fin = fopen("optrans.in", "r");

	polnorm = 0.0;

	for(ip=0; ip<3; ip++)
	{
		fscanf(fin, "%lf %lf", &real, &imag);
		pol[ip] = real + imag * I;

		polnorm += real*real + imag*imag;

		while(fgetc(fin)!='\n');
	}

	polnorm = sqrt(polnorm);
	if(polnorm < eps)
	{
		printf("Please check, input pol vector may be zero.\n");
		exit(0);
	}

	for(ip=0; ip<3; ip++)
		pol[ip] /= polnorm;


	fscanf(fin, "%d", &transvb);
	transvb--;
	while(fgetc(fin)!='\n');
	fscanf(fin, "%d", &transcb);
	transcb--;

	fclose(fin);
	printf("Done reading parameters!\n");
	printf("x-pol: %f+%fi\n", creal(pol[0]), cimag(pol[0]));
	printf("y-pol: %f+%fi\n", creal(pol[1]), cimag(pol[1]));
	printf("z-pol: %f+%fi\n", creal(pol[2]), cimag(pol[2]));
	printf("transvb %d , transcb %d .\n", transvb+1, transcb+1);

	pout = (double complex *)malloc(nk * sizeof(double complex));
	plinout = (double **)malloc(nk * sizeof(double*));
	for(ik=0; ik<nk; ik++)
		plinout[ik] = (double*)malloc(2*sizeof(double));


	for(ik=0; ik<nk; ik++)
	{
		pout[ik] = 0.0 + 0.0 * I;

		for(ip=0; ip<3; ip++)
			pout[ik] += pol[ip] * pmat[ik][ip][transcb][transvb];

		pout[ik] = conj(pout[ik]) * pout[ik];
	}

	fout = fopen("transitions.dat", "w");

	for(ik=0; ik<nk; ik++)
		fprintf(fout, "%f  %f  %f\n", kpts[ik][0], kpts[ik][1], creal(pout[ik]));

	fclose(fout);


	fout = fopen("transitions.mm", "w");

	for(ik=0; ik<nk; ik++)
		fprintf(fout, "{%f, %f, %f}, ", kpts[ik][0], kpts[ik][1], creal(pout[ik]));
	fprintf(fout, "\n");

	fclose(fout);


	if(argc == 2 && strcmp(argv[1], "linpol") == 0)
		is2d = 1;
	else
		is2d = 0;

	printf("Is the 2D max. lin. pol. info. needed? 1 for yes, 0 for no:\nThis run: %d\n", is2d);

	if(is2d == 0)
		return 0;

	fout = fopen("linpol2D.dat", "w");

	for(ik=0; ik<nk; ik++)
	{
		p1 = pmat[ik][0][transcb][transvb];
		p2 = pmat[ik][1][transcb][transvb];

		tantop = 2*creal( conj(p1) * p2 );
		tanbot = creal( conj(p1)*p1 - conj(p2)*p2 );

		theta = 0.5 * atan2(tantop, tanbot); // Origin (R) default is radian, but double check that


		if(Isqr(p1, p2, theta) > Isqr(p1, p2, theta + PI/2.0))
		{
			plinout[ik][0] = theta;
			plinout[ik][1] = Isqr(p1, p2, theta);
		}
		else
		{
			plinout[ik][0] = theta + PI/2.0;
			plinout[ik][1] = Isqr(p1, p2, theta + PI/2.0);
		}


		fprintf(fout, "%f  %f  %f  %f\n", kpts[ik][0], kpts[ik][1], plinout[ik][0], plinout[ik][1]);

	}


	fclose(fout);


	return 0;

}
