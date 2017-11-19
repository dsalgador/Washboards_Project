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
#define beta (unsigned int) 3

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


void WriteMatrixToFile(FILE ** file, ROAD road){
	*file = fopen(F_filename, "a");
	if (*file == NULL){printf("Error changing to append 'a' file!\n");exit(1);	}
	for(int i = 0;i < road.length;i++){		 
		if(i<road.length-1){fprintf(*file, "%u,", road.piles[i].f);}
		else{fprintf(*file, "%u\n", road.piles[i].f);}
	}
	//printf("\n\n"); 
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
	if(xmin < 0 || xmax <0){printf("\n DecrPiles: negative xmin or xmax"); exit(2);}
	for(int xpos = xmin; xpos <= xmax;xpos++){
		DecrPile(road, xpos, width);
	}
}

void IncrPiles(ROAD * road, int xmin, int xmax, int width){
	if(xmin < 0 || xmax <0){printf("\n IncrPiles: negative xmin or xmax"); exit(2);}
	for(int xpos = xmin; xpos <= xmax;xpos++){
		IncrPile(road, xpos, width);
	}
}

int MoveToNextBump(ROAD road, WHEEL * wheel){
	   unsigned short poscount = (*wheel).xf;
	   while(poscount < road.length &  road.piles[poscount].f <= (*wheel).elevation) poscount++;
	   
	   //Now poscount have the position where the bump starts, i.e. x0+1
	   //printf("\nposcount: %d\n", poscount);
	   (*wheel).xf = poscount;
	   (*wheel).x0 = (*wheel).xf - (*wheel).diameter;
	   return poscount;
   }

 int CurrentBumpHeight(ROAD road, WHEEL wheel){
 	   unsigned short h_index;
	   for(int  i = 1; i<= wheel.diameter; i++){
	   	   if(road.piles[wheel.xf +i].f > wheel.elevation & road.piles[wheel.xf +i].f > road.piles[wheel.xf + h_index].f )
	   	   	{h_index=i;   	      	   	
	   	      	   }
	   }
	   h_index += wheel.xf;
	   printf("\n h_index = %d\n ", h_index);

	   int h = road.piles[h_index].f - wheel.elevation;
	   if(h < 0){
	   	printf("\nNegative Bump width\n");
	   	exit(-1);
	   }

	   return h;
 }


int main(){

	//Create, initialise file
	FILE *f = fopen(F_filename, "w");
	if (f == NULL){printf("Error opening file!\n");exit(1);	}

	//Initialise the ROAD	
   ROAD road;
   char sep[] = "";
   InitialiseRoad(&road, 3, ' ', '.');

   IncrPile(&road, 0, 1);
   IncrPiles(&road, 24,28 ,1);
   IncrPile(&road, 25, 1);
   IncrPile(&road, 27, 2);


   PrintRoad(road, sep);

   //Initialise the WHEEL
   WHEEL wheel;
   wheel.diameter = d;
   wheel.xf = 1;
   wheel.x0 = 1-wheel.diameter;
   wheel.elevation = road.piles[wheel.xf+1].f;
   printf("%i", wheel.elevation);

   //int L = 2*wheel.diameter;
   int L = (int) (beta * road.piles[wheel.xf-1].f);
   printf("%d", L);
    //Jump
   wheel.xf += L;
   wheel.x0 += L;
  

   IncrPiles(&road, wheel.x0- wheel.diameter+1, wheel.xf - wheel.diameter, 1);
   PrintRoad(road, sep);

   DecrPiles(&road, wheel.x0+1, wheel.xf , 1);
   PrintRoad(road, sep);

   wheel.elevation = road.piles[wheel.xf+1].f;
   printf("\nwheel elevation: %i \n", wheel.elevation);
   
   L = (int) (beta * road.piles[wheel.xf-1].f);  


   WriteMatrixToFile(&f,road);

  
	  
	  
   //search the first bump that the wheel is going to find and update the wheel positions
   unsigned short poscount = MoveToNextBump(road, &wheel);
   //calculate h  
   unsigned int h = CurrentBumpHeight(road, wheel);
   printf("h: %d", h);





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