#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
extern int errorno;
int main(int argc,char**argv)
{
	int i=open("/usr/src/1.t",0);
	perror(argv[0]);
	printf(strerror(errno));
	putchar('\n');
	return 0;
}
