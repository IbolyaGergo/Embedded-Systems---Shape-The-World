// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"

#define LIGHT                   (*((volatile unsigned long *)0x400050FC)) //PB5-0
#define SENSOR                  (*((volatile unsigned long *)0x4002401C)) //PE2-0
#define WALK                    (*((volatile unsigned long *)0x40025028)) //PF3,1

// ***** 2. Global Declarations Section *****
// Linked data structure
struct State {
  unsigned long Out; 
  unsigned long Time;  
  unsigned long Next[8];
  unsigned long Pedestrian;}; 
typedef const struct State STyp;
#define goN    0
#define waitN  1
#define goE    2
#define waitE  3
#define goP    4
#define waitP0 5
#define waitP1 6
#define waitP2 7
STyp FSM[8]={
 {0x21,300,{goN,waitN,goN,waitN,waitN,waitN,waitN,waitN},0x02},  // 3 sec
 {0x22, 50,{goE,goE,goE,goE,goP,goE,goP,goE},0x02},      // 0.5 sec
 {0x0C,300,{goE,goE,waitE,waitE,waitE,waitE,waitE,waitE},0x02},
 {0x14, 50,{goN,goN,goN,goN,goP,goP,goP,goP},0x02},
 {0x24,300,{goP,waitP0,waitP0,waitP0,goP,waitP0,waitP0,waitP0},0x08},
 {0x24, 50,{waitP1,waitP1,waitP1,waitP1,waitP1,waitP1,waitP1,waitP1},0x00},
 {0x24, 50,{waitP2,waitP2,waitP2,waitP2,waitP2,waitP2,waitP2,waitP2},0x08},
 {0x24, 50,{goN,goE,goN,goN,goN,goE,goN,goN},0x02}};
unsigned long S;  // index to the current state 
unsigned long Input; 

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortE_Init(void);
void PortB_Init(void);
void PortF_Init(void);

// ***** 3. Subroutines Section *****
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x10;      // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;      // 2) no need to unlock
  GPIO_PORTE_AMSEL_R &= ~0x07; // 3) disable analog function on PE2-0
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // 4) enable regular GPIO
  GPIO_PORTE_DIR_R &= ~0x07;   // 5) inputs on PE2-0
  GPIO_PORTE_AFSEL_R &= ~0x07; // 6) regular function on PE2-0
  GPIO_PORTE_DEN_R |= 0x07;    // 7) enable digital on PE2-0
}
void PortB_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x02;      // 1) activate clock for Port B
  delay = SYSCTL_RCGC2_R;      // 2) no need to unlock
  GPIO_PORTB_AMSEL_R &= ~0x3F; // 3) disable analog function on PB5-0
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // 4) enable regular GPIO
  GPIO_PORTB_DIR_R |= 0x3F;    // 5) outputs on PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F; // 6) regular function on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;    // 7) enable digital on PB5-0
}
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R |= 0x0A;         // 5) PF3, 1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_DEN_R |= 0x0A;         // 7) enable digital I/O on PF3, 1
}

int main(void){ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
  PLL_Init();       // 80 MHz, Program 10.1
  SysTick_Init();   // Program 10.2
	
	PortE_Init();
	PortB_Init();
	PortF_Init();
  
  EnableInterrupts();
	S = goN; 
  while(1){
    LIGHT = FSM[S].Out;  // set lights for cars
		WALK = FSM[S].Pedestrian; //set light for pedestrians
    SysTick_Wait10ms(FSM[S].Time);
    Input = SENSOR;     // read sensors
    S = FSM[S].Next[Input];
  }
}

