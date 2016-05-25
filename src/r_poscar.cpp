#include"r_poscar.h"

void r_poscar(char *filename, double *(**latt), double *(**pos), int *natom, int *ntype, char *(**types), int *(*typenum))
{
	FILE *fp;
	int i, j;
	double alat;
	char ch; // element name usually 1 or 2 letters, we assign maxium 10

	fp = fopen(filename, "r");

// pass head line and read lattice constant	
	while(fgetc(fp) != '\n');
	fscanf(fp, "%lf", &alat);
	while(fgetc(fp) != '\n');

// allocate latt and read latt. latt will be deleted in w_poscar.
	*latt = new_array(3);

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			fscanf(fp, "%lf", &(*latt)[i][j]);
			(*latt)[i][j] *= alat;  // consider lattice constant
		}
		while(fgetc(fp) != '\n'); // in case comments
	}
// count how many element types and store the types
	*ntype = 0;

	*types = (char**)malloc(sizeof(char*));
	do
	{
		(*ntype) ++;
		*types = (char**)realloc(*types, *ntype * sizeof(char*));
		(*types)[*ntype - 1] = (char*)malloc(10*sizeof(char)); // maximum element name length 10 letters
		fscanf(fp, "%s", (*types)[*ntype - 1]);
//printf("%s\n", types[*ntype - 1]);
		do
		{
			ch = fgetc(fp);
		}while(ch == ' ');  // There MUST NOT BE comments at the end of this line
		fseek(fp, -1, SEEK_CUR);  // Move backward by one char, so that fscanf can get complete type name
	}while(ch != '\n' && ch != '\r');  // '\r' for windows format, where it is /r/n when entering the next line

// read and add up number of atoms of each type
	*natom = 0;
	*typenum = (int*)malloc(*ntype * sizeof(int));
	for(i=0; i<*ntype; i++)
	{
		fscanf(fp, "%d", &(*typenum)[i]);
//printf("%d\n", typenum[i]);
		*natom += (*typenum)[i];
	}
	while(fgetc(fp) != '\n');
	while(fgetc(fp) != '\n'); // skip C or D line

// read positions
	*pos = new_array(*natom);
	for(i=0; i<*natom; i++)
	{
		for(j=0; j<3; j++)
			fscanf(fp, "%lf", &(*pos)[i][j]);
		while(fgetc(fp) != '\n'); // in case comments
	}

	fclose(fp);

	return;
}
