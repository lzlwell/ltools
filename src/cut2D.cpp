#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define unit_conv 1.0

int insquare(double kx, double ky, double xcut, double ycut)
{
	if((fabs(kx)<=xcut) && (fabs(ky)<=ycut))
		return 1;
	else
		return 0;
}

int main(int argc, char** argv)
{
	int i, j, npts, nval;
	double k[2], *vals, kx, ky;
	double blat, reclatt[3][3];
	double xcut, ycut;
	int n1, n2;
	
	FILE *in, *rl, *mm, *dat;

	printf("2D cut based on first two Cartesian coordinates.\n");
	printf("Please run reclatt.sh first.\n");
	printf("A supercell will be constructed first then cutted into square required.\n");
	printf("Input format: cut2D infile nval npts xcut ycut\n");

	if(argc != 6)
	{
		printf("Please check input format.\n");
		exit(0);
	}

	in = fopen(argv[1], "r");
	rl = fopen("reclatt", "r");
	mm = fopen("cut2D.mm", "w");
	dat = fopen("cut2D.dat", "w");

	fscanf(rl, "%lf", &blat);
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			fscanf(rl, "%lf", &reclatt[i][j]);
			reclatt[i][j] *= blat;
		}
	}

	nval = atoi(argv[2]);
	npts = atoi(argv[3]);
	xcut = atof(argv[4]);
	ycut = atof(argv[5]);
	
	vals = (double*)malloc(nval*sizeof(double));

	for(i=0; i<npts; i++)
	{
		fscanf(in, "%lf %lf", k, k+1);
		for(j=0; j<nval; j++)
			fscanf(in, "%lf", vals+j);

		for(n1=-1; n1<=0; n1++)
		{
			for(n2=-1; n2<=0; n2++)
			{
				kx = k[0] + n1*reclatt[0][0] + n2*reclatt[1][0];
				ky = k[1] + n1*reclatt[0][1] + n2*reclatt[1][1];
				if(insquare(kx, ky, xcut, ycut))
				{
					if(nval == 1)
					{
						fprintf(mm, "{%f, %f, %f}, ", kx, ky, vals[0]);
						fprintf(dat, "%f  %f  %f\n", kx, ky, vals[0]);
					}
					else if(nval == 2)
					{
						fprintf(mm, "{{%f, %f}, {%f, %f}}, ", kx, ky, vals[0], vals[1]);
						fprintf(dat, "%f  %f  %f  %f\n", kx, ky, vals[0], vals[1]);
					}
				}
			}
		}
	}
	fprintf(mm, "\n");
	fprintf(dat, "\n");

	free(vals);

	fclose(in);
	fclose(rl);
	fclose(mm);
	fclose(dat);

	return 0;
}

