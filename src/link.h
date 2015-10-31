/*
 * link.h
 *
 *  Created on: Apr 30, 2015
 *      Author: XIsolated
 */

#ifndef LINK_H_
#define LINK_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct client_in
{
	int sockfd;
	char name[20];

	struct sockaddr_in new_sock;
	struct client_in* next;
}client_in;


extern client_in* client_link_init(void);
extern client_in* client_link_build(client_in* ,int ,struct sockaddr_in);
extern client_in* client_link_delete(client_in* ,client_in*);
void showall(client_in*);

#endif /* LINK_H_ */
