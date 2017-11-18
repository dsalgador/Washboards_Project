#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <stdbool.h>

#include "DiscreteSimulation.h"

#define F_filename "F_matrix.txt"
#define f_max (unsigned short) 11
#define r (unsigned int) 10 //period of the road


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

	//Initialise the ROAD
	ROAD F;
	F.length = r;
	printf("%d", F.length);
	F.piles = (PILE *) malloc( ((int) F.length)*sizeof(PILE) );
	for(int i = 0; i<r;i++){
		//F.piles[i] = malloc(sizeof(PILE));
        F.piles[i].height = f_max;
        //printf("%d", F.piles[i].length);
        F.piles[i].blocks = (BLOCK *) malloc( ((int) F.piles[i].height)*sizeof(BLOCK));
        for(int j = 0; j<3;j++){
        	F.piles[i].blocks[j].filled = 1;
        	F.piles[i].blocks[j].simbol = 'X';

        }
        for(int k = 3; k < (int) F.piles[i].height;k++){
        	F.piles[i].blocks[k].filled = 0;
        	F.piles[i].blocks[k].simbol = 'O';
        }
    }

    //Print ROAD
    




	/* print some text to the file*/

	char *text = "Write this to the file";
	WriteMatrixToFile(&f, text);
	WriteMatrixToFile(&f, text);
	

	

	return 0;
}



