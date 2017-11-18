#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(){

	//Print vector components to a file 
	FILE *f = fopen("F_matrix.txt", "a");
	if (f == NULL){printf("Error opening file!\n");exit(1);	}

	/* print some text */
	const char *text = "Write this to the file";
	fprintf(f, "Some text: %s\n", text);

	fclose(f);








	return 0;
}

