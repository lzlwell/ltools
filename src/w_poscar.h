#ifndef _W_POSCAR_H_
#define _W_POSCAR_H_

#include<stdio.h>
#include<stdlib.h>

void w_poscar(char *filename, double **latt, double **pos, int natom, int ntype, char **types, int *typenum, int cd);

#endif
