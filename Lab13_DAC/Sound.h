// Sound.h
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also initializes DAC
// Input: none
// Output: none
void Sound_Init(void);

// **************Sound_Tone*********************
// Change SysTick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period);


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void);

// the 32 comes from the length of the sine wave table
// bus is 80 MHz
// Period =  80000000/32/Freq=2500000/Freq
#define C 4778   // 523.251 Hz
#define D 4257   // 587.330 Hz
#define E 3792   // 659.255 Hz
#define G 3189   // 783.991 Hz
