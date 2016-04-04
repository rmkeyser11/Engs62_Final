/*
 * servo.c
 *
 * Enable servos on ports PE9, PE11, PE13, PE14, and PC6

 * PC6 = TIM8_CH1
 * * via channels 1-4 of TIM1 (GPIOE) and channel 1 of TIM8 (GPIOC)
 *
 * PE9 = TIM1_CH1
 * PE11 = TIM1_CH2
 * PE13 = TIM1_CH3
 * PE14 = TIM1_CH4
 * Documentation references:
 * [1]: STM32F40x Reference Manual
 * [2]: STM32F407VG Datasheet
 * [3]:
 *
 *  Created on: Feb 16, 2016
 *      Author: matthew
 */

#include "stm32f4xx.h"
#include "stdint.h"

void servo_init(void)
{
	uint32_t val;

	/*******************************************
	 * Enable clocks
	 *******************************************/
	// Enable clock for GPIOE - [1] 6.3.12 p.145
	// Set bit 4 of RCC_AHB1ENR (GPIOEEN) to high
	// Ref. [1] 6.3.12 p.145
	RCC->AHB1ENR |= 1 << 4;

	// Enable clock for GPIOC
	// Set bit 2 of RCC_AHB1ENR (GPIOCEN) to high
	// Ref. [1] 6.3.12 p.145
	RCC->AHB1ENR |= 4;

	// Enable clock for Advanced-control timer TIM1
	// Set bit 0 of RCC_APB2ENR (TIM1EN) to high
	// Ref.	[1] 6.3.17 p.154
	RCC->APB2ENR |= 1;

	// Enable clock for advanced-control timer TIM8
	// Set bit 1 of RCC_APB2ENR (TIM8EN) to high
	// Ref. [1] 6.3.17 p.154
	RCC->APB2ENR |= 2;

	/*******************************************
	 * Configure GPIO pins
	 *******************************************/

	// Configure Port E pins 9, 11, 13, and 14 to AF mode
	// Set GPIOx_MODER bits 29:28 (MODER14), 27:26 (MODER13),
	// 23:22 (MODER11), and 19:18 (MODER9) to '10', alternate
	// function mode
	//
	// Ref. [1] 7.4.1 p.198
	GPIOE->MODER = (GPIOE->MODER & ~0x3CCC0000) | 0x28880000;

	//Configure port C pin 6 to AF mode
	// Set GPIOx_MODER bits 13:12 (MODER6) to '10'
	//
	// Ref. [1] 7.4.1 p.198
	GPIOC->MODER = (GPIOC->MODER & ~0x3000) | 0x2000;


	// Set ports 9, 11, 13, and 14 on GPIOE to AF1 for
	// TIM1 channels 1-4
	//
	// Set GPIOE_AFRH bits 27:24 (AFRH14), 23:20 (AFRH13),
	// 15:12 (AFRH11), and 7:4 (AFRH9) to '0001' for AF1
	//
	// Ref. [2] Table 8 p.60
	// 	[1] 7.4.10 p.203
	GPIOE->AFRH = (GPIOE->AFRH & ~0xFF0F0F0) | 0x1101010;

	// Set port 6 on GPIOC to AF3 (TIM8/9/10/11) for
	// TIM8_CH1
	//
	// Set GPIOC_AFRL bits 27:24 to '0011' for AF3
	// Ref.	[2] Table 8 p.59
	//		[1] 7.4.9 p.202
	GPIOC->AFRL = (GPIOC->AFRL & ~0xF000000) | 0x3000000;

	/*******************************************
	 * Configure TIM1
	 *******************************************/
	// Set Prescaler to 15 (downclock to 1 MHz) - [1] 14.4.11 p.409
	TIM1->TIMx_PSC = (TIM1->TIMx_PSC & ~0xFFFF) | 15;

	// Set auto-reload register to 20,000 (20ms clock period) - [1] 14.4.12 p.409
	TIM1->TIMx_ARR = (~0xFFFF & TIM1->TIMx_ARR) | 20000;


	/*******************************************
	 * Configure TIM8
	 *******************************************/
	// Set Prescaler to 15 (downclock to 1 MHz)
	// Set bits 15:0 of TIMx_PSC to 15
	// Ref. [1] 14.4.11 p.409
	TIM8->TIMx_PSC = (TIM8->TIMx_PSC & ~0xFFFF) | 15;

	// Set a 20ms clock period
	// Set bits 15:0 of TIMx_ARR (auto-reload register) to 20,000
	// Ref: [1] 15.4.12 p.470
	TIM8->TIMx_ARR = (~0xFFFF & TIM8->TIMx_ARR) | 20000;

	/*******************************************
	 * Configure Compare/capture for TIM1_CH1
	 *******************************************/
	// Set Compare/Capture 1 Selection (CC1S) Bits 1:0
	// to 00: output
	// Ref: [1] 14.4.7 p.403
	TIM1->TIMx_CCMR1 = (TIM1->TIMx_CCMR1 & ~0x3) | 0;

	// In Capture/Compare Mode Register 1 (CCMR1)
	// Set Output Compare 1 mode (OC1M) Bits 6:4
	// to '110': PWM Mode 1
	// Ref: [1] 14.4.7 p.402
	TIM1->TIMx_CCMR1 = (TIM1->TIMx_CCMR1 & ~0x70) | 0x60;

	// Enable Output compare 1 preload enable
	// This enables the register to preload a value at update events
	// Discussed in [1] - 14.3.10
	//
	// Set bit 3 (OC1PE) of CCMR1 to high
	// Ref: [1] - 14.4.7 p.402
	TIM1->TIMx_CCMR1 |= 0x8;

	// Configure a default duty cycle of 7.5% (t_high = 1.5ms)
	// To do this, set CCR1 to 1,500 - [1] 14.4.14 p.410
	val = (TIM1->TIMx_CCR1 & TIMx_CCR1_MASK) | 1500;
	TIM1->TIMx_CCR1 = val;

	// Enable Capture/compare 1 to output
	// Set CC1E on to enable output - [1] 14.4.9 p.407
	val = (TIM1->TIMx_CCER & TIMx_CCER_CC1E_MASK) | TIMx_CCER_CC1E;
	TIM1->TIMx_CCER = val;

	/*******************************************
	 * Configure Compare/capture for TIM1_CH2
	 *******************************************/
	// Set Compare/Capture 2 to output
	// Set bits 9:8 of TIMx_CCMR1 to '00'
	// Ref: [1] 14.4.7 p.403
	TIM1->TIMx_CCMR1 = (TIM1->TIMx_CCMR1 & ~0x300) | 0;

	// Set output compare mode to PWM1
	//
	// In Capture/Compare Mode Register 1 (CCMR1)
	// Set Output Compare 2 mode (OC2M) Bits 14:12
	// to '110': PWM Mode 1
	//
	// Ref: [1] 14.4.7 p.402
	TIM1->TIMx_CCMR1 = (TIM1->TIMx_CCMR1 & ~0x7000) | 0x6000;

	// Enable Output compare 2 preload enable
	// This enables the register to preload a value at update events
	// Discussed in [1] - 14.3.10
	//
	// Set bit 11 of Capture/Control Mode Register 1 (CCMR1) to high
	// Ref: [1] 14.4.7 p.402
	TIM1->TIMx_CCMR1 = TIM1->TIMx_CCMR1 | 1 << 11;

	// Configure a default duty cycle of 7.5% (t_high = 1.5ms)
	// Set CCR2 bits 15:0 to 1,500
	// Ref: [1] 14.4.15 p.411
	TIM1->TIMx_CCR2 = (TIM1->TIMx_CCR2 & ~0xFFFF) | 1500;

	// Enable Capture/compare 2 to output
	// Set bit 4 (CC2E) in CCER to high
	// Ref: [1] 14.4.9 p.407
	TIM1->TIMx_CCER |= 0x10;

	/*******************************************
	 * Configure Compare/capture for TIM1_CH3
	 *******************************************/
	// Set compare/capture 3 to output
	// Set bits 1:0 in TIMx_CCMR2 to '00'
	// Ref: [1] 14.4.8 p.404
	TIM1->TIMx_CCMR2 = (TIM1->TIMx_CCMR2 & ~0x3) | 0;

	// In Capture/Compare Mode Register 2 (CCMR2)
	// Set Output Compare 3 mode (OC3M) Bits 6:4
	// to '110': PWM Mode 1
	// Ref: [1] 14.4.8 p.404
	TIM1->TIMx_CCMR2 = (TIM1->TIMx_CCMR2 & ~0x70) | 0x60;

	// In Capture/Control Mode Register 2 (CCMR2)
	// Enable Output compare 3 preload enable (OCM3PE)
	// This enables the register to preload a value at update events
	// Discussed in [1] - 14.3.10
	//
	// Set bit 3 of TIMx_CCMR2 to high
	// Ref: [1] - 14.4.8 p.404
	TIM1->TIMx_CCMR2 |= 0x8;

	// Configure a default duty cycle of 7.5% (t_high = 1.5ms)
	// Set CCR3 bits 15:0 to 1,500
	// Ref: [1] 14.4.16 p.411
	TIM1->TIMx_CCR3 = (TIM1->TIMx_CCR3 & ~0xFFFF) | 1500;

	// Enable Capture/compare 3 to output
	// set bit 8 (CC3E) in CCER to high
	// Ref: [1] 14.4.9 p.407
	TIM1->TIMx_CCER |= 0x100;

	/*******************************************
	 * Configure Compare/capture for TIM1_CH4
	 *******************************************/
	// Set Compare/Capture 4 to output
	// Set bits 9:8 of TIMx_CCMR2 to '00'
	// Ref: [1] 14.4.8 p.404
	TIM1->TIMx_CCMR2 = (TIM1->TIMx_CCMR2 & ~0x300) | 0;

	// Set output compare mode to PWM1
	//
	// In Capture/Compare Mode Register 2 (CCMR2)
	// Set Output Compare 4 mode (OC4M) Bits 14:12
	// to '110': PWM Mode 1
	//
	// Ref: [1] 14.4.8 p.404
	TIM1->TIMx_CCMR2 = (TIM1->TIMx_CCMR2 & ~0x7000) | 0x6000;

	// Enable Output compare 4 preload enable
	// This enables the register to preload a value at update events
	// Discussed in [1] - 14.3.10
	//
	// Set bit 11 (OC4PE) of Capture/Control Mode Register 2 (CCMR2) to high
	// Ref: [1] 14.4.8 p.404
	TIM1->TIMx_CCMR2 = TIM1->TIMx_CCMR2 | 1 << 11;

	// Configure a default duty cycle of 7.5% (t_high = 1.5ms)
	// Set CCR4 bits 15:0 to 1,500
	// Ref: [1] 14.4.17 p.412
	TIM1->TIMx_CCR4 = (TIM1->TIMx_CCR4 & ~0xFFFF) | 1500;

	// Enable Capture/compare 4 to output
	// Set bit 12 (CC4E) in CCER to high
	// Ref: [1] 14.4.9 p.407
	TIM1->TIMx_CCER |= 0x1000;


	/*******************************************
	 * Configure Compare/capture for TIM8_CH1
	 * Copy configuration from TIM1_CH1
	 *******************************************/
	// Set Compare/Capture 1 Selection (CC1S) Bits 1:0
	// to 00: output
	// Ref: [1] 14.4.7 p.403
	TIM8->TIMx_CCMR1 = (TIM8->TIMx_CCMR1 & ~0x3) | 0;

	// In Capture/Compare Mode Register 1 (CCMR1)
	// Set Output Compare 1 mode (OC1M) Bits 6:4
	// to '110': PWM Mode 1
	// Ref: [1] 14.4.7 p.402
	TIM8->TIMx_CCMR1 = (TIM8->TIMx_CCMR1 & ~0x70) | 0x60;

	// Enable Output compare 1 preload enable
	// This enables the register to preload a value at update events
	// Discussed in [1] - 14.3.10
	//
	// Set bit 3 (OC1PE) of CCMR1 to high
	// Ref: [1] - 14.4.7 p.402
	TIM8->TIMx_CCMR1 |= 0x8;

	// Configure a default duty cycle of 7.5% (t_high = 1.5ms)
	// To do this, set CCR1 to 1,500 - [1] 14.4.14 p.410
	val = (TIM8->TIMx_CCR1 & TIMx_CCR1_MASK) | 1500;
	TIM8->TIMx_CCR1 = val;

	// Enable Capture/compare 1 to output
	// Set CC1E on to enable output - [1] 14.4.9 p.407
	val = (TIM8->TIMx_CCER & TIMx_CCER_CC1E_MASK) | TIMx_CCER_CC1E;
	TIM8->TIMx_CCER = val;

	/*******************************************
	 * Enable common settings for TIM1
	 *******************************************/
	// Enable auto-reload preload enable
	// As discussed in [1] - 14.3.10
	//
	// Set bit 7 (ARPE) in TIMx_CR1 to high
	// Ref: [1] 14.4.1 p.390
	TIM1->TIMx_CR1 |= 0x80;

	// Set the MOE bit in the BDTR register to enable all outputs
	// [1] 14.4.18 p.412
	val = TIM1->TIMx_BDTR | TIMx_BDTR_MOE;
	TIM1->TIMx_BDTR = val;

	// Update the preload registers by setting UG bit in TIMx_EGR
	// Discussed in [1] 14.3.10 for using PWM mode
	// Set UG bit (bit 0) in EGR
	val = TIM1->TIMx_EGR | 1;
	TIM1->TIMx_EGR = val;

	// Enable the timer by setting CEN (bit 0 of CR1);
	// [1] 14.4.1 p.390
	val = TIM1->TIMx_CR1 | 1;
	TIM1->TIMx_CR1 = val;

	/*******************************************
	 * Enable common settings for TIM8
	 *******************************************/
	// Enable auto-reload preload enable
	// As discussed in [1] - 14.3.10
	//
	// Set bit 7 (ARPE) in TIMx_CR1 to high
	// Ref: [1] 14.4.1 p.390
	TIM8->TIMx_CR1 |= 0x80;

	// Set the MOE bit in the BDTR register to enable all outputs
	// [1] 14.4.18 p.412
	val = TIM8->TIMx_BDTR | TIMx_BDTR_MOE;
	TIM8->TIMx_BDTR = val;

	// Update the preload registers by setting UG bit in TIMx_EGR
	// Discussed in [1] 14.3.10 for using PWM mode
	// Set UG bit (bit 0) in EGR
	val = TIM8->TIMx_EGR | 1;
	TIM8->TIMx_EGR = val;

	// Enable the timer by setting CEN (bit 0 of CR1);
	// [1] 14.4.1 p.390
	val = TIM8->TIMx_CR1 | 1;
	TIM8->TIMx_CR1 = val;

}

/**
 * t_high is the desired t_high period in microseconds
 * id is the servo to update.
 * 0 = PE9
 * 1 = PE11
 * 2 = PE13
 * 3 = PE14
 * 4 = PC6
 */
void servo_update(int id, uint16_t t_high)
{
	if (t_high > 2000 || t_high < 1000) // return if invalid time
		return;

	switch (id) {
	case 1:
		// Set bits 15:0 of CCR1 to t_high
		TIM1->TIMx_CCR1 = 0xFFFF & t_high;
		break;
	case 2:
		// Set bits 15:0 of TIM1_CCR2 to t_high
		TIM1->TIMx_CCR2 = 0xFFFF & t_high;
		break;
	case 3:
		// Set bits 15:0 of TIM1_CCR3 to t_high
		TIM1->TIMx_CCR3 = 0xFFFF & t_high;
		break;
	case 4:
		// Set bits 15:0 of TIM1_CCR4 to t_high
		TIM1->TIMx_CCR4 = 0xFFFF & t_high;
		break;
	case 5:
		// set bits 15:0 of TIM2_CCR4 to t_high
		TIM8->TIMx_CCR1 = 0xFFFF & t_high;
		break;
	default:
		break;
	}
}
