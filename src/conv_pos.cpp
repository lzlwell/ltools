#include"conv_pos.h"

void dir2car(double *dir, double *car, double **latt)
{
	car[0] = dir[0]*latt[0][0] + dir[1]*latt[1][0] + dir[2]*latt[2][0];
	car[1] = dir[0]*latt[0][1] + dir[1]*latt[1][1] + dir[2]*latt[2][1];
	car[2] = dir[0]*latt[0][2] + dir[1]*latt[1][2] + dir[2]*latt[2][2];
	return;
}

void inv_latt(double **latt, double **invLatt)
{
	double a11, a12, a13, a21, a22, a23, a31, a32, a33;
	double d11, d12, d13, d21, d22, d23, d31, d32, d33, detA;

	a11 = latt[0][0]; a12 = latt[0][1]; a13 = latt[0][2];
	a21 = latt[1][0]; a22 = latt[1][1]; a23 = latt[1][2];
	a31 = latt[2][0]; a32 = latt[2][1]; a33 = latt[2][2];

	d11 = a22*a33 - a23*a32;
	d21 = a21*a33 - a23*a31;
	d31 = a21*a32 - a22*a31;
	d12 = a12*a33 - a13*a32;
	d22 = a11*a33 - a13*a31;
	d32 = a11*a32 - a12*a31;
	d13 = a12*a23 - a13*a22;
	d23 = a11*a23 - a13*a21;
	d33 = a11*a22 - a12*a21;
	d12 *= -1; d21 *= -1; d23 *= -1; d32 *= -1;
	detA = a11*a22*a33 + a12*a23*a31 + a21*a32*a13 - a13*a22*a31 - a23*a32*a11 - a12*a21*a33;

	invLatt[0][0] = d11/detA; invLatt[0][1] = d12/detA; invLatt[0][2] = d13/detA;
	invLatt[1][0] = d21/detA; invLatt[1][1] = d22/detA; invLatt[1][2] = d23/detA;
	invLatt[2][0] = d31/detA; invLatt[2][1] = d32/detA; invLatt[2][2] = d33/detA;

	return;
}

void car2dir(double *car, double *dir, double **latt)
{
	double **invLatt;
	invLatt = new_array(3);
	
	inv_latt(latt, invLatt);

    dir[0] = car[0]*invLatt[0][0] + car[1]*invLatt[1][0] + car[2]*invLatt[2][0];
    dir[1] = car[0]*invLatt[0][1] + car[1]*invLatt[1][1] + car[2]*invLatt[2][1];
    dir[2] = car[0]*invLatt[0][2] + car[1]*invLatt[1][2] + car[2]*invLatt[2][2];

	del_array(invLatt, 3);

    return;
}
