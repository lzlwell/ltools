#include"w_poscar.h"

void w_poscar(char *filename, double **latt, double **pos, int natom, int ntype, char **types, int *typenum, int cd)
{
	FILE *fp;
	int i, j;
// in writing poscar, simply let lattice constant to be 1.0
// alat * latt has been considered in r_poscar
	const double alat = 1.0;

	fp = fopen(filename, "w");

	fprintf(fp, "System\n"); // System name
	fprintf(fp, "%f\n", alat);

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
			fprintf(fp, "      %.10f", latt[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<ntype; i++)
	{
		fprintf(fp, "   %s", types[i]);
	}
	fprintf(fp, "\n");

	for(i=0; i<ntype; i++)
	{
		fprintf(fp, "   %d", typenum[i]);
	}
	fprintf(fp, "\n");

	if(cd == 1)
		fprintf(fp, "Cartesian\n");
	else if(cd == 2)
		fprintf(fp, "Direct\n");
	else if(cd == 3)
		fprintf(fp, "Cartesian/Direct\n");

	for(i=0; i<natom; i++)
	{
		for(j=0; j<3; j++)
			fprintf(fp, "     %.10f", pos[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);

	return;
}
