/*
 * update.c
 *
 *  Created on: Mar 6, 2016
 *      Author: matthew
 */
#include "network.h"
#include "servo.h"
#include "ADC.h"

/**
 * Read each value from the ADC, and update the server accordingly
 */
void update_server_from_adc(void) {
	uint32_t data[5];
	int t_high;
	float f_t_high;
	Msg_t msg;

	ADC_read(data);

	// Send each value as a separate update message

	for (int i=1; i<6; i++) {
		f_t_high = data[i-1] / (float)(0xFFF); // percentage
		t_high = (f_t_high * 1000) + 1000;
		msg.reqmsg.id = i;
		msg.reqmsg.type = TYPE_UPDATE;
		msg.reqmsg.value = t_high;
		send_packet_USART3(&msg);
	}
}

void update_server(int id, uint32_t data[5]) {
	int t_high;
	float f_t_high;
	Msg_t msg;

	// Send an update message for the given ID
	f_t_high = data[id-1] / (float)(0xFFF); // percentage
	t_high = (f_t_high * 1000) + 1000;
	msg.reqmsg.id = id;
	msg.reqmsg.type = TYPE_UPDATE;
	msg.reqmsg.value = t_high;
	send_packet_USART3(&msg);
}

void update_servos(void) {
	Msg_t msg;
	msg.reqmsg.id = JUNK_ID;
	msg.reqmsg.type = TYPE_UPDATE;
	msg.reqmsg.value = 8888; // Junk value, we just want to get the response
	send_packet_USART3(&msg);
	// When the server responds, the USART3 handler will call set_servos_from_network with the response
}

void set_servos_from_network(Msg_t *update)
{
	if (update->pingmsg.id != TYPE_UPDATE) {
		return;
	}
	for (int i=1; i<6; i++)
		servo_update(i, update->respmsg.values[i]);
}
