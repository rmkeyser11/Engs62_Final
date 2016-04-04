/*
 * io.h
 *
 *  Created on: Feb 9, 2016
 *      Author: matthew
 */

#ifndef IO_H_
#define IO_H_

#include "stdint.h"
#include "network.h"
void println(uint32_t val);
void printHex(uint16_t val);
void printSignedDecimal(int16_t val);
void printUnsignedDecimal(int16_t val);
void print_string(char *str);
void print_msg(Msg_t *msg);
#endif /* IO_H_ */
