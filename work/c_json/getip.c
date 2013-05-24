#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
 struct hostent *h;
 char hostname[40];
 printf("请输入域名/n");
 scanf("%s",hostname);
 getchar();
 if((h=gethostbyname(hostname))==NULL)
 {
     fprintf(stderr,"不能得到IP/n");
     exit(1);
 }
 printf("HostName :%s/n",h->h_name);
 printf("IP Address :%s/n",inet_ntoa(*((struct in_addr *)h->h_addr)));
  return EXIT_SUCCESS;
}
