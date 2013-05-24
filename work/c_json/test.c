#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *get_q_str(char *src)
{
   	int i=1;
   	char *tmp = NULL;
   	for(i=1;*(src+i)!='\0';i++)
	{
		if( *(src+i)=='q')
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

int main(void)
{
	char *a="fiquj aq12.3 awq";
	char *b=NULL;
	b = get_q_str(a);
	//memset(b,0,sizeof(b));
	//strncpy(b,a,strlen(a));
	//strncpy(b,get_q_str(b),strlen(get_q_str(b)));
	printf("%s\n",b);
	
	//printf("%s\n",get_q_str(b));
	return 0;
}
