#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdarg.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>  

#define BUFSIZE 204800 
int main(int argc, char *argv[]) 
{     
		if (argc != 2) {         
				printf("Input your host.example:%s www.baidu.com\n",argv[0]);         
				printf("Or input your host.example:%s 220.181.111.148\n",argv[0]);         
				exit(1);     
		}     
		struct hostent * site;     
		char *domain=argv[1];      
		int servPort=80;     
		site = gethostbyname(domain);
		printf("FFFFFqq");         
		char *echoString;         
		memset(echoString, 0, 4096);         
		strcat(echoString, "GET / HTTP/1.1\n");         
		strcat(echoString, "Host:");         
		strcat(echoString,domain);         
		strcat(echoString, "\n");         
		strcat(echoString, "User-Agent: Mozilla/5.0 (Windows; U;Windows NT 5.1; en-US; rv:1.7.6) Gecko/20050225 Firefox/1.0.1\n");         
		strcat(echoString, "Connection: Keep-Alive");        
		strcat(echoString, "\n\n");     
		int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);     
		printf("FFFFFqq");         
		if (sock < 0) {         
				printf("socket() failed\n");         
				exit(1); 
		}     
		struct sockaddr_in servAddr;      
		memset(&servAddr, 0, sizeof(servAddr));     
		printf("%s,%d",site->h_addr_list[0],site->h_length);
		memcpy(&servAddr.sin_addr,site->h_addr_list[0],site->h_length);     
		servAddr.sin_family = AF_INET;     
		servAddr.sin_port = htons(servPort);     
		if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {         
				printf("connect() failed\n");         
				exit(1);    
		}     
		size_t echoStringLen = strlen(echoString);      
		ssize_t numBytes = send(sock, echoString, echoStringLen, 0);     
		if (numBytes < 0) 
		{         
				printf("send() failed");         
				exit(1);     
		}     
		else if (numBytes != echoStringLen) {         
				printf("send(), sent unexpected number of bytes");         
				exit(1);     
		}     
		unsigned int totalBytesRcvd = 0;     
		while (totalBytesRcvd < echoStringLen) {         
				char buffer[BUFSIZE];        
				numBytes = recv(sock, buffer, BUFSIZE, 0);         
				if (numBytes < 0) {             
						printf("recv() failed");             
						exit(1);         
				}         
				else if (numBytes == 0) {             
						printf("recv(), connection closed prematurely");             
						break;         
				}         
				totalBytesRcvd += numBytes;          
				buffer[numBytes] = '\0';           
				printf(buffer);      
		}     
		printf("\n");     
		close(sock);     
		exit(0); 
} 
