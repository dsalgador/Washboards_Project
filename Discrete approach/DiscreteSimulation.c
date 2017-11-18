#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define F_filename "F_matrix.txt"

void WriteMatrixToFile(FILE ** file, char * F){
	*file = fopen(F_filename, "a");
	if (*file == NULL){printf("Error changing to append 'a' file!\n");exit(1);	}
	fprintf(*file, "Some text: %s\n", F);   
    fclose(*file);

}

int main(){

	//Create, initialise file
	FILE *f = fopen(F_filename, "w");
	if (f == NULL){printf("Error opening file!\n");exit(1);	}

	/* print some text to the file*/

	char *text = "Write this to the file";
	WriteMatrixToFile(&f, text);
	WriteMatrixToFile(&f, text);
	

	

	return 0;
}



