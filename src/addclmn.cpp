#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// This program is used to add a column to the end of some rows of a file

int main(int argc, char** argv)
{
	int start, end, line;
	char *add, ch;
	FILE *infile, *outfile;

	printf("Usage: addcol in_file out_file starting_line ending_line string_to_be_added \n");

	if(argc != 6)
	{
		printf("Wrong input format. See usage.\n");
		exit(0);
	}

	infile = fopen(argv[1], "r");
	outfile = fopen(argv[2], "w");
	start = atoi(argv[3]);
	end = atoi(argv[4]);
	add = argv[5];

	line = 1;
	while(line < start)
	{
		ch = fgetc(infile);
		if(ch != EOF)
			fputc(ch, outfile);
		if(ch == '\n')
			line ++;
	}

	while(line <= end)
	{
		ch = fgetc(infile);
		if(ch != EOF  &&  ch != '\n')
			fputc(ch, outfile);
		if(ch == '\n')
		{
			fprintf(outfile, "  %s\n", add);
			line ++;
		}
	}

	while(1)
	{
		ch = fgetc(infile);
		if(ch != EOF)
			fputc(ch, outfile);
		else
			break;
	}

	fclose(infile);
	fclose(outfile);

	return 0;
}


