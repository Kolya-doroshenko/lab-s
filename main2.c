#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define null 0
#define STATUS_SUCCESS 0
#define STATUS_ERROR 1


// ініціалізація змінних
int sockfd; 
struct sockaddr_in servaddr, cliaddr, sockaddr;/*структури для адрес сервера і клієнта*/
char msg[2048];/*максимальний рядок відсилання, масив*/
int clilen, n, server_status, prog_status;/*змінні для різних довжин і кількості символів*/

int message_service(char* _msg, struct sockaddr_in client)
{
	return printf("Sent:\n{\n 	client_ip: %s\n 	msg: %s",
			client.sin_port,
			_msg);
}

int init_server(char* ip, int port)
{
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family=AF_INET; // ???
	servaddr.sin_addr.s_addr=inet_addr(ip); // ip address
	servaddr.sin_port=htons(port); // port to connect
	return 0;
}

int create_socket()
{
	if((sockfd=socket(PF_INET, SOCK_DGRAM, 0))<0)
	{
		perror(NULL); 
		return STATUS_ERROR;
	}
	
	if(bind(sockfd, ( struct sockaddr* ) &servaddr, sizeof(servaddr))<0)
	{
		perror(null);
		close(sockfd);
		return STATUS_ERROR;
	}	
	return STATUS_SUCCESS;
}

int message_control()
{
	clilen=sizeof(cliaddr);
	
	if((n=recvfrom(sockfd, msg, 999, 0, ( struct sockaddr* ) &cliaddr, &clilen))<0)
	{
		perror(null);
		close(sockfd);
		return 1;
	}	
	
	message_service(msg, cliaddr);
	
	if((sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*) &cliaddr, clilen))<0)
	{
		perror(NULL);
		close(sockfd);
		return 1;
	}
}

int async_wait_msg()
{
	return message_control();
}


int main(int argc, char *argv[])
{
	// init server first
	if (argc == 3)
	{
	 server_status = init_server(argv[1], atoi(argv[2]));
	} else
	{
		printf("prog <ip> <port>");
	}
	
	// create udp server
	if ((server_status = create_socket()) != 0) 
		printf("Can't create socket. Server status: %d", server_status);
	else 
	{
		pthread_t pthread_id;
		printf("Socket created, server wait for message...");
		server_status = pthread_create(&thread_id, null, *async_wait_msg, null);
		while (pthread_join(thread_id, null)!=0)
			printf("Server waited...");
	}
	printf("Last server status: %d", server_status);
	return server_status;
}



