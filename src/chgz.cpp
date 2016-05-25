#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc, char **argv)
{
	int nx, ny, nz;
	int i, iz;
	int skip;
	double z;
	double value;
	double *chgz, *chgzint;
	double *zscl, *zintscl; // Scaled values
	double totchg, totvchg;
	FILE *infile, *outz, *outzint;

// 0.99 percent valence electrons. semi-core electrons are assumed to be very very localized
	const double percent = 0.99;
	int ne, nve; // number of electrons and valence electrons
	int upiz, lowiz;

// Since both pwscf and vasp write charge density five numbers per line, this program suits both in principle.
// However before the format of vasp CHGCAR is confirmed, this version is ONLY for pwscf without spin polarization.

	printf("Integration of charge density along the third direction (usually z):\n");
	printf("Usage: chg_z infile\n");
	printf("Two files, chgz.dat and chgzint.dat, will be generated.\n");
	printf("No periodicity is considered.\n");
	printf("Designed for the case that slab is located around middle of z.\n");
	
	if(argc != 2)
	{
		printf("Wrong format. Please see usage.\n");
		exit(0);
	}

	infile = fopen(argv[1], "r");
	outz = fopen("chgz.dat", "w");
	outzint = fopen("chgzint.dat", "w");

	printf("Please input the grids, in form of\nnx  ny  nz\n");
	scanf("%d %d %d", &nx, &ny, &nz);
	printf("Please input the number of lines to skip:\n");
	scanf("%d", &skip);

// The above are the general preparation part. The following is specifically for integration along z.

	printf("Please input the z-height of the cell:\n");
	scanf("%lf", &z);

	printf("Please input the number of the total electrons and valence electrons:\n");
	scanf("%d %d", &ne, &nve);
	printf("Calculating...\n");

	chgz = (double*)malloc(nz * sizeof(double));
	chgzint = (double*)malloc(nz * sizeof(double));
	zscl = (double*)malloc(nz * sizeof(double));
	zintscl = (double*)malloc(nz * sizeof(double));

	for(i=0; i<skip; i++)
		while(fgetc(infile) != '\n');

	for(iz = 0; iz < nz; iz ++)
	{
		chgz[iz] = 0;
		for(i=0; i < nx*ny; i++)
		{
			fscanf(infile, "%lf", &value);
			chgz[iz] += value;
		}

		if(iz == 0)
			chgzint[iz] = chgz[iz];
		else
			chgzint[iz] = chgzint[iz-1] + chgz[iz];
	}

	totchg = chgzint[nz-1];
	totvchg = totchg * nve / ne;
	for(iz = 0; iz < nz; iz ++)
	{
		fprintf(outz, "%f  %f  %f\n", iz*z/nz, chgz[iz], chgz[iz]/totchg);
		fprintf(outzint, "%f  %f  %f\n", iz*z/nz, chgzint[iz], chgzint[iz]/totchg);

		if(chgzint[iz] < (1-percent)*0.5 * totvchg )
			lowiz = iz;
	}

	for(iz = 0; iz < nz; iz ++)
	{
		if(chgzint[iz] - totchg + totvchg > (percent + (1-percent)*0.5) * totvchg )
		{
			upiz = iz;
			break;
		}
	}

	printf("99 percent charge contains in the z-height of %f.\n", z/nz*(upiz - lowiz + 1));
	printf("GW calculations require at least %f due to Coulomb truncation.\n", 2*z/nz*(upiz - lowiz + 1));

	printf("Done!\n\n");

	printf("ATTENTION: When changing lattice, fractional coordinates should be \n");
	printf("           modified AS WELL!\n");

	free(chgz);
	free(chgzint);
	free(zscl);
	free(zintscl);

	fclose(infile);
	fclose(outz);
	fclose(outzint);

	return 0;
}

