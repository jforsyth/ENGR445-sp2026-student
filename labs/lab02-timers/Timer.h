/*
 * Timer.h
 *
 *  Created on: Jun 7, 2017
 *      Author: Jason Forsyth
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include <em_timer.h>

/**
 * TOP constants assuming 48MHz clock and 1024 prescaler
 */
#define ONE_SECOND 46875u
#define HALF_SECOND 23437u
#define QUARTER_SECOND 11718u
#define TENTH_SECOND 4687u

#endif /* SRC_TIMER_H_ */
