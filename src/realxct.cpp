#include"realxct.h"

int main(int argc, char** argv)
{
	double car[3], dir[3];
	int na, nb, nc;
	int ia, ib, ic;
	int i, j;

	double lattice[3][3];
	double num;

	FILE *fxct, *fplot;

	printf("This program convert output of plotxct to Origin readable format.\n");
	printf("Usage: realxct input output\n");
	printf("2D integrated only for now.\n");

	if(argc!=3)
	{
		printf("Please check usage. Program exits.\n");
		exit(0);
	}

	fxct = fopen(argv[1], "r");
	fplot = fopen(argv[2], "w");


	for(i=0; i<6; i++)
		while(fgetc(fxct)!='\n');

	for(i=0; i<3; i++)
	{
		while(fgetc(fxct)!='=');
		fscanf(fxct, "%lf %lf %lf", &lattice[i][0], &lattice[i][1], &lattice[i][2]);
		while(fgetc(fxct)!='\n');
	}

	while(fgetc(fxct)!='\n');
	while(fgetc(fxct)!='=');
	fscanf(fxct, "%d %d %d", &na, &nb, &nc);

	while(fgetc(fxct)!='\n');
	while(fgetc(fxct)!='\n');
	while(fgetc(fxct)!='\n');


	for(ic=0; ic<nc; ic++)
	{
		for(ib=0; ib<nb; ib++)
		{
			for(ia=0; ia<na; ia++)
			{
				dir[0] = (double)ia/na;
				dir[1] = (double)ib/nb;
				dir[2] = (double)ic/nc;

				car[0] = lattice[0][0]*dir[0] + lattice[1][0]*dir[1] + lattice[2][0]*dir[2];
				car[1] = lattice[0][1]*dir[0] + lattice[1][1]*dir[1] + lattice[2][1]*dir[2];
				car[2] = lattice[0][2]*dir[0] + lattice[1][2]*dir[1] + lattice[2][2]*dir[2];


				fscanf(fxct, "%lf", &num);

				fprintf(fplot, "%lf  %lf  %lf \n", car[0], car[1], num);
			}
		}
	}

	fclose(fxct);
	fclose(fplot);

	return 0;
}
