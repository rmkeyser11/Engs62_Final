/*
 * USART3.h
 *
 *  Created on: Feb 28, 2016
 *      Author: matthew
 */

#ifndef USART3_H_
#define USART3_H_

void USART3_init(void);
void USART3_send(int c);
int USART3_recv(void);

void __attribute__ ((interrupt)) USART3_handler(void);

#endif /* USART3_H_ */
