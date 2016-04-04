#pragma once

#include "stdint.h"

/*
 * Some useful definitions for controlling the clock enable registers,
 * configuring GPIO and turning LEDs on and off.
 */
#define ONES (0xFFFFFFFF)
#define ZEROS (0x00000000)


/* GPIO Registers, see the reference manual sections 7.4.X */
volatile typedef struct
{
	uint32_t MODER;                /* Mode register                    - offset 0x00 */
	uint32_t OTYPER;               /* Output type register             - offset 0x04 */
	uint32_t OSPEEDR;              /* Output speed register            - offset 0x08 */
	uint32_t PUPDR;                /* Port pull-up/pull-down register  - offset 0x0C */
	uint32_t IDR;                  /* Input data register              - offset 0x10 */
	uint32_t ODR;                  /* Output data register             - offset 0x14 */
	uint32_t BSRR;                 /* Bit set-reset register           - offset 0x18 */
	uint32_t LCKR;                 /* Port configuration lock register - offset 0x1C */
	uint32_t AFRL;                 /* Alternate function low register  - offset 0x20 */
	uint32_t AFRH;                 /* Alternate function high register - offset 0x24 */
} GPIO_TypeDef;

/* Reset and Clock Control Registers, see User's Manual section 6.3.X */
volatile typedef struct
{
	uint32_t CR;                   /* Clock control register           - offset 0x00 */
	uint32_t PLLCFGR;              /* PLL configuration register       - offset 0x04 */
	uint32_t CFGR;                 /* RCC clock configuration register - offset 0x08 */
	uint32_t CIR;                  /* Clock configuration register     - offset 0x0C */
	uint32_t AHB1RSTR;             /* AHB1 peripheral reset register   - offset 0x10 */
	uint32_t AHB2RSTR;             /* AHB2 peripheral reset register   - offset 0x14 */
	uint32_t AHB3RSTR;             /* AHB3 peripheral reset register   - offset 0x18 */
	uint32_t RES0;                 /* Reserved                         - offset 0x1C */
	uint32_t APB1RSTR;             /* APB1 peripheral reset register   - offset 0x20 */
	uint32_t APB2RSTR;             /* APB2 peripheral reset register   - offset 0x24 */
	uint32_t RES1;                 /* Reserved                         - offset 0x28 */
	uint32_t RES2;                 /* Reserved                         - offset 0x2C */
	uint32_t AHB1ENR;              /* AHB1 peripheral clock enable     - offset 0x30 */
	uint32_t RES3;                 /* Reserved                         - offset 0x34 */
	uint32_t RES4;                 /* Reserved                         - offset 0x38 */
	uint32_t RES5;                 /* Reserved                         - offset 0x3C */
	uint32_t APB1ENR;              /* APB1 peripheral clock enable     - offset 0x40 */
	uint32_t APB2ENR;              /* APB2 peripheral clock enable     - offset 0x44 */
	/* Rest of registers not utilized in lab 3 */
} RCC_TypeDef;

/* See datasheet for memory map / base addresses */

/*
 * GPIOA & GPIOD
 */
#define GPIOA_BASE   (0x40020000)  /* Base address of GPIOA peripheral */
#define GPIOA        ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB_BASE		(0x40020400)
#define GPIOB			((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC_BASE		(0x40020800)
#define GPIOC			((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD_BASE   (0x40020C00)  /* Base address of GPIOD peripheral */
#define GPIOD        ((GPIO_TypeDef*)GPIOD_BASE)
#define GPIOE_BASE	 (0x40021000) /* Base address of GPIOE peripheral */
#define GPIOE		 ((GPIO_TypeDef*)GPIOE_BASE)

#define GPIOx_MODER_PIN9_MASK	(~0xC0000)
#define GPIOx_MODER_PIN12_MASK  (0xfcffffff)
#define GPIOx_MODER_PIN13_MASK  (0xf3ffffff)
#define GPIOx_MODER_PIN14_MASK  (0xcfffffff)
#define GPIOx_MODER_PIN15_MASK  (0x3fffffff)
#define GPIOx_MODER_PIN12_GPOUT (0x01000000)
#define GPIOx_MODER_PIN13_GPOUT (0x04000000)
#define GPIOx_MODER_PIN14_GPOUT (0x10000000)
#define GPIOx_MODER_PIN15_GPOUT (0x40000000)
#define GPIOx_MODER_PIN1_ANALOG (0x0000000C)
#define GPIOx_MODER_PIN3_ANALOG (0x000000C0)
#define GPIOx_MODER_PIN9_AF		(0x80000)
#define GPIOx_AFRH_AFRH9_MASK	(~0xF0)
#define GPIOx_AFRH_AFRH9_AF1	(0x10)

#define RCC_BASE     (0x40023800)
#define RCC          ((RCC_TypeDef*)RCC_BASE)

#define RCC_AHB1ENR_GPIOAEN     (0x01)
#define RCC_AHB1ENR_GPIODEN     (0x08)
#define RCC_AHB1ENR_GPIOEEN		(0x10)
#define RCC_APB2ENR_ADC1EN		(0x100)
#define RCC_APB2ENR_ADC2EN		(0x200)
#define RCC_APB2ENR_ADC3EN		(0x400)
#define RCC_APB2ENR_TIM1EN		(0x1)

/* Systick Timer (STK), see Programming Manual p. 214 */
volatile typedef struct
{
	uint32_t STK_CTRL;				/* SysTick control and status register	- offset 0x00 */
	uint32_t STK_LOAD;				/* SysTick reload value register 		- offset 0x04 */
	uint32_t STK_VAL;				/* SysTick current value register		- offset 0x08 */
	uint32_t STK_CALIB;				/* SysTick calibration value register	- offset 0x0C */
} SYSTICK_TypeDef;

#define STK_BASE	(0xE000E010) /* Base address of systick */
#define STK			((SYSTICK_TypeDef*)STK_BASE)

// Systick control and status register
#define STK_CTRL_COUNTFLAG_MASK (0x10000)
#define STK_CTRL_CLKSOURCE_MASK	(0x04)
#define STK_CTRL_TICKINT_MASK	(0x02)
#define STK_CTRL_ENABLE_MASK	(0x01)

// Systick reload value register
#define STK_LOAD_RELOAD_MASK (0xFFFFFF)

// Systick current value register
#define STK_VAL_CURRENT_MASK (0xFFFFFF)

/* ADC config */
volatile typedef struct
{
	uint32_t ADC_SR;	/* ADC Status Register 							- offset 0x00 */
	uint32_t ADC_CR1;	/* ADC control register 1						- offset 0x04 */
	uint32_t ADC_CR2;	/* ADC control register 2 						- offset 0x08 */
	uint32_t ADC_SMPR1;	/* ADC sample time register 1 					- offset 0x0C */
	uint32_t ADC_SMPR2;	/* ADC sample time register 2 					- offset 0x10 */
	uint32_t ADC_JOFR1; /* ADC injected channel data offset register 1 	- offset 0x14 */
	uint32_t ADC_JOFR2; /* ADC injected channel data offset register 2 	- offset 0x18 */
	uint32_t ADC_JOFR3; /* ADC injected channel data offset register 3 	- offset 0x1C */
	uint32_t ADC_JOFR4; /* ADC injected channel data offset register 4	- offset 0x20 */
	uint32_t ADC_HTR;	/* ADC watchdog higher threshold register		- offset 0x24 */
	uint32_t ADC_LTR;	/* ADC watchdog lower threshold register		- offset 0x28 */
	uint32_t ADC_SQR1;	/* ADC regular sequence register 1				- offset 0x2C */
	uint32_t ADC_SQR2;	/* ADC regular sequence register 2				- offset 0x30 */
	uint32_t ADC_SQR3;	/* ADC regular sequence register 3				- offset 0x34 */
	uint32_t ADC_JSQR;	/* ADC injected sequence register 				- offset 0x38 */
	uint32_t ADC_JDR1;	/* ADC injected data register 1 				- offset 0x3C */
	uint32_t ADC_JDR2;	/* ADC injected data register 2 				- offset 0x40 */
	uint32_t ADC_JDR3;	/* ADC injected data register 3 				- offset 0x44 */
	uint32_t ADC_JDR4;	/* ADC injected data register 4 				- offset 0x48 */
	uint32_t ADC_DR;	/* ADC regular data register 					- offset 0x4C */
} ADC_TypeDef;

#define ADC1_BASE	(0x40012000)
#define ADC1		((ADC_TypeDef*)ADC1_BASE)
#define ADC2_BASE	(0x40012100)
#define ADC2		((ADC_TypeDef*)ADC2_BASE)
#define ADC3_BASE	(0x40012200)
#define ADC3		((ADC_TypeDef*)ADC3_BASE)

volatile typedef struct
{
	uint32_t ADC_CSR;	/* ADC Common status register					- offset 0x00 */
	uint32_t ADC_CCR;	/* ADC common control register					- offset 0x04 */
	uint32_t ADC_CDR;	/* ADC common regular data register for dual and triple modes - offset 0x08 */
} ADC_Common_TypeDef;

#define ADC_COMMON_BASE	(0x40012300)
#define ADC_COMMON	((ADC_Common_TypeDef*)ADC_COMMON_BASE)
#define ADC_SQR1_L_MASK	(0xF00000)
#define ADC_SQR3_SQ1_MASK	(0xF)
#define ADC_SMPR2_SMP1_MASK (0x38)
#define ADC_SMPR2_SMP1_15_CYCLES (0x08)
#define ADC_SR_EOC (0x2)

/* TIM1/8 config */
volatile typedef struct
{
	uint32_t TIMx_CR1; 		/* TIM1&TIM8 control register 1					- offset 0x00 */
	uint32_t TIMx_CR2;		/* TIM1&TIM8 control register 2					- offset 0x04 */
	uint32_t TIMx_SMCR;		/* TIM1&TIM8 slave mode control register		- offset 0x08 */
	uint32_t TIMx_DIER;		/* TIM1&TIM8 DMA/interrupt enable register		- offset 0x0C */
	uint32_t TIMx_SR;		/* TIM1&TIM8 status register					- offset 0x10 */
	uint32_t TIMx_EGR;		/* TIM1&TIM8 event generation register			- offset 0x14 */
	uint32_t TIMx_CCMR1;	/* TIM1&TIM8 capture/compare mode register 1	- offset 0x18 */
	uint32_t TIMx_CCMR2;	/* TIM1&TIM8 capture/compare mode register 2	- offset 0x1C */
	uint32_t TIMx_CCER;		/* TIM1&TIM8 capture/compare enable register	- offset 0x20 */
	uint32_t TIMx_CNT;		/* TIM1&TIM8 counter							- offset 0x24 */
	uint32_t TIMx_PSC;		/* TIM1&TIM8 prescaler							- offset 0x28 */
	uint32_t TIMx_ARR;		/* TIM1&TIM8 auto-reload register				- offset 0x2C */
	uint32_t TIMx_RCR;		/* TIM1&TIM8 repetition counter register		- offset 0x30 */
	uint32_t TIMx_CCR1;		/* TIM1&TIM8 capture/compare register 1			- offset 0x34 */
	uint32_t TIMx_CCR2;		/* TIM1&TIM8 capture/compare register 2			- offset 0x38 */
	uint32_t TIMx_CCR3;		/* TIM1&TIM8 capture/compare register 3			- offset 0x3C */
	uint32_t TIMx_CCR4;		/* TIM1&TIM8 capture/compare register 4			- offset 0x40 */
	uint32_t TIMx_BDTR;		/* TIM1&TIM8 break and dead-time register		- offset 0x44 */
	uint32_t TIMx_DCR;		/* TIM1&TIM8 DMA control register				- offset 0x48 */
	uint32_t TIMx_DMAR;		/* TIM1&TIM8 DMA address for full transfer		- offset 0x4C */
} TIMx_ADV_TypeDef;

#define TIM1_BASE	(0x40010000)
#define TIM1		((TIMx_ADV_TypeDef*)TIM1_BASE)
#define TIM8_BASE	(0x40010400)
#define TIM8		((TIMx_ADV_TypeDef*)TIM8_BASE)

#define TIMx_CCMR1_CC1S_MASK	(~0x3)
#define TIMx_CCMR1_CC1S_OUT		(0x0)
#define TIMx_CCMR1_OC1M_MASK	(~0x70)
#define TIMx_CCMR1_OC1M_PWM1	(0x60)
#define TIMx_PSC_MASK			(~0xFFFF)
#define TIMx_ARR_MASK			(~0xFFFF)
#define TIMx_CCR1_MASK			(~0xFFFF)
#define TIMx_CCER_CC1E_MASK		(~1)
#define TIMx_CCER_CC1E			(1)
#define TIMx_BDTR_MOE			(0x8000)
#define TIMx_CCMR1_OC1PE		(0x8)

/* TIM2-5 config */
volatile typedef struct
{
	uint32_t TIMx_CR1; 		/* offset 0x00 */
	uint32_t TIMx_CR2;		/* offset 0x04 */
	uint32_t TIMx_SMCR;		/* offset 0x08 */
	uint32_t TIMx_DIER;		/* offset 0x0C */
	uint32_t TIMx_SR;		/* offset 0x10 */
	uint32_t TIMx_EGR;		/* offset 0x14 */
	uint32_t TIMx_CCMR1;		/* offset 0x18 */
	uint32_t TIMx_CCMR2;		/* offset 0x1C */
	uint32_t TIMx_CCER;		/* offset 0x20 */
	uint32_t TIMx_CNT;		/* offset 0x24 */
	uint32_t TIMx_PSC;		/* offset 0x28 */
	uint32_t TIMx_ARR;		/* offset 0x2C */
	uint32_t reserved1;		/* offset 0x30 */
	uint32_t TIMx_CCR1;		/* offset 0x34 */
	uint32_t TIMx_CCR2;		/* offset 0x38 */
	uint32_t TIMx_CCR3;		/* offset 0x3C */
	uint32_t TIMx_CCR4;		/* offset 0x40 */
	uint32_t reserved2;		/* offset 0x44 */
	uint32_t TIMx_DCR;		/* offset 0x48 */
	uint32_t TIMx_DMAR;		/* offset 0x4C */
	uint32_t TIM2_or_5_OR;		/* offset 0x50 */

} TIMx_GP_TypeDef;

#define TIM2_BASE		(0x40000000)
#define TIM2			((TIMx_GP_TypeDef*)TIM2_BASE)


// SPI
volatile typedef struct {
	uint32_t SPI_CR1;		/* 0x00 */
	uint32_t SPI_CR2;		/* 0x04 */
	uint32_t SPI_SR;		/* 0x08 */
	uint32_t SPI_DR;		/* 0x0C */
	uint32_t SPI_CRCPR;		/* 0x10 */
	uint32_t SPI_RXCRCR;	/* 0x14 */
	uint32_t SPI_TXCRCR;	/* 0x18 */
	uint32_t SPI_I2SCFGR;	/* 0x1C */
	uint32_t SPI_I2SPR; 	/* 0x20 */
} SPIx_TypeDef;

#define SPI1_BASE		(0x40013000)
#define SPI1			((SPIx_TypeDef*)SPI1_BASE)

volatile typedef struct {
	uint32_t USART_SR;		/* 0x00 */
	uint32_t USART_DR;		/* 0x04 */
	uint32_t USART_BRR;		/* 0x08 */
	uint32_t USART_CR1;		/* 0x0C */
	uint32_t USART_CR2;		/* 0x10 */
	uint32_t USART_CR3;		/* 0x14 */
	uint32_t USART_GTPR;	/* 0x18 */
} USARTx_TypeDef;

#define USART2_BASE		(0x40004400)
#define USART2			((USARTx_TypeDef*)USART2_BASE)
#define USART3_BASE		(0x40004800)
#define USART3			((USARTx_TypeDef *)USART3_BASE)


/* DMA */

volatile typedef struct {
	uint32_t DMA_LISR;		/* 0x00 */
	uint32_t DMA_HISR;		/* 0x04 */
	uint32_t DMA_LIFCR;		/* 0x08 */
	uint32_t DMA_HIFCR;		/* 0x0C */
	uint32_t DMA_S0CR;		/* 0x10 */
	uint32_t DMA_S0NDTR;	/* 0x14 */
	uint32_t DMA_S0PAR;		/* 0x18 */
	uint32_t DMA_S0M0AR;	/* 0x1C */
	uint32_t DMA_S0M1AR;	/* 0x20 */
	uint32_t DMA_S0FCR;		/* 0x24 */

	uint32_t DMA_S1CR;		/* 0x28 */
	uint32_t DMA_S1NDTR;	/* 0x2C */
	uint32_t DMA_S1PAR;		/* 0x30 */
	uint32_t DMA_S1M0AR;	/* 0x34 */
	uint32_t DMA_S1M1AR;	/* 0x38 */
	uint32_t DMA_S1FCR;		/* 0x3C */

	uint32_t DMA_S2CR;		/* 0x40 */
	uint32_t DMA_S2NDTR;	/* 0x44 */
	uint32_t DMA_S2PAR;		/* 0x48 */
	uint32_t DMA_S2M0AR;	/* 0x4C */
	uint32_t DMA_S2M1AR;	/* 0x50 */
	uint32_t DMA_S2FCR;		/* 0x54 */

	uint32_t DMA_S3CR;		/* 0x58 */
	uint32_t DMA_S3NDTR;	/* 0x5C */
	uint32_t DMA_S3PAR;		/* 0x60 */
	uint32_t DMA_S3M0AR;	/* 0x64 */
	uint32_t DMA_S3M1AR;	/* 0x68 */
	uint32_t DMA_S3FCR;		/* 0x6C */

	uint32_t DMA_S4CR;		/* 0x70 */
	uint32_t DMA_S4NDTR;	/* 0x74 */
	uint32_t DMA_S4PAR;		/* 0x78 */
	uint32_t DMA_S4M0AR;	/* 0x7C */
	uint32_t DMA_S4M1AR;	/* 0x80 */
	uint32_t DMA_S4FCR;		/* 0x84 */

	uint32_t DMA_S5CR;		/* 0x88 */
	uint32_t DMA_S5NDTR;	/* 0x8C */
	uint32_t DMA_S5PAR;		/* 0x90 */
	uint32_t DMA_S5M0AR;	/* 0x94 */
	uint32_t DMA_S5M1AR;	/* 0x98 */
	uint32_t DMA_S5FCR;		/* 0x9C */

	uint32_t DMA_S6CR;		/* 0xA0 */
	uint32_t DMA_S6NDTR;	/* 0xA4 */
	uint32_t DMA_S6PAR;		/* 0xA8 */
	uint32_t DMA_S6M0AR;	/* 0xAC */
	uint32_t DMA_S6M1AR;	/* 0xB0 */
	uint32_t DMA_S6FCR;		/* 0xB4 */

	uint32_t DMA_S7CR;		/* 0xB8 */
	uint32_t DMA_S7NDTR;	/* 0xBC */
	uint32_t DMA_S7PAR;		/* 0xC0 */
	uint32_t DMA_S7M0AR;	/* 0xC4 */
	uint32_t DMA_S7M1AR;	/* 0xC8 */
	uint32_t DMA_S7FCR;		/* 0xCC */
} DMA_TypeDef;

#define DMA2_BASE	(0x40026400)
#define DMA2		((DMA_TypeDef*)DMA2_BASE)
