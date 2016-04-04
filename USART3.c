/*
 * USART3.c - USART3 support on the STM32F4-Discovery Board
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 * REFERENCES:
 *
 * All documentation references written as "[#]-<section>" where:
 *
 * [1] = RM0090 STM32F407 Reference Manual.pdf
 * [2] = STM32F4 Discovery Schematic.pdf
 * [3] = STM32F407xx Data Sheet.pdf
 * [4] = DM00046982 PM0214 STM32F4xxx Programming Manual.pdf
 *
 * Created on: February 28, 2016
 * Author: Matthew West
 */

#include "stm32f4xx.h"

void USART3_init(void) {
	/* We'll run USART3 through ports PD8 (TX) and PD9 (RX)
	 * [3] Table 8, p59
	 */

	/* Enable GPOID peripheral clock
	 * Set bit 3 of RCC_AHB1ENR to high
	 * See: [1]-6.3.12
	 */
	RCC->AHB1ENR |= 0x8;

	/* Enable USART3 peripheral clock
	 * Set bit 18 of RCC_APB1ENR to high
	 * See: [1]-6.3.16
	 */
	RCC->APB1ENR |= 1 << 18;

	/*
	 * Configure GPIOD Pin 8 (TX) as:
	 *   Alternate function output, AF7
	 *   Push-pull
	 *   No pull-up/pull-down
	 *   2 MHz drive speed
	 *
	 * See [1]-7.4 and [3]-Table 8 (p 59).
	 */

	/* Set pin #8 as Alternate Function
	 * Set bits 17:16 to '10'
	 * See [1]-7.4.1
	 */
	GPIOD->MODER = (GPIOD->MODER & ~0x30000) | 0x20000;

	/* Set pin #9 as alternate function
	 * Set bits 19:18 to '10'
	 * See [1]-7.4.1
	 */
	GPIOD->MODER = (GPIOD->MODER & ~0xC0000) | 0x80000;

	/* Default settings on reset are push-pull, 2MHz, no pull-up/pull-down */

	/* Pin 9 is configured as input floating on reset - rely on defaults */

	/* Set pin #8 and #9 MUX to Alternate Function #7 (USART3 TX/RX) */
	/* See	[3]-Table 8 (pp 60)
	 * 		[1]-7.4.10
	 */
	// Set pin 8 to AF7
	GPIOD->AFRH = (GPIOD->AFRH & ~0xF) | 0x7;
	// Set pin 9 to AF7
	GPIOD->AFRH = (GPIOD->AFRH & ~0xF0) | 0x70;


	/* Enable the USART peripheral in the USART (separate from clock enable)
	 * Set bit 13 in USART_CR1
	 * See	[1]-26.6.4
	 */
	USART3->USART_CR1 |= 0x2000;

	/* Set the baudrate to 57600.
	 * SYSCLK = 16 MHz:
	 * 16,000,000 / 57,600 = 278
	 *
	 *
	 * See	[1]-26.6.3
	 * Following USART2 code given - doesn't make sense with documentation but I know it works
	 */
//	USART3->USART_BRR = 0xFFFF & 1667; // baud rate 9600
//	USART3->USART_BRR = 0xFFFF & 278; // baud rate 57600
	USART3->USART_BRR = 0xFFFF & 139; // baud rate 57600

	/* Configure interrupts *from* the USART
	 * Set bit 5 in USART3_CR1
	 * RXNE interrupt enable
	 * See	[1]-26.6.4
	 */
	USART3->USART_CR1 |= 1 << 5;

	/* Configure the NVIC to pass interrupts *from* the USART *to* the processor
	 * Enable position 39 in the vector table [1]-10.2 p.249
	 * Set bit 7 in NVIC_ISER1 [4]-4.3.11 p.205
	 * */
	uint32_t *NVIC_ISER1 = (uint32_t*)0xE000E104;
	*NVIC_ISER1 |= 0x80;

	/* Enable transmitter and receiver
	 * Set bits 3:2 to high
	 * See [1]-26.6.4 pp.782-783
	 */
	USART3->USART_CR1 |= 0xC;
}



void USART3_send(char c) {
 	/* Wait for USART transmit shift register to be empty */
	uint32_t done_flag = 1 << 7;
	while (!(USART3->USART_SR & done_flag));
 	USART3->USART_DR = 0xFF & c;
}


char USART3_recv(void) {
	// Wait for a bit to be received
	while (!(USART3->USART_SR & 0x20));

	char c = USART3->USART_DR & 0xFF;
	return c;
}
