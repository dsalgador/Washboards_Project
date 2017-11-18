#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <stdbool.h>

#include "DiscreteSimulation.h"

#define F_filename "F_matrix.txt"
#define f_max (unsigned short) 11
#define r (unsigned int) 60 //period of the road
#define d (unsigned int) 7

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


void WriteMatrixToFile(FILE ** file, char * F){
	*file = fopen(F_filename, "a");
	if (*file == NULL){printf("Error changing to append 'a' file!\n");exit(1);	}
	fprintf(*file, "Some text: %s\n", F);   
    fclose(*file);
}

void PrintRoad(ROAD road, char * sep){
	char simbol;
	printf("\n");
	for(int j = (int) (f_max-1); j >=0; j--){
    	for(int i = 0; i <road.length;i++){  
            if(road.piles[i].blocks[j].filled){
            	simbol = road.simbol_filled;
            }
            else{simbol = road.simbol_empty;}
    	    printf(sep);	
    		printf("%c",simbol);


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
	(*F).simbol_empty = simbol_empty;
	(*F).simbol_filled = simbol_filled;


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
        	//(*F).piles[i].blocks[j].simbol = simbol_filled;

        }
        for(int k = (int) (*F).piles[i].f; k < (int) (*F).piles[i].height;k++){
        	(*F).piles[i].blocks[k].filled = 0;
        	//(*F).piles[i].blocks[k].simbol = simbol_empty;
        }
    }
}

void DecrPile(ROAD * road, int xpos, int width){
	int width2 = min(width, max((*road).piles[xpos].f - width , (*road).piles[xpos].f ));
	///if(  ((*road).piles[xpos].f - width) <0 ){width2 = }
    width = width2;
	(*road).piles[xpos].f = (*road).piles[xpos].f-width;
	for(int i = 0; i < width;i++){
   		(*road).piles[xpos].blocks[(*road).piles[xpos].f+i].filled = 0;
   	}
   

}

void IncrPile(ROAD * road, int xpos, int width){
    int width2 = min(width, f_max - (*road).piles[xpos].f); //check better?
    width = width2;

	(*road).piles[xpos].f = (*road).piles[xpos].f+width;
	
	   	//(*road).piles[xpos].blocks[(*road).piles[xpos].f-1].filled = 1;
   	for(int i = 0; i < width;i++){
   		(*road).piles[xpos].blocks[(*road).piles[xpos].f-i-1].filled = 1;
   	}
}

void DecrPiles(ROAD * road, int xmin, int xmax, int width){
	for(int xpos = xmin; xpos <= xmax;xpos++){
		DecrPile(road, xpos, width);
	}
}

void IncrPiles(ROAD * road, int xmin, int xmax, int width){
	for(int xpos = xmin; xpos <= xmax;xpos++){
		IncrPile(road, xpos, width);
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

   IncrPile(&road, 0, 1);
   PrintRoad(road, sep);

   //Initialise the WHEEL
   WHEEL wheel;
   wheel.diameter = d;
   wheel.xf = 1;
   wheel.x0 = 1-wheel.diameter;

   int L = 2*wheel.diameter;
    //Jump
   wheel.xf += L;
   wheel.x0 += L;
  

   IncrPiles(&road, wheel.x0- wheel.diameter+1, wheel.xf - wheel.diameter, 1);
   PrintRoad(road, sep);

   DecrPiles(&road, wheel.x0+1, wheel.xf , 1);
   PrintRoad(road, sep);

   /*DecrPiles(&road, wheel.x0+1, wheel.xf , 2);
   PrintRoad(road, sep);

   DecrPiles(&road, wheel.x0+1, wheel.xf , 0);
   PrintRoad(road, sep);*/


   wheel.elevation = road.piles[wheel.xf+1].f;
   printf("%i", wheel.elevation);

   printf("%d", max(1,2));

   IncrPiles(&road, wheel.x0- wheel.diameter+1, wheel.xf - wheel.diameter, 8);

   PrintRoad(road, sep);

   IncrPiles(&road, wheel.x0- wheel.diameter+1, wheel.xf - wheel.diameter, 2);

   PrintRoad(road, sep);

   




	/* print some text to the file*/

	/*char *text = "Write this to the file";
	WriteMatrixToFile(&f, text);
	WriteMatrixToFile(&f, text);*/
	

	

	return 0;
}

/*

  //Increment a pile
   IncrPile(&road, d+1, 3);
   PrintRoad(road, sep);

   //Decrement a pile

   DecrPile(&road, d+1, 2);
   PrintRoad(road, sep);
   DecrPile(&road, d+1, 2);
   PrintRoad(road, sep);
*/