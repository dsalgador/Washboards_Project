#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
//#include <stdbool.h>

#include "DiscreteSimulation_p.h"

#define F_filename "F_matrix.txt"
#define f_max (unsigned short) 15
#define r (unsigned int) 150 //period of the road 60
#define d (unsigned int) 5 //7
#define beta (unsigned int) 20 //3
//presentation w = 35, d 7 beta 5 gmax 15

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
  if (*file == NULL){printf("Error changing to append 'a' file!\n");exit(1);  }
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


void PrintRoadWheelInfo(ROAD road, WHEEL wheel){
  printf("\n wheel.xf =  %d \n", wheel.xf );
  printf(" wheel.x0 = %d  \n", wheel.x0);
  printf(" wheel elevation = %d  \n", wheel.elevation);
  printf(" wheel diameter = %d  \n", wheel.diameter);

  printf(" f(wheel.xf) = %d   \n", road.piles[wheel.xf].f );
  printf(" f(wheel.x0) = %d    \n", road.piles[wheel.x0].f);
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
    int width2 = min(width, f_max - ((*road).piles[xpos].f +0) ); //check better?
    width = width2;
    /*if(xpos == 0){
      printf("\nwidth incr: %d\n", width);}*/

    if(width != 0){
        (*road).piles[xpos].f = (*road).piles[xpos].f+width;
      
          //(*road).piles[xpos].blocks[(*road).piles[xpos].f-1].filled = 1;
        for(int i = 0; i < width;i++){
          (*road).piles[xpos].blocks[(*road).piles[xpos].f-i-1].filled = 1;
        }
  }
  else{  printf("Width == 0, no increase considered in pos %d", xpos);}
}


int * positions(int xmin, int xmax){
  int size = (xmax-xmin +1);
  int count = 0;
  int * positions = (int *) malloc( size*sizeof(int) );
  
  if(xmin >= 0 & xmax <= r){
      positions = NULL;
  } //We do not consider the possibility that the wheel's diameter is larger than the length of the road:
  else if(xmin < 0 & xmin > -r & xmax <=r & xmax >=0){
    xmin = r+xmin +1;
    for(int i = 0; i <= xmax; i++){
        positions[count] = i;
        count++;  }

    for(int j = xmin; j<=r;j++){
      positions[count] = j;
      count++;    }
  }
  else if(xmax > r & xmax > 0 & xmin >=0 & xmin <= r){
    xmax = xmax -r -1;
    for(int i = 0; i<=xmax; i++){
      positions[count] = i;
      count++;
    }
    for(int j = xmin; j <=r;j++){
      positions[count] = j;
      count++;
    }

  }
  else{
    printf("\nError in the function int * positions(), entered in an else and it is not expected. Check the code \n");
    exit(-1);
  }

  return positions;
}

void PrintVector(int * v, int n){
  printf("\n( ");
  for(int i = 0; i <n;i++){
    printf("%d, ", v[i]);
  }
  printf(")\n");
}


void DecrPiles(ROAD * road, int xmin, int xmax, int width){
  //if(xmin < 0 || xmax <0){printf("\n DecrPiles: negative xmin or xmax"); exit(2);}
  if( !((xmin <0 & xmax <0) | (xmin > (*road).length & xmax > (*road).length)) ){
    for(int xpos = max(0,xmin); xpos <= min(xmax, (*road).length);xpos++){
      if(xpos <0 | xpos > (*road).length){printf("\n DecrPiles: negative xpos or greather than road length"); exit(2);}
      DecrPile(road, xpos, width);
    }
   }
}

void DecrPiles_p(ROAD * road,  int xmin, int xmax, int width){


/*
  if( !((xmin <0 & xmax <0) | (xmin > (*road).length & xmax > (*road).length)) ){
    for(int xpos = max(0,xmin); xpos <= min(xmax, (*road).length);xpos++){
      if(xpos <0 | xpos > (*road).length){printf("\n DecrPiles: negative xpos or greather than road length"); exit(2);}
      DecrPile(road, xpos, width);
    }
   }*/


}


void IncrPiles(ROAD * road, int xmin, int xmax, int width){
  //if(xmin < 0 || xmax <0){printf("\n IncrPiles: negative xmin or xmax"); exit(2);}
  
    if( !((xmin <0 & xmax <0) | (xmin > (*road).length & xmax > (*road).length)) ){
    for(int xpos = max(0,xmin); xpos <= min(xmax, (*road).length);xpos++){
      if(xpos <0 | xpos > (*road).length){printf("\n IncrPiles: negative xpos or greather than road length"); exit(2);}
     /* else if( (*road).piles[xpos].blocks[xpos] ){}
      else{*/
          IncrPile(road, xpos, width);//}
    }
  }
}


void IncrPiles_p(ROAD * road, int * positions, int width){
  
   
    /*
    if( !((xmin <0 & xmax <0) | (xmin > (*road).length & xmax > (*road).length)) ){
    for(int xpos = max(0,xmin); xpos <= min(xmax, (*road).length);xpos++){
      if(xpos <0 | xpos > (*road).length){printf("\n IncrPiles: negative xpos or greather than road length"); exit(2);}
 
          IncrPile(road, xpos, width);
  }*/
}

int MoveToNextBump(ROAD road, WHEEL * wheel){
     int poscount = (*wheel).xf;
     while(poscount < road.length &  road.piles[poscount].f <= (*wheel).elevation) poscount++;
     
     //Now poscount have the position where the bump starts, i.e. x0+1
     //printf("\nposcount: %d\n", poscount);
     (*wheel).xf = poscount-1;
     (*wheel).x0 = (*wheel).xf - (*wheel).diameter+1;
     return poscount;
   }

int CurrentBumpHeight(ROAD road, WHEEL wheel){
     int h_index = 0;
     for(int  i = 1; i<= wheel.diameter; i++){
      //printf("\nfor para h_index iter %d \n",i);      
      //PrintRoadWheelInfo(road, wheel);
      //printf("road piles ...: %d\n", road.piles[wheel.xf +i].f );
         if(road.piles[wheel.xf +i].f > wheel.elevation & road.piles[wheel.xf +i].f > road.piles[wheel.xf + h_index].f )
          {h_index=i;                 
                 }
     }
     //printf("\n wheel.xf= %d\n ", wheel.xf);
     //printf("\n wheel.elevation= %d\n ", wheel.elevation);


     //printf("\nAcabado el for para h_index \n");
     //Index where we situate the wheel, just before the next bump if there is
     h_index += wheel.xf;
     //if there is no next bump, h_index will overpass the length of the road
     if(h_index >= road.length){
       printf("\nNo more bumps, h_index exceded the range [0,%d]\n", road.length);
       return -1;
     }

     //printf("\n h_index = %d\n ", h_index);

     int h = road.piles[h_index].f - wheel.elevation;
     if(h < 0){
      printf("\nNegative Bump width\n");
      exit(-1);
     }

     return h;
 }


int L_jump(int Beta, int bump_height){
  //Jump length
  int L= beta*bump_height;
  return L;
}


void JumpIteration(ROAD * road, WHEEL * wheel){
  int L; 
  unsigned short poscount;
  unsigned int h;

  if((*wheel).jumps == 0){
     //L = L_jump(beta, ( (*road).piles[(*wheel).xf].f-(*wheel).elevation) );  
     /*L = (rand() % 6) + d; 

     printf("\nInital L = %d\n", L);*/
    L = 5;
   }

   
   else{ printf("\nAnava a entrar al else de JumpIteration\n") ;exit(0);}
 

  (*wheel).xf += L;
  (*wheel).x0 += L;
  IncrPiles(road, (*wheel).x0- (*wheel).diameter+1, (*wheel).xf - (*wheel).diameter, 1);
     //PrintRoad(road, sep);
  DecrPiles(road, (*wheel).x0+1, (*wheel).xf , 1);
     //PrintRoad(road, sep);
  (*wheel).elevation = (*road).piles[(*wheel).xf+1].f;


  //falta fer el check de si la roda esta o no en la road
  if(!(*wheel).in_road){(*road).n +=1; 
                         (*road).occuped= 0 ;}


  (*wheel).jumps +=1;
  //printf("L = %d\n", L);

}

int Jump(ROAD * road, WHEEL * wheel){
   //search the first bump that the wheel is going to find and update the wheel positions
   char sep[] = "";
   unsigned short poscount = MoveToNextBump(*road, wheel);

   //printf("\nposcount: %d\n", poscount);
   if(poscount >= r){
    printf("\n No bumps found, the wheel is going to leave the road, poscount = %d\n", poscount);
    (*wheel).in_road = 0;
    return -1;
   }
   //calculate h  
   //unsigned int h = CurrentBumpHeight(*road, *wheel);
   int h;
   if( (h = CurrentBumpHeight(*road, *wheel)) == -1){ (*wheel).in_road = 0;
    printf("\n h = -1 returned. h_index exceded road range\n");
   }  

   else if(h != 0){
     int L;
     //Now we are in the current jump and we have to jump:
     L = L_jump(beta, h);
     (*wheel).xf += L;
     (*wheel).x0 += L;
     IncrPiles(road, (*wheel).x0- (*wheel).diameter+1, (*wheel).xf - (*wheel).diameter, 1);
     //PrintRoad(*road, sep);
     DecrPiles(road, (*wheel).x0+1, (*wheel).xf , 1);
     //PrintRoad(*road, sep);
     (*wheel).elevation = (*road).piles[(*wheel).xf+1].f;
     //printf("L = %d\n", L);
     (*wheel).jumps += 1;
 }
  else{
    printf("\nh is equal to zero, no bumps found\n"); exit(0);

  }
  //printf("h: %d", h);

  return 0;

}

void InitialiseWheel(ROAD * road, WHEEL * wheel, int xf, int n){
   (*wheel).diameter = d;
   (*wheel).xf = xf;
   (*wheel).x0 = xf-(*wheel).diameter+1;
   (*wheel).elevation = (*road).piles[(*wheel).xf+1].f;
   (*wheel).in_road = 1;
   (*wheel).jumps = 0;
   (*road).n = n;
   (*road).occuped =1;
}






int main(){

  //Create, initialise file
  FILE *f = fopen(F_filename, "w");
  if (f == NULL){printf("Error opening file!\n");exit(1); }

  FILE *f2 = fopen("InitialRoad.txt", "w");
  if (f2 == NULL){printf("Error opening initialroad file!\n");exit(1); }

    
  //Initialise the ROAD 
   ROAD road;
   char sep[] = "";
   int depth = 6;
   InitialiseRoad(&road, depth, ' ', '.');

   IncrPile(&road, 0, 1);

   //NEW PROFILE
   IncrPile(&road, 25, 1);
   IncrPile(&road, 40, 1);
   IncrPile(&road, 70, 1);

   IncrPile(&road, 100, 1);
   IncrPile(&road, 140, 1);

   //OLD PROFILE
   /*IncrPiles(&road, 24,28 ,1);
   IncrPile(&road, 25, 1);
   IncrPile(&road, 27, 2);
   IncrPiles(&road, 40, 42, 1);
   IncrPile(&road, 41, 1);
   IncrPile(&road, 47, 1);*/
   PrintRoad(road, sep);


   


   //Initialise the WHEEL
   WHEEL wheel;
   InitialiseWheel(&road, &wheel, 0, 1);    
   int L, nmax = 2;
   /*int iter;   int itermax =  10;
   
   while(road.n < nmax)
  {  
   for(int j = 0; j <2;j++){
    iter = 0;
    //InitialiseWheel(&road, &wheel, 0, 1);
    JumpIteration(&road, &wheel);
    PrintRoad(road, sep);
    while(wheel.in_road & iter <=itermax){
     Jump(&road, &wheel);
     iter++; 
   }
 }
*/
  //}
   //WriteMatrixToFile(&f2,road);

   JumpIteration(&road, &wheel);
   //JumpIteration(&road, &wheel);
   //PrintRoadWheelInfo(road, wheel);
   PrintRoad(road, sep);

   int iter = 0;   int itermax =  10;
   while(wheel.in_road & iter <=itermax){
     Jump(&road, &wheel);
     iter++;
   }


   /* Intializes random number generator */
   time_t t;
   //srand((unsigned) time(&t));
   srand((unsigned) 2);

   /* Print 5 random numbers from 0 to 49 */
   /*for(int i = 0 ; i < 10 ; i++ ) {
      printf("%d\n", (rand() % 9)+1 );
   }*/


   int x0rand, wmax = 35;
   itermax = 10;
  for(int w = 1; w <=wmax;w++){
     printf("\n w = %d\n",w); /*
     PrintRoadWheelInfo(road, wheel);
     PrintRoad(road, sep);*/

     x0rand = (rand() % 9) +1;
     //printf("\n x0rand %d \n", x0rand);
     InitialiseWheel(&road, &wheel, x0rand, w+1);
     if(wheel.xf == 99){printf("iter %d  %d\n", w, wheel.xf);}
      //if(w == 26){ PrintRoadWheelInfo(road, wheel);}
     //PrintRoadWheelInfo(road, wheel);
     //if(w == 26){ PrintRoad(road, sep);}
     JumpIteration(&road, &wheel);
     //if(w == 26){ PrintRoadWheelInfo(road, wheel);
     //printf("???????????");
     //PrintRoad(road, sep);
   //}

     iter = 0;
     while(wheel.in_road & iter < itermax){
       //printf("\n jump number = %d\n", wheel.jumps);
       Jump(&road, &wheel); //ERROR EN EL PRIMER JUMP
       iter++;
     }
     if(iter >= itermax){wheel.in_road = 0;};
     PrintRoad(road, sep);
}

  
    PrintRoad(road, sep);


   PrintRoadWheelInfo(road, wheel);
   PrintRoad(road, sep);
   WriteMatrixToFile(&f,road);






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