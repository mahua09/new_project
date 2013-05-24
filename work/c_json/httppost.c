#include<stdio.h>  
#include<string.h>  
#include<sys/socket.h>  
#include<stdlib.h>  
#include<netinet/in.h>  

#define Req "POST /upload.php HTTP/1.0\r\n" \ 
			"Accept:*/*\r\n"	\  
			"Accept-Language:en-us\r\n" \  
			"Accept-Encoding:gzip, deflate\r\n" \  
			"User-Agent:Mozilla/4.0\r\n"    \  
			"Host:192.168.1.180\r\n"    \  
			"Content-Type:application/x-www-urlencoded\r\n" \  
			"-----------------------------7d91a515b05a2\r\n"    \  
			"Content-Disposition:form-data;name=\"upload_file\";filename=\"tmp.gz\"\r\n"    \  
			"Content-Type:application/x-gzip\r\n"  


#define DST_IP "192.168.1.180"  
#define ReqLen sizeof(Req)  

int main()  
{  
	struct sockaddr_in srv;  
	int sock, nbytes;  
	char sndbuf[1024] = {0};  
	char recbuf[1024] = {0};  

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
	{  
		fprintf(stderr, "socket() error!\n");  
		exit(1);  
	}  

	srv.sin_family = AF_INET;  
	srv.sin_addr.s_addr = inet_addr(DST_IP);  
	srv.sin_port = htons(80);  

	if((connect(sock, (struct sockaddr *)&srv, sizeof(struct sockaddr))) == -1)  
	{  
		printf("connect() error!\n");  
		exit(1);  
	}  


	strncpy(sndbuf, Req, ReqLen);  
	if(write(sock, sndbuf, ReqLen) == -1)  
	{  
		fprintf(stderr, "write() error!\n");  
		exit(1);  
	}  

	int bytes;  
	while(1)  
	{  
		nbytes = read(sock, recbuf, 1023);  
		if(nbytes < 0)  
			break;  
		recbuf[nbytes] = '\0';  
		printf(recbuf);  
	}  

	close(sock);  

	return 0;  
}  
