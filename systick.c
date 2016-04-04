/*
 * systick.c
 *
 *  Created on: Feb 9, 2016
 *      Author: matthew
 */

#include "stm32f4xx.h"
#include "stdint.h"

/*
 * void systick_init(int timer_count)
 *
 * Configure systick to enable interrupts at timer_count clock cycle intervals
 */
void systick_init(uint32_t timer_count) {
	uint32_t ctrl_val = 0;
	uint32_t reload_val = 0;
	reload_val = timer_count & STK_LOAD_RELOAD_MASK;
	STK->STK_LOAD = reload_val;
	// Use internal clock
	ctrl_val |= (STK_CTRL_CLKSOURCE_MASK & ONES);
	// Enable systick interrupts
	ctrl_val |= (STK_CTRL_TICKINT_MASK & ONES);
	// Enable counting
	ctrl_val |= (STK_CTRL_ENABLE_MASK & ONES);
	STK->STK_CTRL = ctrl_val;
}
