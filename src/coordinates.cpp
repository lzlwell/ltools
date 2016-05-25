#include"coordinates.h"

int main(int argc, char **argv)
{
	char *infile, *outfile;
	double **latt, **dir, **car;
	int natom, ntype, *typenum;
	char **types;
	int cd;
	int i;
	double *dtrans;

	if(argc < 3)
	{
		printf("Error! Wrong input format! Program exits!\n");
		exit(0);
	}

	infile = argv[1];
	outfile = argv[2];

#ifdef D2C
	r_poscar(infile, &latt, &dir, &natom, &ntype, &types, &typenum);

	car = new_array(natom);
	for(i=0; i<natom; i++)
		dir2car(dir[i], car[i], latt);
	
	cd = 1;
	w_poscar(outfile, latt, car, natom, ntype, types, typenum, cd);
#endif

#ifdef C2D
	r_poscar(infile, &latt, &car, &natom, &ntype, &types, &typenum);

	dir = new_array(natom);
	for(i=0; i<natom; i++)
		car2dir(car[i], dir[i], latt);

	cd = 2;
	w_poscar(outfile, latt, dir, natom, ntype, types, typenum, cd);
#endif

#ifdef VASP2PW
	printf("At present vasp2pw only supports input POSCAR with fractional coordinates. \n");
	r_poscar(infile, &latt, &dir, &natom, &ntype, &types, &typenum);
	w_pw(outfile, latt, dir, natom, ntype, types, typenum);
#endif

#ifdef POSTRANS
	if(argc != 6)
	{
		printf("Error! Wrong input format! Program exits!\n");
		exit(0);
	}
	printf("At present postrans mainly supports input POSCAR with Cartetian coordinates.\n");
	dtrans = (double*)malloc(3*sizeof(double));
	dtrans[0] = atof(argv[3]);
	dtrans[1] = atof(argv[4]);
	dtrans[2] = atof(argv[5]);

	r_poscar(infile, &latt, &car, &natom, &ntype, &types, &typenum);

	trans_array(car, dtrans, natom);

	cd = 3; // to print Cartesian/Direct
	w_poscar(outfile, latt, car, natom, ntype, types, typenum, cd);
	free(dtrans);
#endif


#ifndef VASP2PW
	del_array(car, natom);
#endif
#ifndef POSTRANS
	del_array(dir, natom);
#endif
	del_array(latt, 3);
	free(typenum);
	for(i=0; i<ntype; i++)
		free(types[i]);
	free(types);

	return 0;
}
