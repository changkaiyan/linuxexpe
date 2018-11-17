#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<error.h>
#include<stdbool.h>
#include<sys/types.h>
#include<dirent.h>

extern int errorno;
int main(int args,char**argv)
{
	char*source=argv[1];
	char*destination=argv[2];
	int sourcelength=strlen(source);
	int destilength=strlen(destination);
	_Bool flag,is_exit;
	char*p1=source+sourcelength-1;
	char*p2=destination+destilength-1;
	while(*p1==*p2&&*p1!='.'&&p1>=source)p1--,p2--;
	if(*(p1)=='.'&&*(p2)=='.')//判断文件扩展名是否相同，相同即为从文件到文件
	{

		flag=true;
		char path[60]={'\0'};
		strcpy(path,destination);
		for(int i=destilength-1;i>0;--i)
		{
			if(destination[i]=='/')
				{path[i]='\0';break;}
		}
		DIR*di=opendir(path);p1=source+sourcelength-1;
		
		if(di==NULL)
		{	
			perror(destination);//异常处理
			return 1;
		}
		char*filename;
		while(*p1!='/')
			p1--;
		filename=p1;
		struct dirent* dirp;
		is_exit=false;
		while((dirp=readdir(di))!=NULL)
		{
			if(strcmp(dirp->d_name,filename+1)==0)
			{
				is_exit=true;
				break;
			}
		}
	}
	else//从文件到目录
	{
		flag=false;
		p1=source+sourcelength-1;
		
	}
	if(flag==true)//从文件复制到文件
	{
		int rfd=open(source,O_RDONLY);
		int fd;
		if(is_exit==false)
			fd=open(destination,O_WRONLY|O_CREAT|O_TRUNC,0xffff);
		else
		{
			char option;
			printf("覆盖for y；合并 for n\n");
			scanf("%c",&option);
			if(option=='y')
				fd=open(destination,O_WRONLY|O_CREAT|O_TRUNC,0xffff);
			else
				fd=open(destination,O_APPEND|O_WRONLY,0xffff);
		}
		char ch;
		
		while(read(rfd,&ch,1)!=0)
		{
			if(write(fd,&ch,1)==-1)
			{
				fprintf(stderr,"写入文件错误");
				break;
			}
		}
		if(close(rfd)==0&&close(fd)==0);
		else
		{
			fprintf(stderr,"关闭文件出错");
		}
			
	}
	else//从文件复制到目录
	{
		DIR*di=opendir(destination);p1=source+sourcelength-1;
		
		if(di==NULL)
		{	
			perror(destination);//异常处理
			return 1;
		}
		char*filename;
		while(*p1!='/')
			p1--;
		filename=p1;
		struct dirent* dirp;
		is_exit=false;
		while((dirp=readdir(di))!=NULL)
		{
			if(strcmp(dirp->d_name,filename+1)==0)
			{
				is_exit=true;
				break;
			}
		}
		char option='y';
		if(is_exit==true)
		{
			printf("覆盖for y；合并 for n");
			scanf("%c",&option);
		}
		char path[80]={'\0'};
		strcpy(path,destination);
		strcat(path,filename);
		int rfd=open(source,O_RDONLY);
		int fd;
		if(option=='y')
		{
		
		fd=open(path,O_WRONLY|O_CREAT|O_TRUNC,0xffff);
		char ch;
		if(closedir(di)==-1)
			perror("destination");
		while(read(rfd,&ch,1)!=0)
		{
			if(write(fd,&ch,1)==-1)
			{
				fprintf(stderr,"写入文件错误");
				break;
			}
		}
		}
		else
		{
			fd=open(path,O_APPEND|O_WRONLY,0xffff);
			char ch;
			if(closedir(di)==-1)
				perror("destination");
			while(read(rfd,&ch,1)!=0)
			{
				if(write(fd,&ch,1)==-1)
			{
				fprintf(stderr,"写入文件错误");
				break;
			}
			
		}
		if(close(rfd)==0&&close(fd)==0);
		else
		{
			fprintf(stderr,"关闭文件出错");
		}
	}
	return 0;
}
}
