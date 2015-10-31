/*
 * server.h
 *
 *  Created on: May 3, 2015
 *      Author: XIsolated
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "link.h"

#define PORT		(6000)
#define CLIENTNUM	(100)
#define BUFFER_SIZE (1024)

extern int maxsockfd;
extern int num_sock;


extern void* listening_thread(void);
extern void* client_thread(void);
extern void* send_thread(void);
extern void server_init(void);
extern client_in* findclient(char* );

#endif /* SERVER_H_ */
