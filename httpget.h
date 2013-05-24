#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPSTR "10.124.20.136"
#define PORT 80
#define BUFSIZE 1024

char *get_Q_str(char *src);

int get_Q_num(char *src);

char *get_Q_value(char *src);

char *get_ROE_value(char *src);

int judge_XT_num(char *src);

char *get_TOTAL_value(char *src);

char * trim(char *str);

char *get_last2str(char *src);

char *del_last2str(char *src);

char *string_analyze(char *buf);

char *get_json(char *url);
