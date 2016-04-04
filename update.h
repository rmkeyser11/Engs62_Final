/*
 * update.h
 *
 *  Created on: Mar 6, 2016
 *      Author: matthew
 */

#ifndef UPDATE_H_
#define UPDATE_H_
#include "network.h"
void update_server_from_adc(void);
void update_server(int id, 	uint32_t data[5]);
void update_servos(void);
void set_servos_from_network(Msg_t *update);
#endif /* UPDATE_H_ */
