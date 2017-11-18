#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <stdbool.h>

#include "DiscreteSimulation.h"

#define F_filename "F_matrix.txt"
#define f_max (unsigned short) 11
#define r (unsigned int) 60 //period of the road


void WriteMatrixToFile(FILE ** file, char * F){
	*file = fopen(F_filename, "a");
	if (*file == NULL){printf("Error changing to append 'a' file!\n");exit(1);	}
	fprintf(*file, "Some text: %s\n", F);   
    fclose(*file);
}

void PrintRoad(ROAD road, char * sep){
	for(int j = (int) (f_max-1); j >=0; j--){
    	for(int i = 0; i <road.length;i++){    
    	    printf(sep);	
    		printf("%c" ,road.piles[i].blocks[j].simbol);
    	}
    	printf(sep);
    	printf("\n");
    }
    if(strcmp(sep, "") != 0){ 
         for(int k = 0; k < road.length;k++){printf("##");}
     }
    else{for(int k = 0; k < road.length;k++){printf("#");} }
}

void InitialiseRoad(ROAD * F, unsigned short initial_height, char simbol_empty, char simbol_filled){
	(*F).length = r;
	//printf("%d", F.length);
	(*F).piles = (PILE *) malloc( ((int) (*F).length)*sizeof(PILE) );
	for(int i = 0; i<r;i++){
		//F.piles[i] = malloc(sizeof(PILE));
        (*F).piles[i].height = f_max;
        (*F).piles[i].f = initial_height;
        //printf("%d", F.piles[i].length);
        (*F).piles[i].blocks = (BLOCK *) malloc( ((int) (*F).piles[i].height)*sizeof(BLOCK));
        for(int j = 0; j< (int) (*F).piles[i].f;j++){
        	(*F).piles[i].blocks[j].filled = 1;
        	(*F).piles[i].blocks[j].simbol = simbol_filled;

        }
        for(int k = (int) (*F).piles[i].f; k < (int) (*F).piles[i].height;k++){
        	(*F).piles[i].blocks[k].filled = 0;
        	(*F).piles[i].blocks[k].simbol = simbol_empty;
        }
    }
}

int main(){

	//Create, initialise file
	/*FILE *f = fopen(F_filename, "w");
	if (f == NULL){printf("Error opening file!\n");exit(1);	}
*/
	//Initialise the ROAD	
   ROAD road;
   char sep[] = "";
   InitialiseRoad(&road, 3, ' ', '.');
   PrintRoad(road, sep);



	/* print some text to the file*/

	/*char *text = "Write this to the file";
	WriteMatrixToFile(&f, text);
	WriteMatrixToFile(&f, text);*/
	

	

	return 0;
}



