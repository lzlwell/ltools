#include"eqpgap.h"

int main(int argc, char** argv)
{
	int nk, nb;
	double *mfgap, *mfvbm, *mfcbm;
	double *qpgap, *qpvbm, *qpcbm;
	double **kdir, **kcar;
	double **reclatt;

	int index, nocc, tmpi;

	FILE *mfgap, *qpgap, *diffgap;
	FILE *rl;

	printf("Please run reclatt.sh first.\n");
	printf("Usage: eqpgap nk nb nocc \n");


	if(argc != 4)
	{
		printf("Please check usage. Program exits.\n");
		exit(0);
	}

	nk = atoi(argv[1]);
	nb = atoi(argv[2]);
	nocc = atoi(argv[3]);


	mfgap = new double[nk];
	mfvbm = new double[nk];
	mfcbm = new double[nk];

	qpgap = new double[nk];
	qpvbm = new double[nk];






}
