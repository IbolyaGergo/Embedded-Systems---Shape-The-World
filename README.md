This repo contains my completed labs for the two-part course on embedded systems
- [UTAustinX: Embedded Systems - Shape The World: Microcontroller Input/Output](https://www.edx.org/learn/embedded-systems/the-university-of-texas-at-austin-embedded-systems-shape-the-world-microcontroller-input-output)
- [UTAustinX: Embedded Systems - Shape The World: Multi-Threaded Interfacing](https://www.edx.org/learn/embedded-systems/the-university-of-texas-at-austin-embedded-systems-shape-the-world-multi-threaded-interfacing).

The courses build gradually from simple interfacing of switches and LEDs to complex concepts like a traffic light controller, display drivers,
digital to analog conversion, generation of sound, analog to digital conversion, motor control, graphics, interrupts, and communication.
They teach how to program microcontrollers in C by directly accessing the registers, debugging using logic analyzer and oscilloscope.
Laboratory assignments are first performed in simulation, and then built and debugged on the real microcontroller.

The labs were completed using an Arm Cortex M4 based Texas Instruments TM4C123 microcontroller, some electronic components and the Keil uVision IDE.
The instructions about purchasing the kit and installing required software can be found [here](http://edx-org-utaustinx.s3.amazonaws.com/UT601x/index.html).

Brief description of the labs:
- Lab 2. - Run existing project on LaunchPad with switch input and LED output
- Lab 4. - Debug a system with two inputs and two outputs
- Lab 5. - Write a C function and perform input/output
- Lab 6. - Write C software that inputs from a switch and toggles an LED output
- Lab 7. - Write C functions that inputs from a switch and outputs to two LEDs, which is a virtual pacemaker
- Lab 8. - Interface an external switch and LED and write input/output software
- Lab 9. - Write C functions using array data structures that collect/debug your system
- Lab 10. FSM - Interface 3 switches and 6 LEDs and create a traffic light finite state machine
- Lab 11. UART - Write C functions that output decimal and fixed-point numbers to serial port
- Lab 12. Interrupts - Produce a 440 Hz tone using SysTick interrupts
- Lab 13. DAC and Sound - Design and test a digital piano, with 4 inputs, digital to analog conversion, and sound
- Lab 14. ADC and Data Acquisition - Design and test a position measurement, with analog to digital conversion and calibrated output
- Lab 15. Systems Approach to Game Design - Design and test a hand-held video game, which integrates all components from previous labs

You can find more detailed descriptions of the labs by checking the *.c files. Alternatively, you can open the project files *.uvproj using the Keil uVision IDE ([download here](https://www.keil.com/demo/eval/arm.htm)).
