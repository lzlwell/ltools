#ifndef _FUNC_H_
#define _FUNC_H_

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double **new_array(int nArray);
void del_array(double **array, int nArray);
void print_array(double **array, int nArray);
void trans_array(double **array, double *dtrans, int nArray);
double dist(double pta[], double ptb[]);
double gaussian(double x, double mu, double sigma);
double findmax(double *array, int n, int *ind);
double findmin(double *array, int n, int *ind);

#endif
