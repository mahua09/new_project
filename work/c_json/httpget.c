#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "httpget.h"

#define TAX_CURRENCY "TAX   CNY"
#define CURRENCY "CNY"
#define ERROR_AIRLINE "FSQ01"
#define XT_CURRENCY "XT CNY"
#define TOTAL_FARE "TOTAL"
#define TOTAL_STR "TOTAL CNY"
#define ENDOS_STR "ENDOS"
#define AUTO_STR "*AUTO"
#define RATE_USED_STR "RATE USED"
#define END_STR "END"
#define XT_DETAIL "xtdetail"

char result[256]={0};

char *get_Q_str(char *src)
{
	int i=1;
	char *tmp = NULL;
	for(i=1;*(src+i)!='\0';i++)
	{
		if( *(src+i)=='Q')
		{
			if(*(src+i+1) >= '0' && *(src+i+1) <= '9')
			{
				tmp=src+i;
				break;
			}
		}
	}
	return tmp;
}

int get_Q_num(char *src)
{
	int i=1,num=0;

	for(i=1;*(src+i)!='\0';i++)
	{
		if( *(src+i)=='Q')
		{
			if(*(src+i+1) >= '0' && *(src+i+1) <= '9')
			{
				num++;
			}
		}
	}
	return num;
}

char *get_Q_value(char *src)
{
	int i=0;
	char *tmp=src+1;

	for(i=0;*(tmp+i);i++){
		if(*(tmp+i)=='.')continue;
		if(*(tmp+i)<'0'|| *(tmp+i)>'9'){
			*(tmp+i)='\0';
			break;
		}
	}
	return tmp;
}

char *get_ROE_value(char *src)
{
	int i=0;
	char *tmp=src;

	for(i=0;*(tmp+i)!=' ';i++){
	    if(*(tmp+i)>'0'&&*(tmp+i)<'9')
            break;
	}
	tmp=src+i;

	for(i=0;*(tmp+i);i++){
	    if(*(tmp+i)=='.')continue;
		if(*(tmp+i)<'0'|| *(tmp+i)>'9'){
			*(tmp+i)='\0';
			break;
		}
	}
	return tmp;
}

int judge_XT_num(char *src)
{
	int i=0,num=0;
	char *tmp=NULL;
	for(i=0;*(src+i);i++)
	{
		if(*(src+i)=='\n')
			break;
	}
	tmp = strstr(src,CURRENCY);
	while(tmp != NULL)
	{
		if(tmp-src<i){
			num++;
			tmp=strstr(tmp+3,CURRENCY);
		}
		else
			break;
	}
	return num;
}

char *get_TOTAL_value(char *src)
{
	int i=0;
	char *tmp=src;
	for(i=0;*(src+i);i++)
	{
		if(*(src+i)=='\n')
		{
			*(tmp+i)='\0';
			break;
		}
	}
	return tmp;
}

char * trim(char *str)
{
		char *str_last,*str_cur;
		if(str==NULL)
				return NULL;
		for(;*str==' ' || *str=='\t'; ++str);
		for(str_last=str_cur=str;*str_cur!='\0';++str_cur)
				if(*str_cur!=' '&&*str_cur!='\t'&&*str_cur!='\n')
						str_last=str_cur;
		*++str_last=0;
		return str;
}

char *get_last2str(char *src)
{
		return src+strlen(src)-2;
}

char *del_last2str(char *src)
{
		int i=strlen(src);
		*(src+i) = '\0';
		*(src+i-1) = '\0';
		return src;
}

char *string_analyze(char *buf)
{
		//char result[256]={0};
		char *tmp=NULL;
		char *str1=NULL;
		char *str2=NULL;
		char *q=NULL;
		char p[20]={0};
		int i=0,num=0;

		memset(p,0,sizeof(p));
		memset(result, 0, sizeof(result));
		strncat(result,"{", 1);

		str1 = strstr(buf,TAX_CURRENCY);
		if(str1 == NULL){
			//printf("########result not found#######\n");
			//printf("%s\n",strstr(buf,"FSQ01"));
			return strstr(buf,ERROR_AIRLINE);
		}
		//printf("##########str1#########\n");
		//printf("%s", str1);

		tmp = strstr(str1, CURRENCY);
		//printf("##########tmp#########\n");
		//printf("%s\n",tmp);
		num = judge_XT_num(tmp);
		//printf("judge_XT_num %d.\n",num);
		tmp = tmp+3;
		while((str2=strstr(tmp,CURRENCY))!=NULL)
		{
				memset(p,0,sizeof(p));
				strncpy(p,tmp,str2-tmp);
				strncat(result,"\"",1);
				strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
				strncat(result,"\":",2);
				strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
				strncat(result,",",1);

				tmp = str2;
				tmp = tmp+3;
				i++;
				if(i== num-1){
						str2=strstr(tmp,TOTAL_FARE);
						strncpy(p,tmp,str2-tmp);
						strncat(result,"\"",1);
						strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
						strncat(result,"\":",2);
						strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
						strncat(result,",",1);
						break;
				}
		}
		num = 0;
		//printf("*******next oprate***********\n");
		//printf("%s\n",trim(str2));
		strncat(result,"\"Q\":",4);
		str1 = NULL;
		tmp = NULL;

		num = get_Q_num(str2);
		//printf("Q numbers is:%d.\n",num);
		//str1 = get_Q_str(str2);
		if(num >= 1){
				str1 = get_Q_str(str2);
				q=(char *)malloc(1024);
				memcpy(q,str1,strlen(str1));

				if(num == 1)
				{
					tmp = get_Q_value(q);
				//	printf("*******GET num=1 Q value***********\n");
				//	printf("%s\n",tmp);
					strncat(result,trim(tmp),strlen(trim(tmp)));
					strncat(result,",",1);

				}else{
					strncat(result,"{",1);
					while(q!=NULL){
						tmp = get_Q_value(q);
					//	printf("*******GET num>1 Q value***********\n");
					//	printf("%s\n",tmp);
						strncat(result,trim(tmp),strlen(trim(tmp)));
						strncat(result,",",1);

						str1++;
						tmp = NULL;
						q = get_Q_str(str1);
					}
					strncat(result,"}",1);
					strncat(result,",",1);
				}
				free(q);
				num = 0;
				tmp = NULL;
				str1 = NULL;
				str1 = strstr(str2,END_STR);
			//	q = NULL;
				q=(char *)malloc(1024);
				memcpy(q,trim(str1+3),strlen(trim(str1+3)));
			//	q=trim(str1+3);
				tmp = get_ROE_value(q);
		//		printf("*******ROE value***********\n");
		//		printf("%s\n",tmp);
				strncat(result,"\"ROE\":",6);
				strncat(result,tmp,strlen(tmp));
				strncat(result,",",1);
				free(q);
		}else if(num == 0){
				strncat(result,"0,",2);
		}
		//**************get XT detail**********
		//printf("*******get XT detail**********\n");
		//printf("%s\n",trim(str2));
		i=0;
		tmp = strstr(str2,XT_CURRENCY);
		if(tmp!=NULL){
				strncat(result,"\"",1);
				strncat(result,XT_DETAIL,8);
				strncat(result,"\":",2);
				strncat(result,"{",1);
				//tmp = tmp+6;
			for(;;){
				num = judge_XT_num(tmp);
				//printf("*******XT numbers*****\n");
				//printf("judge_XT_num %d.\n",num);
				//printf("tmp === %s\n",tmp);
				tmp = tmp+6;

			//	if(strstr(tmp,CURRENCY)&&(strstr(str2,"ENDOS")||strstr(str2,"RATE USED")||strstr(str2,"*AUTO"))){
				if(num > 1){
					while((str2=strstr(tmp,CURRENCY))){
					//	if(strstr(str2,"ENDOS")||strstr(str2,"RATE USED")||strstr(str2,"*AUTO")){
						memset(p,0,sizeof(p));
						strncpy(p,tmp,str2-tmp);
						strncat(result,"\"",1);
						strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
						strncat(result,"\":",2);
						strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
						strncat(result,",",1);
						tmp = str2;
						tmp = tmp+3;
						i++;

						//	if(num <= 5){
							if(i >= num-1){
								if(strstr(tmp,XT_CURRENCY)){
									str2=strstr(tmp,XT_CURRENCY);
									strncpy(p,tmp,str2-tmp);
									strncat(result,"\"",1);
									strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
									strncat(result,"\":",2);
									strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));

									tmp = str2;
									strncat(result,",",1);
								}
								else if(strstr(tmp,ENDOS_STR)){
									str2=strstr(tmp,ENDOS_STR);
									strncpy(p,tmp,str2-tmp);
									strncat(result,"\"",1);
									strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
									strncat(result,"\":",2);
									strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
								}
								else if(strstr(tmp,RATE_USED_STR)){
									str2=strstr(tmp,RATE_USED_STR);
									strncpy(p,tmp,str2-tmp);
									strncat(result,"\"",1);
									strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
									strncat(result,"\":",2);
									strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
								}
								else{
									str2=strstr(tmp,AUTO_STR);
									strncpy(p,tmp,str2-tmp);
									strncat(result,"\"",1);
									strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
									strncat(result,"\":",2);
									strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
								}

								memset(p,0,sizeof(p));
								break;
							}
					//	}
						str2 = NULL;
					//	}
					}
				}
				else{
					if(strstr(tmp,ENDOS_STR)){
						str2=strstr(tmp,ENDOS_STR);
						strncpy(p,tmp,str2-tmp);
						strncat(result,"\"",1);
						strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
						strncat(result,"\":",2);
						strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
					}
					else if(strstr(tmp,RATE_USED_STR)){
						str2=strstr(tmp,RATE_USED_STR);
						strncpy(p,tmp,str2-tmp);
						strncat(result,"\"",1);
						strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
						strncat(result,"\":",2);
						strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
					}
					else{
						str2=strstr(tmp,AUTO_STR);
						strncpy(p,tmp,str2-tmp);
						strncat(result,"\"",1);
						strncat(result,get_last2str(trim(p)),strlen(get_last2str(trim(p))));
						strncat(result,"\":",2);
						strncat(result,del_last2str(trim(p)),strlen(del_last2str(trim(p))));
					}
					break;
				}


				if(strstr(str2,XT_CURRENCY)==NULL){
					str2 = NULL;
					break;
				}
			}
			strncat(result,"},",2);
		}

		str1=NULL;
		str1=strstr(buf,TOTAL_STR);
		strncat(result,"\"TOTAL\":",8);
		strncat(result,get_TOTAL_value(trim(str1+9)),strlen(get_TOTAL_value(trim(str1+9))));
		strncat(result,"}", 1);
		//printf("##########result##########\n");
		//printf("%s\n", result);
		return result;
}

char *get_json(char *url)
{
	char *data="GET /data-qte/";
	int sockfd, ret, i;
	struct sockaddr_in servaddr;
	char str1[4096], buf[BUFSIZE];
	fd_set   t_set1;
	char city[36];
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("创建网络连接失败,本线程即将终止---socket error!\n");
		exit(0);
	};

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
		printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
		exit(0);
	};

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		printf("连接到服务器失败,connect error!\n");
		exit(0);
	}
	printf("与远端建立了连接\n");

	memset(city, 0, 100);
	strcat(city, data);
	strcat(city, url);
	strcat(city, " HTTP/1.1\n");
	memset(str1, 0, 4096);
	strcat(str1, city);
	strcat(str1, "Host: 10.124.20.136\n");
	strcat(str1, "Content-Type: application/x-www-form-urlencoded\n");
	strcat(str1, "Content-Length: 0");
	strcat(str1, "\n\n");

	strcat(str1, "\r\n\r\n");
	printf("%s\n",str1);

	ret = write(sockfd,str1,strlen(str1));
	if (ret < 0) {
		printf("发送失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
		exit(0);
	}else{
		printf("消息发送成功，共发送了%d个字节！\n\n", ret);
	}

	FD_ZERO(&t_set1);
	FD_SET(sockfd, &t_set1);

	memset(buf, 0, 4096);
	i = read(sockfd, buf, 4095);
	if (i==0){
		close(sockfd);
		printf("读取数据报文时发现远端关闭，该线程终止！\n");
		return NULL;//-1;
	}

	printf("%s\n", buf);

	string_analyze(buf);
	close(sockfd);

	printf("%s\n",result);
	return result;
}

//int main(int argc, char **argv)
//{
//	get_json("hkg/mia/aa/");
//	return 0;
//}
