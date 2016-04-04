/*
 * servo.h
 *
 *  Created on: Feb 16, 2016
 *      Author: matthew
 */

#ifndef SERVO_H_
#define SERVO_H_
#include "stdint.h"

void servo_init(void);
void servo_update(int id, uint32_t t_high);

#endif /* SERVO_H_ */
