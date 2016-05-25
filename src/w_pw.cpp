#include"w_pw.h"

void w_pw(char *filename, double **latt, double **pos, int natom, int ntype, char **types, int *typenum)
{
	FILE *fp;
	int i, j, itype, accumul;
// rememberer that alat is 1.0
	const double alat = 1.0;

	fp = fopen(filename, "w");

	fprintf(fp, "CELL_PARAMETERS angstrom/bohr\n");

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
			fprintf(fp, "      %.10f", latt[i][j]);
		fprintf(fp, "\n");
	}

	fprintf(fp, "ATOMIC_POSITIONS crystal\n");

	itype = 0;
	accumul = typenum[itype];
	for(i=0; i<natom; i++)
	{
		if(i == accumul)
		{
			itype ++;
			accumul += typenum[itype];
		}

		fprintf(fp, " %s  ", types[itype]);
		for(j=0; j<3; j++)
			fprintf(fp, "     %.10f", pos[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);

	return;
}
