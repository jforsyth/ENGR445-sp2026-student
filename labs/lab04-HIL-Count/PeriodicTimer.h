/*
 * PeriodicTimer.h
 *
 *  Created on: Apr 29, 2024
 *      Author: jforsyth
 */

#ifndef PERIODICTIMER_H_
#define PERIODICTIMER_H_

#include "em_timer.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

class PeriodicTimer
{
public:
  PeriodicTimer ();
  void initialize();

  /**
   * Dummy wait for a fixed interval
   * @param duration_ms Duration to wait in milliseconds
   */
  void wait(unsigned int duration_ms);
  //bool sleep(unsigned int duration);

  virtual
  ~PeriodicTimer ();

private:
  TIMER_TypeDef*  timer = TIMER0;
  const unsigned MAX_TOP_VALUE = 0xffff;
  const unsigned int PRESCALER = 1024;
  bool initialized = false;
};

#endif /* PERIODICTIMER_H_ */
