/*
 * DMA.c
 *
 * Configure the DMA2 (Direct memory Access) for the ADC to read bytes into
 *
 * References:
 * [1] DM00031020 RM0090 STM32F40x Reference Manual.pdf
 * [2]
 *  Created on: Mar 6, 2016
 *      Author: matthew
 */
#include "stm32f4xx.h"

void DMA_init(void) {
	// Enable clock to DMA2
	// Set bit 22 (DMA2EN) of RCC_AHB1ENR to high
	// [1] 6.3.12 p.145
	RCC->AHB1ENR |= 1 << 22;

	// Set DMA2 to peripheral-to-memory mode
	// Set bits 7:6 (DIR) of DMA_S0CR to '00'
	// [1] 9.5.5 p.239
	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0xC0) | 0;

	// Set DMA2 stream 0 to channel 0, ADC1
	// Ref: [1] 9.3.3, Table 34, p.217
	//
	// Set bits 27:25 (CHSEL) of DMA_S0CR to '000'
	// Ref: [1] 9.5.5, p.237
	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0xE000000) | 0;

	// Set the size of the peripheral read to 16 bits (half-word)
	// Set bits 12:11 (PSIZE) in DMA_S0CR to '01'
	// Ref: [1] 9.5.5, p.237 (DMA)
	//		[1] 11.13.14, p.298 (ADC)
	// Try it at 32 bits
//	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0x1800) | 0x800; 16 bits
	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0x1800) | 0x1000; // 32 bits

	// Set the size of memory write to 16 bits (half-word)
	// Set bits 14:13 (MSIZE) in DMA_S0CR to '01'
	// Ref: [1] 9.5.5, p.237
	// Try it at 32 bits
//	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0x6000) | 0x2000; 16 bits
	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0x6000) | 0x4000; // 32 bits

	// Set the peripheral location to the data register of ADC1
	// Set bits 31:0 of DMA_S0PAR to the address of the data register of ADC1
	// Ref: [1] 9.5.7, p.240
	DMA2->DMA_S0PAR = (uint32_t)&(ADC1->ADC_DR);

	// Set the priority level of the transfer to high (2nd highest)
	// Set bits 17:16 (PL) of DMA_S0CR to '10'
	// Ref: [1] 9.5.5, p.238
	DMA2->DMA_S0CR = (DMA2->DMA_S0CR & ~0x30000) | 0x20000;

	// Set the memory address to automatically increment on each write
	// Set bit 10 (MINC) of DMA_S0CR to high
	// Ref: [1] 9.5.5, p.238
	DMA2->DMA_S0CR |= 1 << 10;

	// Set the number of data items to transfer each time to 5
	// Set bits 15:0 of DMA_S0NDTR to 5
	// Ref: [1] 9.5.6, p.240
	DMA2->DMA_S0NDTR = 5;

	// Set the peripheral (ADC) to control the flow
	// Set bit 5 (PFCTRL) of DMA_S0CR to '1'
	// Ref: [1] 9.5.5, p.239
	DMA2->DMA_S0CR |= 1 << 5;
}

void start_DMA_transfer(uint32_t *addr) {
	if (DMA2->DMA_S0CR & 1) // if enabled
		DMA2->DMA_S0CR &= ~1;

	// Clear the flags relating to stream 0
	// Required before starting transfer ([1] 9.5.5 p.240)
	// Set bits 5:2, 0, of DMA_LIFCR to high (clear flags)
	DMA2->DMA_LIFCR |= 0x3D;

	// Set the number of data items to transfer to 5 (maybe not necessary if it remembers from init?)
	// Set bits 15:0 of DMA_S0NDTR to 5
	// Ref: [1] 9.5.6, p.240
	DMA2->DMA_S0NDTR = 5;

	// Set the memory address of the target to the given argument
	// Set bits 31:0 of DMA_S0M0AR to the address
	// Ref: [1] 9.5.8, p.241
	DMA2->DMA_S0M0AR = (uint32_t)addr;

	// Enable the stream
	// Set bit 1 (EN) of DMA_S0CR to high
	// Ref: [1] 9.5.5, p.240
	DMA2->DMA_S0CR |= 1;
}
