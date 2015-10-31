/*
 * link.c
 *
 *  Created on: Apr 30, 2015
 *      Author: XIsolated
 */

#include "link.h"


client_in* client_link_init(void)
{
	client_in* tmp;
	tmp = (client_in*)malloc(sizeof(client_in));
	tmp->sockfd = -1;
	return tmp;
}

client_in* client_link_build(client_in* prv,int sockfd,struct sockaddr_in new_sock)
{
	if(prv == NULL || sockfd <=0)
	{
		printf("node is NULL or SOCKETFD is Vailed\n");
		return (client_in*)-1;
	}

	client_in* curr;
	curr  = (client_in*)malloc(sizeof(client_in));
	if(curr == NULL)
	{
		printf("Build Link Node Failed!\n");
		return (client_in*)-1;
	}
	prv	 -> next   = curr;
	curr -> sockfd = sockfd;
	curr -> new_sock = new_sock;
	curr -> next   = NULL;
	return curr;
}

client_in* client_link_delete(client_in* head,client_in* delp)
{
	if(head == NULL || delp == NULL)
	{
		printf("Pointer is NULL\n");
		return (client_in*)-1;
	}
	if(delp->next == NULL)
	{
		while(head->next !=delp)
			head = head->next;

		head->next = NULL;
		free(delp);
		delp = NULL;
		return head;
	}
	client_in* tmp = delp->next;
	*delp = *tmp;
	free(tmp);
	return 0;
}
void showall(client_in* head)
{
	while(1)
	{
		printf("%d \n",head->sockfd);
		if(head->next == NULL)
			break;
		head = head->next;
	}
}
