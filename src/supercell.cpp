 #include<stdio.h>
 #include<stdlib.h>
 int main()
 {
	printf("\n\n\n");
	printf("************************************************************************************************\n");
	printf("This program is used to create supercell without losing accuracy from CONTCAR directly.\n");
	printf("The following parameters should be input mannually:\n");
	printf("natom - the number of all atom types; na, nb, nc - the scale of the supercell.\n");
	printf("ntype - the number of types of atoms.\n");
	printf("The file with supercell imformation named 'supercell' will be created.\n");
	printf("The 'supercell' file could be directly used in VASP after rename.\n");
	printf("\nATTENTION: MAKE SURE the positions in CONTCAR is in Direct coordinates!\n");
	printf("\n");
	
	FILE *fread, *fwrite;
	double latticeBefore[3][3], latticeAfter[3][3];
	double latticeConstant;
	double temp;
	double **positionBefore, **positionAfter;
	double na, nb, nc;
	int natom, Natom, ntype, i, j, k, markBefore, markAfter;
	char title[31], **elementName;
	int *elementNumber;
	

	
	if (  ( fread = fopen("CONTCAR","r") ) == NULL )
	{
	        printf("Cannot open CONTCAR!\n");
	        exit(0);
	}

	printf("Please input natom (integer):\n");
	scanf("%d", &natom);
	printf("Please input ntype (integer):\n");
	scanf("%d", &ntype);
	printf("Please input na nb nc (float number):\n");
	scanf("%lf %lf %lf", &na, &nb, &nc);
	
	Natom = natom * na * nb * nc;
	printf("Natom - the number of atoms in supercell is :%d\n",Natom);

	positionBefore = (double**)malloc(natom * sizeof(double*));
	positionAfter  = (double**)malloc(Natom * sizeof(double*));
	elementName = (char**)malloc(ntype * sizeof(char*));
	elementNumber = (int*)malloc(ntype * sizeof(int));
	
	for(i=0; i<natom; i++)
		positionBefore[i] = (double*)malloc(3 * sizeof(double));
	for(i=0; i<Natom; i++)
		positionAfter[i]  = (double*)malloc(3 * sizeof(double));
	for(i=0; i<ntype; i++)
		elementName[i] = (char*)malloc(3 * sizeof(char));
	
	fscanf(fread, "%s", title);
	while(fgetc(fread)!='\n');
	fscanf(fread, "%lf", &latticeConstant);
	while(fgetc(fread)!='\n');

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
			fscanf(fread, "%lf", &latticeBefore[i][j]);
		while(fgetc(fread)!='\n');
	}

	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
			latticeBefore[i][j] *= latticeConstant;
	

	for(i=0; i<ntype; i++)
		fscanf(fread, "%s", elementName[i]);
	while(fgetc(fread)!='\n');

	for(i=0; i<ntype; i++)
		fscanf(fread, "%d", &elementNumber[i]);
	while(fgetc(fread)!='\n');
	while(fgetc(fread)!='\n');
	
	for(i=0; i<natom; i++)
	{
		for(j=0; j<3; j++)
			fscanf(fread, "%lf", &positionBefore[i][j]);
		while(fgetc(fread)!='\n');
	}
	fclose(fread);
	
	for(i=0; i<natom; i++)
	{
		positionBefore[i][0] /= na;
		positionBefore[i][1] /= nb;
		positionBefore[i][2] /= nc;
	}

	markAfter = 0;
	for(markBefore=0; markBefore<natom; markBefore++)
	{
		for(i=0; i<na; i++)
		{
			for(j=0; j<nb; j++)
			{
				for(k=0; k<nc; k++)
				{
					positionAfter[markAfter][0] = positionBefore[markBefore][0] + i/na ;
					positionAfter[markAfter][1] = positionBefore[markBefore][1] + j/nb ;
					positionAfter[markAfter][2] = positionBefore[markBefore][2] + k/nc ;

					markAfter ++ ; 
				}
			}
		}
	}
	
	if (  ( fwrite = fopen("supercell","w") ) == NULL )
	{
	        printf("Cannot open or create 'supercell'!\n");
		exit(0);
	}

	for(j=0; j<3; j++)
	{
		latticeAfter[0][j] = latticeBefore[0][j] * na;
		latticeAfter[1][j] = latticeBefore[1][j] * nb;
		latticeAfter[2][j] = latticeBefore[2][j] * nc;
	}

	latticeConstant = 1.0;
	
	fprintf(fwrite, "%s\n", title);
	fprintf(fwrite, "%.14f\n", latticeConstant);

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
			fprintf(fwrite, "     %.16f", latticeAfter[i][j]);
		fprintf(fwrite, "\n");
	}

	for(i=0; i<ntype; i++)
		fprintf(fwrite, "    %s", elementName[i]);
	fprintf(fwrite, "\n");

	for(i=0; i<ntype; i++)
		fprintf(fwrite, "    %d", elementNumber[i]*(int)(na*nb*nc));
	fprintf(fwrite, "\n");

	fprintf(fwrite, "Direct\n");
	
	for(i=0; i<Natom; i++)
	{
		for(j=0; j<3; j++)
			fprintf(fwrite, "    %.16f", positionAfter[i][j]);
		fprintf(fwrite, "\n");
	}
	fclose(fwrite);
	
	printf("\nThe file 'supercell' is created successfully!\n");
	printf("************************************************************************************************\n");
	
	return 0;
}
