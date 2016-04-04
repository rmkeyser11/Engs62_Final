/*
 * mutex.S
 *
 * Implement simple mutexes based on the ldrex and strex instructions
 *
 *  Created on: Feb 1, 2016
 *      Author: matthew
 */

.syntax unified
.thumb
 .global lock_mutex_blocking
 .global lock_mutex
 .global unlock_mutex

 /* Define possible states */
 .equ	locked, 1
 .equ	unlocked, 0

/* Implement a simple mutex, locking the address
 * passed in in r0.
 */
 lock_mutex_blocking:
 	push	{r1, r2, lr}
 	ldr		r1, =locked
 	start_block:
 	ldrex	r2, [r0]
 	cmp		r2, r1   		// Test whether the mutx is locked
 	beq		wait			// If locked, wait for it to unlock

 	strex	r2, r1, [r0] 	// Not locked, attempt to lock it
 	cmp		r2, #1			// check if store-exclusive failed
 	beq		start_block		// retry
 	// Lock acquired
 	dmb						// Required before accessing protected resource
 	pop		{r1, r2, pc}		// return

 	wait:
// 	WFE // wait for a signal
 	B		start_block


/* Set r0 to 1 if failed */
lock_mutex:
	push	{r1, r2, lr}
 	ldr		r1, =locked
 	ldrex	r2, [r0]
 	cmp		r2, r1   		// Test whether the mutx is locked
 	beq		fail			// If locked, return fail

 	strex	r2, r1, [r0] 	// Not locked, attempt to lock it
 	cmp		r2, #1			// check if store-exclusive failed
 	beq		fail			// return fail
 	// Lock acquired
 	dmb						// Required before accessing protected resource
 	pop		{r1, r2, pc}		// return

 	fail:
	mov		r0, #1
	pop		{r1, r2, pc}

unlock_mutex:
	push	{r1, lr}
	ldr		r1, =unlocked
	dmb		// required before releasing protected resource
	str		r1, [r0]
//	SEV // Signal that it's unlocked
	pop		{r1, pc}
