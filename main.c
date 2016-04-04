/*
 * main.c
 */

#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */
#include "systick.h"	/* Systick initializer */
#include "USART2.h"		/* USART2 */
#include "USART3.h"		/* USART3 */
#include "button.h"		/* Initialize and use button */
#include "io.h"			/* print, printHex, printUnsignedDecimal, etc */
#include "ADC.h"		/* Initialize and read ADCs */
#include "servo.h"		/* Servo initialization and setting */
#include "network.h"	/* Network routines and prototypes */
#include "update.h"		/* Functions to update servos from server or server from ADCs */
#include "DMA.h"        /* Direct Memory Access */

#define DEBUG 0

volatile int systemTicks = 0;

// Flags set from systick
volatile int update_f = 0;
volatile int update_leds_f = 1;
volatile int update_servos_from_server_f = 0;
volatile int send_update_f = 0;

// Flags about wifi sending
volatile int waiting_to_recv_packet = 0;
volatile int received_new_packet = 0;

// Globals to hold received data
volatile int recv_offset = 0;
volatile Msg_t recv_msg;

// Test flag
int test_flag = 0;

typedef enum {
	CONFIGURE_S = 0,
	CLIENT_S,
	COMMAND_S,
} state_t;

state_t mode_state = CONFIGURE_S;

void buttonResponse(void);
void __attribute__ ((interrupt)) systick_handler(void);
void __attribute__ ((interrupt)) USART2_handler(void);
void __attribute__ ((interrupt)) USART3_handler(void);
void __attribute__ ((interrupt)) EXTI0_handler(void);

int main()
{
	int which_to_update = 6; // start greater than 5 so we get data
	uint32_t data[5]; // Array to hold ADC data

	// Initialize all the things
	LED_init();
	systick_init(400000); // Timer goes off 10 times per second
	USART2_init();
	USART3_init();
	button_init();
	ADC_init();
	servo_init();
	DMA_init();

	/* Enable interrupts */
	__asm ("  cpsie i \n" );


	/* Main program loop */
	while(1)
	{
		// State specific behavior (every time)
		switch (mode_state) {
		case CONFIGURE_S:
			// Don't do anything
			break;
		case COMMAND_S:
		{
			/* Send in two cases:
			 * a) we're not waiting for a packet
			 * b) the update flag is set (every second, because sometimes packets get dropped
			 * 	and we don't want to wait forever)
			 *
			 * When we send a byte, we'll send whichever is next in the sequence (the current one
			 * is stored in the local variable (in main) which_to_update). When it goes over 5,
			 * we read in all the data for the next round of packets.
			 */
			if (!waiting_to_recv_packet || send_update_f) {
//			if (send_update_f) {
				if (which_to_update > 5) { // finished updating
					ADC_read(data);
					which_to_update = 1;
				}

				// We will be waiting for a packet back, so set this ahead of time
				waiting_to_recv_packet=1;
				// A new packet will be inbound, so reset the offset to 0 (in case it got messed up before)
				recv_offset = 0;
				update_server(which_to_update, data);
				which_to_update++;

				send_update_f = 0;
			}

			break;
		}
		case CLIENT_S:
		{
			/*
			 * Update the servos on the high tick of this flag. That is set in systick, and happens 10 times
			 * per second
			 */
			if (update_servos_from_server_f) {
				// We're about to receive a response packet, so we reset the recv_offset to 0
				recv_offset = 0;
				waiting_to_recv_packet = 1;
				update_servos();
				update_servos_from_server_f = 0;
			}

			break;
		}
		default:
			break;
		}

		// Every time, regardless of state:
		// If we received a packet, print it
		if (received_new_packet) {
//			switch (recv_msg.pingmsg.type) {
//			case TYPE_PING:
//				print_string("[PING,id=");
//				printUnsignedDecimal(recv_msg.pingmsg.id);
//				print_string("]\n");
//				break;
//			case TYPE_UPDATE:
//				print_string("[UPDATE,id=");
//				printUnsignedDecimal(recv_msg.respmsg.id);
//				print_string(",average=");
//				printUnsignedDecimal(recv_msg.respmsg.average);
//				print_string(",{");
//				for (int i=0; i<CLASS_SIZE_MAX; i++) {
//					print_string(" ");
//					printUnsignedDecimal(recv_msg.respmsg.values[i]);
//				}
//				print_string("}]\n\r");
//				break;
//			default:
//				break;
//			}
			// Reset the flag
			received_new_packet = 0;

			// If we're in client mode, set the servo values to those from the server
			if (mode_state == CLIENT_S) {
				if (recv_msg.respmsg.type == TYPE_UPDATE) {
					for (int i=1; i<=5; i++)
						servo_update(i, recv_msg.respmsg.values[i]);
				}
			}
		}

		// After switching states, update leds
		if (update_leds_f) {
			switch (mode_state) {
			case CONFIGURE_S:
				LED_update(LED_BLUE_ON|LED_ORANGE_OFF);
				// Configuration:
				// $$$ (escape sequence)
				// set ip dhcp 1 (get IP address with dhcp)
				// set ip host 172.16.1.10 (set remote IP)
				// set ip remote 8004
				// set wlan join 1 (try to connect to stored access point)
				// set wlan auth 4 (set to WPA2-PSK)
				// set wlan phrase ENGS62wifi
				// set wlan ssid ENGS62
				// save
				// reboot
				break;
			case CLIENT_S:
				LED_update(LED_BLUE_OFF|LED_ORANGE_ON);
				break;
			case COMMAND_S:
				LED_update(LED_BLUE_ON|LED_ORANGE_ON);
				waiting_to_recv_packet = 0;
				break;
			}
			update_leds_f = 0;
		}

		// If in debug mode, print ADC data to the console
		if (DEBUG && test_flag)
		{
			test_flag = 0;
			uint32_t data[5];
			// Initialize the data array to 0 for clarity
			for (int i=0; i<5; i++) {
				data[i] = 0;
			}

			ADC_read(data);
			for (int i=0; i<5; i++) {
				printUnsignedDecimal((uint16_t)data[i]);
				print_string("\n");
				print_string("\r");
			}
			print_string("-----------\n");
		}
	}
	/* We'll never reach this line */
	return 0;
}


/*
 * The systick Interrupt Service Routine
 */
void __attribute__ ((interrupt)) systick_handler(void)
{
	static int waiting_prev = 0;

	/*
	 * Every .5 seconds, toggle the green LED
	 */
	if (systemTicks % 20 == 0)
	{
		LED_toggle(LED_GREEN);

		// Test ADC
		test_flag = 1;
	}

	/*
	 * If we're in client mode, we want to account for dropped packets
	 * so we won't wait forever. If we're waiting for two consecutive systicks,
	 * (.2 seconds), reset the waiting_to_recv_packet flag to low
	 */
	if (mode_state == CLIENT_S) {
		update_servos_from_server_f = 1;
		if (waiting_to_recv_packet == waiting_prev) {
			waiting_to_recv_packet = 0;
		}
		waiting_prev = waiting_to_recv_packet;
	}

	/*
	 * If we're in command mode, send an update flag
	 * (to "reset" the sending if we happened to drop a packet)
	 * every second
	 */
	if (mode_state == COMMAND_S) {
		send_update_f = 1;
	}

	// global counter of how many systicks we've had
	systemTicks++;
}


void __attribute__ ((interrupt)) USART2_handler(void) {
	char c = USART2_recv(); // Always immediately read the input


	switch (mode_state) {
	case CONFIGURE_S: // In configure mode, pass it along to the WiFly
		USART3_send(c);
		break;
	default: // Other modes just echo back input
		USART2_send(c);
		break;
	}
}

void __attribute__ ((interrupt)) USART3_handler(void) {
	char c = USART3_recv(); // Always immediately read the input

	switch (mode_state) {
	case CONFIGURE_S: // In configure, pass to console
	{
		USART2_send(c);
		break;
	}

	case CLIENT_S:
	case COMMAND_S: // Intentional fall-through - these do the same thing
	{
		/* Read in consecutive bytes of the message */
		*(((char*)&recv_msg)+recv_offset) = c;
		recv_offset++;

		/* When we get the full message, set a flag for it
		 * to print in the main loop, and clear the recv_offset
		 * and waiting_to_recv_packet flag
		 */
		if (recv_offset == sizeof(Update_resp_t)) {
			received_new_packet = 1;
			recv_offset = 0;
			waiting_to_recv_packet = 0;
		}
		break;
	}
	default:
		break;
	}
}

void __attribute__ ((interrupt)) EXTI0_handler(void) {
	int n = 0;
	/* Button debounce */
	for (int i=0; i<1000; i++) {
		if (GPIOA->IDR & 1)
			n++;
	}
	if (n > 990)
		buttonResponse();

	/* Reset the pending bit */
	__asm(	".equ 	EXTI_PR, 0x40013C14\n"
			"ldr		r1, =EXTI_PR\n"
			"mov		r0, #1\n"
			"mov		r2, r0, LSL #0\n"
			"str		r2, [r1]\n" );
}

/* buttonResponse
 * Move to the next state
 */
void buttonResponse(void) {
	switch (mode_state) {
	case CONFIGURE_S:
		mode_state = CLIENT_S;
		break;
	case CLIENT_S:
		mode_state = COMMAND_S;
		break;
	case COMMAND_S:
		mode_state = CONFIGURE_S;
		break;
	}
	update_leds_f = 1;
}

