// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "DAC.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delayms(unsigned long msec);
int main(void){ // Real Lab13 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
//	DAC_Init();
  EnableInterrupts();  // enable after all initialization are done
	unsigned long input,previous;
	previous = Piano_In();
  while(1){ unsigned long i; // static debugging
//     for(i=0;i<16;i++){
//       DAC_Out(i);
//       delayms(10); // connect PD3 to DAC output
//     }
		// input from keys to select tone
		input = Piano_In();             // input from piano keys on PE3-0
//		if ( (input == 0x01) && (previous == 0) ){  // just pressed
//			Sound_Tone(C);
//		}
		if (input == 0x01){  // just pressed
			Sound_Tone(C);
		}
		if (input == 0x02){  // just pressed
			Sound_Tone(D);
		}
		if (input == 0x04){  // just pressed
			Sound_Tone(E);
		}
		if (input == 0x08){  // just pressed
			Sound_Tone(G);
		}
		if (previous && (input == 0)){  // just released
			Sound_Off();
		}
		previous = input;
    delayms(10);        // 10 ms to remove switch bounce
  }
            
}

// Inputs: Number of msec to delay
// Outputs: None
void delayms(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


