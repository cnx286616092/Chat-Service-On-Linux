/*
 * server.c
 *
 *  Created on: May 3, 2015
 *      Author: XIsolated
 */

#include "server.h"
#include "mess.h"

int maxsockfd = 0;
int num_sock = 0;
int pip[2];	//Pipe

client_in* 	fd_a;
client_in*	fd_h;
pthread_rwlock_t sock_lock;

void* listening_thread(void)
{
	int listen_sock;
	struct sockaddr_in listen_add;

	listen_sock = socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock < 0)
	{
		printf("Build Socket Failed!\n");
		exit(1);
	}
	listen_add.sin_family = AF_INET;
	listen_add.sin_port   = htons(PORT);
	listen_add.sin_addr.s_addr = INADDR_ANY;

	if( bind(listen_sock, (struct sockaddr*)&listen_add, sizeof(struct sockaddr) ) < 0 )
	{
		printf("Bind Failed!\n");
		exit(1);
	}

	listen(listen_sock,20);
	char* ip;
	while(1)
	{
		if(num_sock < CLIENTNUM)
		{
			struct sockaddr_in new_sock;
			socklen_t new_length = sizeof(struct sockaddr_in);
			int sock = accept(listen_sock, (struct sockaddr*)&new_sock, &new_length);
			ip = inet_ntoa(new_sock.sin_addr);
			if(fd_a != NULL && sock >0)
			{
				pthread_rwlock_wrlock(&sock_lock);
				fd_a = client_link_build(fd_a,sock,new_sock);
				pthread_rwlock_unlock(&sock_lock);
				num_sock ++;

				printf("%s:%d is conneceed!\n",ip,new_sock.sin_port);
			}
			else
			{
				printf("%s:%d  connected !\n",ip,new_sock.sin_port);
			}
		}
		else
		{
			sleep(1);
		}

	}
	close(listen_sock);
	return NULL;
}
void* client_thread(void)
{
	fd_set fdread;

	char buffer[BUFFER_SIZE];
	struct timeval timeout;

	int nerr = 0;
	while(1)
	{
		FD_ZERO(&fdread);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		client_in* tmp = fd_h;
		client_in* tmp_prv = NULL;
		maxsockfd = 0;
		while(num_sock > 0)
		{
			if(tmp->sockfd > 0)
				FD_SET(tmp->sockfd,&fdread);

			if(tmp->sockfd > maxsockfd)
				maxsockfd = tmp->sockfd;

			if(tmp->next == NULL)
				break;

			tmp = tmp -> next;

		}


		if(num_sock > 0)
		{
			nerr = select(maxsockfd + 1,&fdread,NULL,NULL,&timeout);
			switch(nerr)
			{
				case -1 :printf("select error!\n");break;
				case 0: break;
				default:
				{
					pthread_rwlock_rdlock(&sock_lock);
					tmp = fd_h;
					char* ip = NULL;
					while(1)
					{
						if( tmp->sockfd >0 && FD_ISSET(tmp->sockfd, &fdread) )
						{
							bzero(buffer,BUFFER_SIZE);
							nerr = recv(tmp->sockfd,buffer,BUFFER_SIZE,0);
							ip = inet_ntoa(tmp->new_sock.sin_addr);
							if(nerr == -1 || nerr == 0)
							{
								close(tmp->sockfd);
								FD_CLR(tmp->sockfd,&fdread);
								pthread_rwlock_unlock(&sock_lock);

								if(num_sock >0)
									num_sock--;
								pthread_rwlock_wrlock(&sock_lock);

								printf("%d socket from %s:%d is closed\n",tmp->sockfd,ip,tmp->new_sock.sin_port);
								int t = (int)client_link_delete(fd_h,tmp);

								if(t > 0)
								{
									fd_a = (client_in*)t;
									tmp = fd_a;
								}
								else if(t == 0)
								{
									tmp = tmp_prv;
								}
							}
							else
							{
								printf("Received form %s:%d (%d socket) is : %s\n" , ip,tmp->new_sock.sin_port,tmp->sockfd , buffer);
								char* massage = NULL;
								massage = (char*)malloc((nerr+1) * sizeof(char));
								strcpy(massage,buffer);

								write(pip[1],&massage,sizeof(massage));
								write(pip[1],&tmp,sizeof(tmp));
							}
						}
						if(tmp->next == NULL)
							break;
						tmp_prv = tmp;
						tmp = tmp->next;
					}
					pthread_rwlock_unlock(&sock_lock);
				}
			}
		}
		else
		{
			sleep(1);
		}
	}
}
void* send_thread(void)
{
	char* str = NULL;
	char* noclient = "系统:该用户不在线";
	client_in* from ;
	client_in* to ;

	char** name = (char**)malloc(sizeof(char*));
	char** sendto = (char**)malloc(sizeof(char*));

	while(1)
	{
		read(pip[0], &str,sizeof(str));
		read(pip[0], &from ,sizeof(from));
		char* tmp = str;

		mess_prce(str , sendto , name);
		int twolen = strlen(*sendto) + strlen(*name) +18;

		if(twolen == strlen(str))
		{
			strcpy(from->name,*name);
			client_in* allname = fd_h -> next;
			char* sum = (char*)malloc(2);
			*sum = 0;
			int len_sum = 0;
			while(1)
			{
				int len = strlen(allname->name);
				len_sum = len_sum + len + 4 ;
				sum = (char*)realloc(sum ,len_sum * sizeof(char));

				strcat(sum , allname->name);
				strcat(sum , ".!.");

				if(allname->next == NULL)
					break;

				allname = allname->next;
			}
			send(from->sockfd , sum , strlen(sum) , 0);
		}
		else
		{
			to = findclient(*sendto);
			if(  to == NULL )
			{
				send(from->sockfd , noclient , strlen(noclient),0);
			}
			else
			{
				str = str + strlen(*sendto) + 10;
				printf("send :%s\n",str);
				send(to->sockfd , str , strlen(str) , 0);
			}
		}
		free(tmp);
		free(*name);
		free(*sendto);
	}
	return NULL;
}
client_in* findclient(char* sendto)
{
	client_in* tmp = fd_h;
	while(1)
	{
		if( strcmp(sendto , tmp->name) == 0)
		{
			return tmp;
		}


		if(tmp->next == NULL)
			break;

		tmp = tmp->next;
	}
	return NULL;
}
void server_init(void)
{
	pthread_t pdl,pdc,pds;
	fd_a = client_link_init();
	fd_h = fd_a;

	mess_init();

	if( pipe(pip) < 0)
	{
		printf("Pipe Build failed!\n");
	}

	pthread_rwlock_init(&sock_lock,NULL);

	pthread_create(&pdl,NULL,(void*)listening_thread,NULL);
	pthread_create(&pdc,NULL,(void*)client_thread,NULL);
	pthread_create(&pds,NULL,(void*)send_thread,NULL);

	pthread_join(pdl,NULL);
	pthread_join(pdc,NULL);
	pthread_join(pdc,NULL);

	return;
}
