/*
 * network.h
 *
 *  Created on: Feb 29, 2016
 *      Author: matthew
 */

#ifndef NETWORK_H_
#define NETWORK_H_

// Types here are taken from udp62.c file provided
/* message types */
#define TYPE_PING 1
#define TYPE_UPDATE 2
#define CLASS_SIZE_MAX 30

/* IDs and our group's UDP port */
#define PIVOT_ID 0
#define SHOULDER_ID 1
#define ELBOW_ID 2
#define WRIST_ID 3
#define GRIP_ID 4
#define JUNK_ID 18


typedef struct {
  int type;
  int id;
} Ping_t;

typedef struct {
  int type;
  int id;
  int value;
} Update_req_t;

typedef struct {
  int type;
  int id;
  int average;
  int values[CLASS_SIZE_MAX];
} Update_resp_t;

typedef union {
  Ping_t pingmsg;
  Update_req_t reqmsg;
  Update_resp_t respmsg;
} Msg_t;

extern volatile Msg_t recv_msg;
extern volatile int recv_offset;

void send_ping(void);
void send_update(int val);
void send_packet_USART3(Msg_t *msg);
void receive_packet_USART3(void);

#endif /* NETWORK_H_ */
