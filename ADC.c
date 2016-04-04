/*
 * ADC.c
 *
 * * Enable potentiometers on ports PA1, PA2, PA3, PA4, and PA5
 * via channels 1-5 of the ADC1
 *
 * PA1 = ADC1_CH1
 * PA2 = ADC2_CH1
 * PA3 = ADC3_CH1
 * PA4 = ADC4_CH1
 * PA5 = ADC5_CH1
 *
 * Documentation references:
 * [1]: STM32F40x Reference Manual
 * [2]: STM32F407VG Datasheet
 * [3]:
 *  Created on: Feb 9, 2016
 *      Author: matthew
 */

#include "stm32f4xx.h"
#include "io.h"
#include "DMA.h"

int initialized = 0;

void ADC_init(void) {


	/*******************************************
	 * Enable clocks
	 *******************************************/
	// Enable clock for GPIOA
	// set bit 0 of AHB1ENR to high
	// [1] 6.3.12 p.145
	RCC->AHB1ENR |= 1; // Enable clock

	// Enable clock of ADC1
	// set bit 8 of APB2ENR to high
	// [1] 6.3.17 p.154
	RCC->APB2ENR |= 1 << 8;

	/*******************************************
	 * Configure GPIO pins
	 *******************************************/
	// Set pins PA1-PA5 to analog
	// set bits 2-11 of MODER register to high
	// [1] 7.4.1 p.198
	GPIOA->MODER |= 0x3FF << 2;

	/*******************************************
	 * Configure ADC1 for 5 channels
	 *******************************************/

	// Set the regular sequence to length 5
	// set bits 23:20 of SQR1 to 0b0100
	// [1] 11.13.9 p. 296
	ADC1->ADC_SQR1 = (ADC1->ADC_SQR1 & ~0xF00000) | 0x400000;

	// Setting the channel and pin for each conversion
	// Clear bits 0:24 and set 1st conversion in
	// regular sequence to pin 1, 2nd conversion to
	// pin 2, etc. until 5th to pin 5
	// 11.13.11 p. 297
	uint32_t val;
	val = ADC1->ADC_SQR3 & ~0x1FFFFFF; // clear those bits
	val |= 0x01; // set 1st to pin 1
	val |= 0x02<<5; // set 2nd to pin 2
	val |= 0x03<<10; // set 3rd to pin 3
	val |= 0x04<<15; // set 4th to pin 4
	val |= 0x05<<20; // set 5th to pin 5
	ADC1->ADC_SQR3 = val;

	// Set the sampling time to 15 cycles
	// set SMP1-5 to 0b001 in SMPR2
	// 11.13.5 p. 294
	// Assume 10k ohms is the worst case resistance
	// Using Equation 1 from 5.3.20 [2]
	// k = 10.43 (solved from other)
	// f_ADC = 16 MHz
	// C_ADC = 4 pF (typ)
	// N = 12 (bits of resolution)
	// R_ADC = 6 kOhms
	ADC1->ADC_SMPR2 &= ~(0x7<<3); // clear those bits
	ADC1->ADC_SMPR2 |= (1<<3); // set to 15 cycles

	ADC1->ADC_SMPR2 &= ~(0x7<<6); // clear those bits
	ADC1->ADC_SMPR2 |= (1<<6); // set to 15 cycles

	ADC1->ADC_SMPR2 &= ~(0x7<<9); // clear those bits
	ADC1->ADC_SMPR2 |= (1<<9); // set to 15 cycles

	ADC1->ADC_SMPR2 &= ~(0x7<<12); // clear those bits
	ADC1->ADC_SMPR2 |= (1<<12); // set to 15 cycles

	ADC1->ADC_SMPR2 &= ~(0x7<<15); // clear those bits
	ADC1->ADC_SMPR2 |= (1<<15); // set to 15 cycles

	// Enable EOC signal for each member of the sequence (NOPE, WE'RE USING DMA)
	// rather than just at the end of the sequence
	// Set bit 10 of ADC_CR2 to high
	// Ref: [1] 11.13.3 p.291
	ADC1->ADC_CR2 |= 1 << 10;

	//Enable scan mode
	//set bit 8 of the CR1 high
	// 11.13.2 p. 290
	ADC1->ADC_CR1 |= 1 << 8;

	// Enable DMA mode
	// set bit 8 (DMA) in ADC_CR2 to high
	// Ref: [1] 11.13.3, p.293
	ADC1->ADC_CR2 |= 1 << 8;

	// Enable conversions
	// set the ADON bit in CR2 to high
	// 11.13.3 p. 293
	ADC1->ADC_CR2 |= 0x1; // Enable ADC
	initialized = 1;
}

/**
 * Accepts a 5-element integer array to store the data
 */
void ADC_read(uint32_t *data) {
	if (!initialized) {
		return;
	}

	// Set DMA bit to 0 then to 1 to reenable
	// Ref: [1] 11.8.1, p.274
	ADC1->ADC_CR2 &= ~(1 << 8);
	ADC1->ADC_CR2 |= 1 << 8;

	// Set DMA stream location
	start_DMA_transfer(data);

	// Clear EOC bit
	ADC1->ADC_CR2 |= 1 << 30; // set bit 30 (SWSTART)

	// Tight loop until finished
//	while (!(ADC1->ADC_SR & ADC_SR_EOC));

	if (ADC1->ADC_SR & (1 << 5)) { // if OVR (overrun) bit
		print_string("Overflow\n");
	}
}
