#include "D:\Masters Parhai\2nd Semester\Embedded Systems\Lab2_Final\main.h"


#include <stdlib.h>
#define C0 PIN_B7
#define C1 PIN_B6
#define C2 PIN_B5
#define EN PIN_B4
#define R0 PIN_B3
#define R1 PIN_B2
#define R2 PIN_B1
#define R3 PIN_B0
#define B1 PIN_D1
#define B2 PIN_D0



void RLED(int i) {
 if (bit_test (i, 0)) output_high(R0); else output_low(R0);
 if (bit_test (i, 1)) output_high(R1); else output_low(R1);
 if (bit_test (i, 2)) output_high(R2); else output_low(R2);
 if (bit_test (i, 3)) output_high(R3); else output_low(R3);
}
void CLED(int i) {
 if (bit_test (i, 0)) output_high(C0); else output_low(C0);
 if (bit_test (i, 1)) output_high(C1); else output_low(C1);
 if (bit_test (i, 2)) output_high(C2); else output_low(C2);
}
void LED (int x1,int y1) {
 output_high(EN);
 RLED(15-y1);
 CLED(7-x1);
 output_low(EN);
}

int rand_gen(int min, int max) {
   return (rand() % (max - min + 1)) + min;
}

#use rtos(timer=1, minor_cycle=50us)
static int boat_row = 0;                      // storing location for dynamic movement


#define NUM_PARAS 8
#define BOAT_SIZE 4
static int game_over = 0;
static int para_trust[NUM_PARAS];
static int para_row[NUM_PARAS];
static int para_speed[NUM_PARAS];
static int para_counter[NUM_PARAS];
static int para_index = 0;
static int score = 0;
static int miss = 0;

#task (rate=250us, max=50us)
void boat (){

   static int boat_index = 0;
   
   switch(boat_index) {
   case 0: LED(boat_row,15); break;
   case 1: LED((boat_row + 1),15); break;
   case 2: LED((boat_row + 2),15); break;
   case 3: LED((boat_row+ 3),15); break;
   }
   
   
   boat_index++;
   if(boat_index >= 4){
      boat_index = 0;
   }
   
}

#task (rate=250us, max=50us)
void paratrooper() {

   if (para_trust[para_index] == 1){
      LED(para_index, para_row[para_index]);
   }
   
   
   para_index++;
   if(para_index >= NUM_PARAS) {
      para_index = 0;
   }
}


#task (rate=250us, max=50us)
void boat_control() {
   
   if(input(PIN_D7)) {     //added extra condition to avoid double press ambiguity
      if(boat_row > 0)
         boat_row--;
         RTOS_AWAIT(!input(PIN_D7));
   }
   if (input(PIN_D0)) {
      if(boat_row < 4)
         boat_row++;
         RTOS_AWAIT(!input(PIN_D0));
   }
}

#task (rate=10ms, max=50us)
void paratrooper_update() {

   if(game_over)
   
      return;
   int i;

   for(i = 0; i < NUM_PARAS; i++){

   if(para_trust[i] == 0)
      continue;
      
      para_counter[i]++;

      if(para_counter[i] >= para_speed[i]){

         para_counter[i] = 0;
         para_row[i]++;

         if(para_row[i] > 15){
         
            if ( i >= boat_row && i < (boat_row + BOAT_SIZE)) {
               score++;
               printf("Score: %d \r\n", score);
               
               para_row[i] = 0;
               para_speed[i] = rand_gen(10,40);
            }
            else {
               miss++;
               printf("Miss: %d\r\n", miss);
               
               para_trust[i] = 0;
               
               if (miss >= 3 ) {
                  printf("*******The GAME IS OVER************\r\n");
                  printf("Final Score: %d \r\n",score);
                  game_over=1;
               }
            }

            para_row[i] = 0;   // restart from TOP
            para_speed[i] = rand_gen(10,40);
         }
      }
   }
}

#task (rate=20ms, max=50us)
void restart_game() {

   if(!game_over)
      return;

   if(input(PIN_D7) || input(PIN_D0)) {

      int m;

      score = 0;
      miss = 0;
      boat_row = 0;

      for(m = 0; m < NUM_PARAS; m++) {
         para_row[m] = 0;
         para_speed[m] = rand_gen(10,40);
         para_counter[m] = 0;
         para_trust[m] = 1;      // ?? THIS resets rows
      }

      game_over = 0;

      RTOS_AWAIT(!(input(PIN_D7) || input(PIN_D0)));
   }
}

void main(){

   
   int j;
   
   srand(1);
   
   for(j = 0; j < NUM_PARAS; j++) {
      para_row[j] = 0;
      para_speed[j] = rand_gen(10,40);
      para_counter[j] = 0;
      para_trust[j] = 1;
   }
   

   rtos_run();
   
}



