/*
 * io.c
 *
 *  Created on: Feb 9, 2016
 *      Author: matthew
 */
#include "USART2.h"
#include "stdint.h"
#include "network.h"
#define LSNibble 0xF

//hex print for 32 bit
void print(uint32_t val) {
	int started = 0;
	int part;
	USART2_send('0');
	USART2_send('x');
	for (int nibble = 7; nibble >= 0; nibble--) {
		part = (val >> nibble*4) & LSNibble;
		if (part && !started)
			started = 1;

		switch (part) {
		case 0x0:
//			if (started)
			USART2_send('0');
			break;
		case 0x1:
			USART2_send('1');
			break;
		case 0x2:
			USART2_send('2');
			break;
		case 0x3:
			USART2_send('3');
			break;
		case 0x4:
			USART2_send('4');
			break;
		case 0x5:
			USART2_send('5');
			break;
		case 0x6:
			USART2_send('6');
			break;
		case 0x7:
			USART2_send('7');
			break;
		case 0x8:
			USART2_send('8');
			break;
		case 0x9:
			USART2_send('9');
			break;
		case 0xA:
			USART2_send('A');
			break;
		case 0xB:
			USART2_send('B');
			break;
		case 0xC:
			USART2_send('C');
			break;
		case 0xD:
			USART2_send('D');
			break;
		case 0xE:
			USART2_send('E');
			break;
		case 0xF:
			USART2_send('F');
			break;
		}
	}
}

//hex print for 16 bit
void printHex(uint16_t val) {
	int started = 0;
	int part;
	USART2_send('0');
	USART2_send('x');
	for (int nibble = 3; nibble >= 0; nibble--) {
		part = (val >> nibble*4) & LSNibble;
		if (part && !started)
			started = 1;

		switch (part) {
		case 0x0:
//			if (started)
			USART2_send('0');
			break;
		case 0x1:
			USART2_send('1');
			break;
		case 0x2:
			USART2_send('2');
			break;
		case 0x3:
			USART2_send('3');
			break;
		case 0x4:
			USART2_send('4');
			break;
		case 0x5:
			USART2_send('5');
			break;
		case 0x6:
			USART2_send('6');
			break;
		case 0x7:
			USART2_send('7');
			break;
		case 0x8:
			USART2_send('8');
			break;
		case 0x9:
			USART2_send('9');
			break;
		case 0xA:
			USART2_send('A');
			break;
		case 0xB:
			USART2_send('B');
			break;
		case 0xC:
			USART2_send('C');
			break;
		case 0xD:
			USART2_send('D');
			break;
		case 0xE:
			USART2_send('E');
			break;
		case 0xF:
			USART2_send('F');
			break;
		}
	}
}

void printUnsignedDecimal(int16_t val) {
	int digits[5];
	for (int i=0; i<5; i++) {
		digits[i] = val % 10;
		val = val / 10;
	}

	int started = 0;

	for (int i=4; i>=0; i--) {
		if (!started && digits[i]) started = 1;

		switch (digits[i]) {
		case 0:
			if (started || i==0) {
				USART2_send('0');
			}
			break;
		case 1:
			USART2_send('1');
			break;
		case 2:
			USART2_send('2');
			break;
		case 3:
			USART2_send('3');
			break;
		case 4:
			USART2_send('4');
			break;
		case 5:
			USART2_send('5');
			break;
		case 6:
			USART2_send('6');
			break;
		case 7:
			USART2_send('7');
			break;
		case 8:
			USART2_send('8');
			break;
		case 9:
			USART2_send('9');
			break;
		}
	}
}

void printSignedDecimal(int16_t val) {
	uint16_t uval;

	if (val & 0x8000) {// If the sign bit is set
		USART2_send('-');
		uval = ~val + 1;
	} else {
		USART2_send(' ');
		uval = val;
	}

	int digits[5];
	for (int i=0; i<5; i++) {
		digits[i] = uval % 10;
		uval = uval / 10;
	}

	for (int i=4; i>=0; i--) {

		switch (digits[i]) {
		case 0:
			USART2_send('0');
			break;
		case 1:
			USART2_send('1');
			break;
		case 2:
			USART2_send('2');
			break;
		case 3:
			USART2_send('3');
			break;
		case 4:
			USART2_send('4');
			break;
		case 5:
			USART2_send('5');
			break;
		case 6:
			USART2_send('6');
			break;
		case 7:
			USART2_send('7');
			break;
		case 8:
			USART2_send('8');
			break;
		case 9:
			USART2_send('9');
			break;
		}
	}
}

void println(uint32_t val) {
	print(val);
	USART2_send('\n');
}

void print_string(char *str) {
	while (*str != '\0') {
		USART2_send(*str);
		str++;
	}
}

void print_msg(volatile Msg_t *msg) {
	switch (msg->pingmsg.type) {
	case TYPE_PING:
		print_string("[PING,id=");
		printUnsignedDecimal(msg->pingmsg.id);
		print_string("]\n");
		break;
	case TYPE_UPDATE:
		print_string("[UPDATE,id=");
		printUnsignedDecimal(msg->respmsg.id);
		print_string(",average=");
		printUnsignedDecimal(msg->respmsg.average);
		print_string(",{");
		for (int i=0; i<CLASS_SIZE_MAX; i++) {
			print_string(" ");
			printUnsignedDecimal(msg->respmsg.values[i]);
		}
		print_string("}]\n\r");
		break;
	default:
		break;
	}
}
