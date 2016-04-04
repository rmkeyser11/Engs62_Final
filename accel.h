/*
 * accel.h
 *
 *  Created on: Feb 18, 2016
 *      Author: matthew
 */

#ifndef ACCEL_H_
#define ACCEL_H_

void accel_init(void);
uint16_t accel_read(uint8_t accel_reg);
void accel_write(uint8_t accel_reg, uint8_t data);


#endif /* ACCEL_H_ */
