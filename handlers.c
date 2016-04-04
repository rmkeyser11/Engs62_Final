/*
 * handlers.c
 *
 *  Created on: Mar 6, 2016
 *      Author: matthew
 */

#include "LED.h"

void __attribute__ ((interrupt)) fault_handler(void) {
	LED_update(LED_RED_ON);
	while (1);
}
