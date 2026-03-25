# ENGR 445 - Lab 02 - GPIO, Timers, and Interrupts

> Important: This lab solution uses multiple source files: `app.cpp`, `GPIO.h`,`Timer.h`,
> and `Helper.h`.
>
> All three files must be included/used in your project, and all must be
> submitted to Gradescope.

## Overview
In this lab you will utilize the GPIO peripherals on the Giant Gecko to read in
the state of a push button and control external LEDs. This assignment requires
you to combine knowledge across the GPIO, Timers, and Interrupts modules.

Lab solution must be completely interrupt-driven; no source code should be
placed in `app_action()`.

## Required Tasks (10 pts)
- Using PB0, cycle LED0 through blinking cycles of 1, 2, and 5 Hz with 50%
	duty cycle.
- At boot, LED0 should be off or already blinking at 1 Hz.
- After each push/release of PB0, LED0 should blink at the next pattern:
	2 Hz, then 5 Hz, then back to 1 Hz.
- A 1 Hz duty cycle implies the LED is on for 1/2 second and then off for
	1/2 second. A 2 Hz cycle is on for 1/4 second then off for 1/4 second, etc.

## Bonus Tasks (+5 pts)
- PB1 will be used to control LED1.
- When PB1 is pressed, calculate how long PB1 was held down and then illuminate
	LED1 for that amount of time.
- LED1 should be off after reboot.
- Example: if PB1 is held down for 0.5s, LED1 should illuminate for 0.5s and
	then turn off.
- The timer should be able to measure durations of at least 1.4 seconds.
- The functions of PB0/LED0 and PB1/LED1 should operate independently.

## Hints
- Use the GPIO functions provided in `GPIO.h` to make your code easier to read
	and understand. They will also be helpful in future labs.
- Your two buttons operate independently. You will likely need two GPIO
	interrupt handlers and two Timer interrupt handlers. Plan out how you wish to
	allocate the hardware and interrupts.
- Pre-calculate the TOP values required for the blink delays. Keep them in a
	const array for easy access.

## Submission
Submit all source code files (including `.cpp` and `.h`) to Gradescope.


