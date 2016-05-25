#include"func.h"

double **new_array(int nArray)
{
	int i;
	double **array;

	array = (double**)malloc(nArray*sizeof(double*));
	for(i=0; i<nArray; i++)
		array[i] = (double*)malloc(3*sizeof(double));

	return array;
}

void del_array(double **array, int nArray)
{
	int i;

	for(i=0; i<nArray; i++)
		free(array[i]);
	free(array);

	return;
}

void print_array(double **array, int nArray)   // mainly for debug
{
	int i;

	printf("\n");
	for(i=0; i<nArray; i++)
		printf(" %f  %f  %f\n", array[i][0], array[i][1], array[i][2]);
	printf("\n");

	return;
}

void trans_array(double **array, double *dtrans, int nArray)
{
	int i, j;

	for(i=0; i<nArray; i++)
		for(j=0; j<3; j++)
			array[i][j] += dtrans[j];

	return;
}

double dist(double pta[], double ptb[])
{
	double dsqr;
	int i;

	dsqr = 0.0;
	for(i=0; i<3; i++)
		dsqr += (pta[i] - ptb[i]) * (pta[i] - ptb[i]);

	return sqrt(dsqr);
}

double gaussian(double x, double mu, double sigma)
{
  double pi = 3.14159265359;
  double f;

  f = exp(-1*(x-mu)*(x-mu)/2/sigma/sigma);
  f = f/sigma/sqrt(2*pi);

  return f;
}

double findmax(double *array, int n, int *ind)
{
	int i;
	double max;

	max = array[0];
	*ind = 0;
	for(i=0; i<n; i++)
	{
		if(array[i]>max)
		{
			max = array[i];
			*ind = i;
		}
	}

	return max;
}

double findmin(double *array, int n, int *ind)
{
	int i;
	double min;

	min = array[0];
	*ind = 0;
	for(i=0; i<n; i++)
	{
		if(array[i]<min)
		{
			min = array[i];
			*ind = i;
		}
	}

	return min;
}
