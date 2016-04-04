/*
 * USART2.h
 *
 *  Created on: Feb 9, 2016
 *      Author: matthew
 */

#ifndef USART2_H_
#define USART2_H_

void USART2_init(void);
void USART2_send(int c);
int USART2_recv(void);

void __attribute__ ((interrupt)) USART2_handler(void);

#endif /* USART2_H_ */
