// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define PE0											(*((volatile unsigned long *)0x40024004))
#define PE1											(*((volatile unsigned long *)0x40024008))
	
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

// ***** 2. Global Declarations Section *****
unsigned long SW;  // input from PE0

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay1ms(unsigned long msec); // Delay in ms
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ unsigned long volatile delay;
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  SYSCTL_RCGC2_R |= 0x10;           // Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTE_DIR_R |= 0x02;         // PE1 output
  GPIO_PORTE_DIR_R &= ~0x01;        // PE0 input 
  GPIO_PORTE_AFSEL_R &= ~0x17;      // not alternative
  GPIO_PORTE_AMSEL_R &= ~0x17;      // no analog
  GPIO_PORTE_PCTL_R &= ~0x000F0FFF; // bits for PE4,PE2,PE1,PE0
  GPIO_PORTE_DEN_R |= 0x17;         // enable PE4,PE2,PE1,PE0
	
  EnableInterrupts();           // enable interrupts for the grader
	SysTick_Init();
	//GPIO_PORTE_DATA_R |= 0x02;
	//PE1 = 0x02;	// turn LED on
  while(1){
		SW = GPIO_PORTE_DATA_R&0x01;
		if (SW) {
			GPIO_PORTE_DATA_R ^= 0x02;   // toggle output
		} else {
			GPIO_PORTE_DATA_R |= 0x02;
		}
		// Delay1ms(100);  // 100ms delay makes a 5Hz period
		// SysTick_Wait(8000000);
		SysTick_Wait10ms(50); // wait 10*10ms = 0.1 sec
	}
  
	
	  //if (PE0 == 0x01) { // switch pressd, toggle LED
		//	  PE1 ^= 0x02;
	  //} else {
	  //	PE1 = 0x02;      // switch released, turn on LED
	  //}
	  //Delay1ms(100);  // 100ms delay makes a 5Hz period
		//}
			
}



void Delay1ms(unsigned long msec){
// write this function
  msec = msec*727240*2/91;  // 0.001sec
  while(msec){
		msec--;
  }
}


// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

// Time delay using busy wait.
// The delay parameter is in units of the core clock. 
void SysTick_Wait(unsigned long delay){
  volatile unsigned long elapsedTime;
  unsigned long startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}
// Time delay using busy wait.
// This assumes 16 MHz system clock.
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(160000);  // wait 10ms (assumes 16 MHz clock)
  }
}
