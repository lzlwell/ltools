#ifndef _R_POSCAR_H_
#define _R_POSCAR_H_

#include"func.h"
#include<stdio.h>
#include<stdlib.h>

// This function will allocate memory. Free them after usage.
void r_poscar(char *filename, double *(**latt), double *(**pos), int *natom, int *ntype, char *(**types), int *(*typenum));

#endif
