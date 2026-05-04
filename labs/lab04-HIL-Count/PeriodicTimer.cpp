/*
 * PeriodicTimer.cpp
 *
 *  Created on: Apr 29, 2024
 *      Author: jforsyth
 */

#include <PeriodicTimer.h>

/**
 * TIMER0 interrupt handler. Will pop on any
 * interrupt enabled with TIMER0
 */
void TIMER0_IRQHandler(void)
{
  //CMSIS implementation handles clearing NVIC interrupt flag

  //must clear the flag or we will keep ending up here
  TIMER0->IFC=TIMER0->IFC|0x1;

  //stop the timer
  TIMER0->CTRL=0x1;

  //return nothing
  return;

}

PeriodicTimer::PeriodicTimer ()
{
  // TODO Auto-generated constructor stub

}

PeriodicTimer::~PeriodicTimer ()
{
  // TODO Auto-generated destructor stub
}

void PeriodicTimer::initialize()
{
  //setup all clocks

  //enable the specific clock for the TIMER0
  CMU_ClockEnable(cmuClock_TIMER0, true);

  //set timer MODE UP
  //value 0x0, bit pos [0]

  //set timer clksel HFPER
  //value 0x0, bit pos [0]

  //set prescale to 1024
  //value 0xA, bit pos [27:24]
  timer->CTRL=((timer->CTRL)&~(0xf<<24))|(0xA<<24);

  //enable debugRun
  //value 0x1, bit pos 6
  timer->CTRL=(timer->CTRL)|(0x1<<6);

  //enable interrupts?
  //@TODO

  //configure sleep?
  //@TODO

}

void PeriodicTimer::wait(unsigned int duration_ms)
{
  unsigned int timerClock = CMU_ClockFreqGet(cmuClock_TIMER0);

  unsigned int top_value = (float)duration_ms/1000.0 * (float)timerClock / (float)PRESCALER;

  if (top_value > MAX_TOP_VALUE)
    {
      top_value = MAX_TOP_VALUE;
    }

  //clear timer
  timer->CNT=0x0;

  //set top
  timer->TOP=top_value;

  //start timer
  timer->CMD|=0x1;

  //wait until the IF flag occurs
  while((timer->IF&0x1)==0){}

  //stop the timer
  timer->CMD=(0x1)<<1;

  //clear the IF flag
  timer->IFC=(timer->IFC & ~(0x1))|0x1;

  //return
  return;
}
