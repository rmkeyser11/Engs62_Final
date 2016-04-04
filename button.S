/*
 * button.S
 *
 * Provide routines to set up and read the button.
 *  Created on: Feb 1, 2016
 *      Author: matthew
 */
.thumb
.syntax unified


.global button_init

button_init:
	PUSH {R0-R3, LR}

	/* Button is on GPIOA Pin 0 - Enable GPIOA */
	/* AHB Clock gating */
	.equ RCC_AHB1ENR, 0x40023830
	MOV R0, #1
	LDR R1, =RCC_AHB1ENR
	LDR R2, [R1]    /* Read */
	ORR R2, R2, R0  /* Modify */
	STR R2, [R1]    /* Write */

	/* PA0 default configuration is general-purpose input */
	/* EXTI0 default mapping is to PA0 */
	/* Configure EXTI0 for rising-edge event detection */
	.equ EXTI_RTSR, 0x40013C08
	MOV R0, #1
	LDR R1, =EXTI_RTSR
	LDR R2, [R1]    /* Read */
	ORR R2, R2, R0  /* Modify */
	STR R2, [R1]    /* Write */

	/* Configure EXTI0 interrupt mask to signal NVIC for EXTI0 events */
	.equ EXTI_IMR,  0x40013C00
	MOV R0, #1
	LDR R1, =EXTI_IMR
	LDR R2, [R1]    /* Read */
	ORR R2, R2, R0  /* Modify */
	STR R2, [R1]    /* Write */

	/* Enable interrupt ID6 (EXTI0) in the NVIC */
	.equ NVIC_ISER0, 0xE000E100
	MOV R0, #1
	MOV R1, R0, LSL #6
	LDR R2, =NVIC_ISER0
	STR R1, [R2]

	/* Return */
	POP {R0-R3, PC}
