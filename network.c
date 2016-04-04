/*
 * network.c
 *
 *  Created on: Feb 29, 2016
 *      Author: matthew
 *
 * I'm operating under the assumption that the UDP protocol,
 * putting the data into packets, will be taken care of by
 * the WiFi board.
 *
 * The send_packet_USART3 function will simply send the message
 * byte by byte over USART3. Not sure if that will work.
 */

#include "USART3.h"
#include "USART2.h"
#include "network.h"
#include "io.h"

void send_packet_USART3(Msg_t *msg) {
	int type = msg->pingmsg.type;
	static int size = 0;
	static int i;
	static char *data;

	switch (type) {
	case TYPE_PING:
		size = sizeof(Ping_t);
		break;
	case TYPE_UPDATE:
		size = sizeof(Update_req_t);
		break;
	default:
		size = 0;
		break;
	}

//	__asm("cpsid i");
	data = (char *)msg;
	for (i=0; i<size; i++) {
		USART3_send(data[i]);
	}
//	__asm("cpsie i");
}

void send_ping(void) {
	Msg_t msg;
	msg.pingmsg.id = 18;
	msg.pingmsg.type = TYPE_PING;

	send_packet_USART3(&msg);
}

void send_update(int val) {
	Msg_t msg;
	msg.reqmsg.id = 18;
	msg.reqmsg.type = TYPE_UPDATE;
	msg.reqmsg.value = val;

	send_packet_USART3(&msg);
}
