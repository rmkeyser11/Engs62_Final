/*
 * mutex.h
 *
 *  Created on: Feb 9, 2016
 *      Author: matthew
 */

#ifndef MUTEX_H_
#define MUTEX_H_

void lock_mutex_blocking(int *mutex);
// Return 0 on success, else nonzero
int lock_mutex(int *mutex);

void unlock_mutex(int *mutex);


#endif /* MUTEX_H_ */
