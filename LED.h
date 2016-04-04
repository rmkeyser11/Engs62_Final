/*
 * LED.h - routines for initializing and controlling LEDs on the STM3210C-EVAL board
 */
#pragma once

#include "stdint.h"       /* define uint16_t, etc... */

#define LED_GREEN	   (0x00001000)
#define LED_ORANGE     (0x00002000)
#define LED_RED        (0x00004000)
#define LED_BLUE       (0x00008000)

#define LED_GREEN_ON   (0x00001000)
#define LED_ORANGE_ON  (0x00002000)
#define LED_RED_ON     (0x00004000)
#define LED_BLUE_ON    (0x00008000)

#define LED_GREEN_OFF  (0x10000000)
#define LED_ORANGE_OFF (0x20000000)
#define LED_RED_OFF    (0x40000000)
#define LED_BLUE_OFF   (0x80000000)

/*
 * void LED_init(void)
 *
 * Configure GPIOD pins 15:12 as output push-pull to drive the LEDs
 */
void LED_init(void);

/*
 * void LED_update(uint32_t LED)
 *
 * Update LED states based on value of LED.
 * LED should be set using LED_<color>_ON or LED_<color>_OFF #defines
 */
void LED_update(uint32_t LED);

/*
 * void LED_toggle(uint32_t LED)
 *
 * Toggle an individual LED's state.
 * LED should be set using LED_<color>
 */
void LED_toggle(uint32_t LED);
