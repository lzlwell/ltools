#ifndef _CONV_POS_H_
#define _CONV_POS_H_

#include"func.h"
#include<stdio.h>
#include<stdlib.h>

void dir2car(double *dir, double *car, double **latt);
void inv_latt(double **latt, double **invLatt);
void car2dir(double *car, double *dir, double **latt);

#endif
