#include"spinplot.h"

void spin_k(int nthb, int skip)
{
	int nbnds, nkpts;
	int ib, ik;
	int i, j;
	FILE *fx, *fy, *fz;
	FILE *fout, *fmm;
	double kx, ky, kz;
	double sx, sy, sz;
	double **reclatt;
	double blat;
	FILE *rl;

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


	fx = fopen("filband.1", "r");
	fy = fopen("filband.2", "r");
	fz = fopen("filband.3", "r");

	fout = fopen("spin_k.out", "w");
	fmm = fopen("spin_k.mm", "w");

	while(fgetc(fx) != '=');
	fscanf(fx, "%d", &nbnds);
	while(fgetc(fx) != '=');
	fscanf(fx, "%d", &nkpts);
	while(fgetc(fx) != '\n');
	while(fgetc(fy) != '\n');
	while(fgetc(fz) != '\n');

	for(ik=0; ik<nkpts; ik++)
	{
		fscanf(fx, "%lf %lf %lf", &kx, &ky, &kz);
		fscanf(fy, "%lf %lf %lf", &kx, &ky, &kz);
		fscanf(fz, "%lf %lf %lf", &kx, &ky, &kz);

		kx *= blat;
		ky *= blat;
		kz *= blat;

		for(ib=0; ib<nbnds; ib++)
		{
			fscanf(fx, "%lf", &sx);
			fscanf(fy, "%lf", &sy);
			fscanf(fz, "%lf", &sz);

			if(nthb == (ib+1))
			{
				fprintf(fout, "%f %f %f %f\n", kx, ky, sx, sy);
				if(ik==0)
				{
					fprintf(fmm, "ListVectorPlot[{");
					fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx, ky, sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[0][0], ky-reclatt[0][1], sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[1][0], ky-reclatt[1][1], sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[0][0]-reclatt[1][0], ky-reclatt[0][1]-reclatt[1][1], sx, sy);
				}
				else if(ik==nkpts-1)
				{
					fprintf(fmm, "{{%f, %f}, {%f, %f}}", kx, ky, sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[0][0], ky-reclatt[0][1], sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[1][0], ky-reclatt[1][1], sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[0][0]-reclatt[1][0], ky-reclatt[0][1]-reclatt[1][1], sx, sy);
					fprintf(fmm, "}, VectorPoints->All, VectorScale->Tiny]\n");
				}
				else if(ik%skip == 0)
				{
					fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx, ky, sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[0][0], ky-reclatt[0][1], sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[1][0], ky-reclatt[1][1], sx, sy);
	//				fprintf(fmm, "{{%f, %f}, {%f, %f}}, ", kx-reclatt[0][0]-reclatt[1][0], ky-reclatt[0][1]-reclatt[1][1], sx, sy);
				}
			}
		}
	}

	fclose(fx);
	fclose(fy);
	fclose(fz);
	fclose(fout);
	fclose(fmm);
	fclose(rl);

	return;
}


int main(int argc, char** argv)
{
	if(argc == 3)
	{
		printf("Please run reclatt.sh first.\n");
		spin_k(atoi(argv[1]), atoi(argv[2]));
	}
	else
		printf("Check input format: spinplot nthb skip\n nth starts from 1, and skip=1 is no skip.\n");
	return 0;
}
