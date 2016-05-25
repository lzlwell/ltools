#include"pdos.h"

int main(int argc, char **argv)
{
	int nfile, nline, iscol;
	int ifile, iline, icol;

	char fname[100];

	double *energy, *pdos, num;

	FILE *fpdos, *flist, *fout;

	printf("This program adds up pdos in different files and generate a file with the same format.\n");
	printf("Please prepare an input file list.pdos or similar. QE only.\n");
	printf("Usage:  pdos input output\n");

	if(argc!=3)
	{
		printf("Please check usage.\n");
		exit(0);
	}

	flist = fopen(argv[1], "r");

	fscanf(flist, "%d", &nfile);
	while(fgetc(flist)!='\n');

	fscanf(flist, "%d", &nline);
	while(fgetc(flist)!='\n');
	nline --;  // remove first head line

	fscanf(flist, "%d", &iscol);
	while(fgetc(flist)!='\n');


	energy = (double*)malloc(nline*sizeof(double));
	pdos = (double*)malloc(nline*sizeof(double));

	for(iline=0; iline<nline; iline++)
	{
		energy[iline] = 0;
		pdos[iline] = 0;
	}

	for(ifile = 0; ifile<nfile; ifile++)
	{
		fscanf(flist, "%s", fname);
		fpdos = fopen(fname, "r");

		while(fgetc(fpdos)!='\n');


		for(iline=0; iline<nline; iline++)
		{
			for(icol=0; icol <= iscol; icol++)
			{
				fscanf(fpdos, "%lf", &num);

				if(icol == 0)
					energy[iline] = num;

				else if(icol == iscol)
					pdos[iline] += num;
			}

			while(fgetc(fpdos)!='\n');
		}

		fclose(fpdos);
	}
	fclose(flist);

	fout = fopen(argv[2], "w");

	fprintf(fout, "Energy   PDOS_sum\n");

	for(iline = 0; iline<nline; iline++)
		fprintf(fout, "%f   %f   \n", energy[iline], pdos[iline]);

	fclose(fout);

	return 0;
}
