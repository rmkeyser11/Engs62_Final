/*
 * accel.c
 *
 *  Created on: Feb 18, 2016
 *      Author: matthew
 *
 * Set up the accelerometer using the SPI interface.
 *
 * References:
 * [1] DM00031020 RM0090 STM32F40x Reference Manual.pdf
 * [2] STM32F4-Discovery Schematics.pdf
 * [3] DM00037051 STM32F407VG Datasheet.pdf
 * [4] DM00040962 LIS3DSH Datasheet.pdf
 */

#include "stm32f4xx.h"
#include "stdint.h"
#include "accel.h"

void accel_cs_high(void);
void accel_config(void);


void accel_init(void)
{
	uint32_t val;
	/**************************************************
	 * Setup IO pins
	 **************************************************/
	// Pins for SPI1:
	// SPI1_SCK = PA5
	// SPI1_MISO = PA6
	// SPI1_MOSI = PA7
	// C2_I2C/SPI = PE3
	// Reference: [2] MEMS sub-diagram, p.6

	// Enable GPIOA and GPIOE clocks
	// Reference: [1] 6.3.12, p.145
	RCC->AHB1ENR |= 0x1; // enable GPIOA clock
	RCC->AHB1ENR |= 0x10; // enable GPIOE clock

	// Set GPIOA pins 5-7 to AF
	// Reference [1] 7.4.1, p.198
	val = (GPIOA->MODER & ~0xFC00); // Mask pins 5-7
	val |= 0xA800; // Set pins 5-7 to alternate function mode
	GPIOA->MODER = val;

	// Set GPIOA pins 5-7 to AF5 (for SPI1 pins)
	// Reference:	[3] Table 8, p.58
	// 				[1] 7.4.9, p.202
	val = GPIOA->AFRL & 0x000FFFFF; // Mask pins 5-7
	val |= 0x55500000; // Set pins 5-7 to AF5
	GPIOA->AFRL = val;

	// Set GPIOE pin 3 to output (CS_I2C/SPI pin)
	val = GPIOE->MODER & ~0xC0; // Mask pin 3
	val |= 0x40; // Set pin 3 to output
	GPIOE->MODER = val;

	// Start with the chip select as logic high by default
	accel_cs_high();

	/************************************************
	 * Setup LI3DSH functionality
	 ************************************************/
	// Enable SPI1 Peripheral clock
	// Set bit 12 of RCC_APB2ENR
	// Reference: [1] 6.3.17
	RCC->APB2ENR |= 0x1000;

	// Set the baud rate to divide clock by 16 => 1 MHz
	// by setting BR[2:0] <= 011
	// Reference:	[1] 27.3.3 p.799
	//				[1] 27.5.1 p.833
	SPI1->SPI_CR1 = (SPI1->SPI_CR1 & ~0x38) | 0x18;

	// Set the clock idle to high
	// Set bit 1 (CPOL) of SPI_CR1 to high
	// Reference:	[1] 27.5.1 p.834
	SPI1->SPI_CR1 |= 0x2;

	// Set the clock phase to be stable on the second clock edge
	// Set bit 0 (CPHA) of SPI_CR1 to high
	// Reference:	[1] 27.5.1 p.835
	SPI1->SPI_CR1 |= 0x1;

	// Set up a 16-bit data frame format
	// Because the output registers of the accelerometer
	// are 2-bytes long
	// Set bit 11 (DFF) in SPI_CR1 to high
	// Reference:	[1] 27.5.1 p.833
	//				[4] 7.23 p.39
	SPI1->SPI_CR1 |= 0x800;


	// Set the format to MSB first
	// Set bit 7 (LSBFIRST) in SPI_CR1 to low
	// Reference:	[1] 27.5.1 p.834
	//				[4] 5.2.1 p.26
	SPI1->SPI_CR1 &= ~0x80;

	// Set the NSS pin to software NSS
	// Set bit 9 (SSM) in SPI_CR1 to high
	// Reference:	[1] 27.5.1 p.834
	SPI1->SPI_CR1 |= 0x200;

	// Set the Slave select to 1
	// Set bit 8 (SSI) of SPI_CR1 to high
	// Reference:	[1] 27.5.1 p.834
	SPI1->SPI_CR1 |= 0x100;

	// Do not enable TI protocol
	// Use default value 0 for bit 4 (FRF) in SPI_CR2

	// Set SPI1 to Master
	// Set bit 2 (MSTR) in SPI_CR1
	// Reference:	[1] 27.5.1 p.834
	SPI1->SPI_CR1 |= 0x4;

	// Enable SPI1
	// Set bit 6 (SPE) in SPI_CR1
	// Reference:	[1] 27.5.1 p.834
	SPI1->SPI_CR1 |= 0x40;

	// Configure the acceleration
	accel_config();
}

/**
 * Configure the accelerometer to enable all 3 channels with frequency of 100 Hz
 */
void accel_config(void)
{
	// Start with forcing reboot
	// Set bit 7 in 0x25 (CTRL_REG6) to high
	accel_write(0x25, 0x80);

	while (accel_read(0x25) & 0x80); // Wait for the reboot to finish

	// Enable all 3 axes at 100 Hz
	// Register address 0x20 is CTRL_REG4
	//
	// Value 0x67 is 0b01100111
	// Bits 7:4 are ODR, 0110 gives 100Hz timing
	// Bit 3 is BDU (block data update), we're leaving it disabled
	// Bits 2:0 are enable for z, y, and x respectively
	// Reference:	[4] 7.5 p.33
	accel_write(0x20, 0x67);

	// Set anti-aliasing filter bandwidth to 50 Hz
	// Set bits 7:6 in CTRL_REG5 to 11
	// CTRL_REG5 is at 0x24
	// value 0xC0 is 0b11000000
	// Leaving other values cleared
	accel_write(0x24, 0xC0);
}

void accel_cs_high(void)
{
	// Set GPIOE pin 3 high
	GPIOE->ODR |= 0x8; // Set bit 3 high
	// wait for the bit to go high
	while (!(GPIOE->IDR & 0x8));
}

void accel_cs_low(void)
{
	// Set GPIOE pin 3 low
	GPIOE->ODR &= ~0x8; // Set bit 3 low
	// Wait for the bit to go low
	while ((GPIOE->IDR & 0x8));
}

/**
 * Only bits 6:0 of accel_reg are used
 *
 */
uint16_t accel_read(uint8_t accel_reg)
{
	uint16_t write_val = 0;
	write_val = accel_reg << 8;
	write_val |= 0x8000; // Set the READ bit to 1

	// Wait for the transmit buffer to be empty
	while (!(SPI1->SPI_SR & 0x2));

	// Set chip select to low to enable the slave
	accel_cs_low();

	// Start shifting data
	SPI1->SPI_DR = write_val;

	// Wait for the BSY flag to unset (done sending)
	while ((SPI1->SPI_SR & 0x80));

	// Return the read value from the data register
	uint16_t read_val = SPI1->SPI_DR;

	// Set chip select back to high
	accel_cs_high();

	// Masked to return the bottom 8 bits
	return read_val & 0xFF;
}

void accel_write(uint8_t accel_reg, uint8_t data)
{
	uint16_t write_val = 0;
	write_val = accel_reg << 8;
	write_val &= ~0x8000; // Set the READ/WRITE bit to 0
	// Set the bottom 8 bits to the data to be written
	write_val = (write_val & 0xFF00) | data;

	// Wait for the transmit buffer to be empty
	while (!(SPI1->SPI_SR & 0x2));

	// Set chip select to low to enable the slave
	accel_cs_low();

	// Start shifting data
	SPI1->SPI_DR = write_val;

	// Wait for the BSY flag to unset (done sending)
	while ((SPI1->SPI_SR & 0x80));

	// Set chip select back to high
	accel_cs_high();
}
