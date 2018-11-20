#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <wait.h>
#include <stdbool.h>
#include <fcntl.h>
#define SHOW_P 0x01
#define SHOW_A 0x02
#define SHOW_S 0x04
#define SHOW_I 0x08

void* run(char* filename)
{
        char*source=filename;
        char*destination="/home/changkaiyan/changkaiyan";
        int sourcelength=strlen(source);
        int destilength=strlen(destination);
        _Bool flag,is_exit;
        char*p1=source+sourcelength-1;
        char*p2=destination+destilength-1;
        while(*p1--==*p2--&&*p1!='.');
        if(*p1=='.'&&*p2=='.')//判断文件扩展名是否相同，相同即为从文件到文件
        {
            flag=true;
            while(*p1==*p2&&*p1!='/')p1--,p2--;
            if(*p1==*p2&&*p1=='/'&&*p2=='/')
                is_exit=true;
            else
                is_exit=false;
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
                fd=open(destination,O_WRONLY|O_CREAT|O_TRUNC);
            else
            {
                char option;
                printf("覆盖for y；合并 for n\n");
                scanf("%c",&option);
                if(option=='y')
                    fd=open(destination,O_WRONLY|O_CREAT|O_TRUNC,0xff);
                else
                    fd=open(destination,O_APPEND|O_WRONLY);
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
                if(strcmp(dirp->d_name,filename)==0)
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
        }

    pthread_exit(NULL);
}
int main(int argc,char**argv) {
    int8_t flag=0;
    for(int i=0;i<argc;++i)//Assign the flag bit
    {
        if(strcmp(argv[i],"-p")==0)
            flag|=SHOW_P;
        if(strcmp(argv[i],"-a")==0)
            flag|=SHOW_A;
        if(strcmp(argv[i],"-s")==0)
            flag|=SHOW_S;
        if(strcmp(argv[i],"-i")==0)
            flag|=SHOW_I;

    }
    char* now_dirent=getcwd(NULL,0);//Get now dirent
    DIR* dirent=opendir(now_dirent);//Open the current dirent structure
    if(dirent==NULL)
    {
        fprintf(stderr,"The error occur,can not open dirent %s",now_dirent);
        return 0;
    }
    struct dirent* difile;
    char* now_file;
    struct stat buf;
    mode_t mo;
    uid_t ui;
    gid_t gi;
    struct passwd* pad;
    struct group* gp;
    struct tm* tim;
    while((difile=readdir(dirent))!=NULL)
    {
        now_file=difile->d_name;
        if(strcmp(now_file,".")==0||strcmp(now_file,"..")==0)
            if(!(SHOW_P&flag))
                continue;
        if(now_file[0]=='.'&&strcmp(now_file,".")!=0&&strcmp(now_file,"..")!=0)
            if(!(SHOW_A&flag))
                continue;
        if(SHOW_S&flag)
            stat(now_file,&buf);
        else
            lstat(now_file,&buf);
        mo=buf.st_mode;
        if(!S_ISDIR(mo))
        {

            pthread_t pi;
            char filename[60]="./";
            strcat(filename,now_file);
            pthread_create(&pi,NULL,(void*)run,filename);
            pthread_join(pi,NULL);
        }

        if(S_ISREG(mo))
            putchar('-');
        else if(S_ISBLK(mo))
            putchar('b');
        else if(S_ISCHR(mo))
            putchar('c');
        else if(S_ISDIR(mo))
            putchar('d');
        else if(S_ISFIFO(mo))
            putchar('p');
        else if(S_ISLNK(mo))
            putchar('l');
        else if(S_ISSOCK(mo))
            putchar('s');
        else
            fprintf(stderr,"An error has occured!Please check the file!");
        if(S_IRUSR&mo)
            putchar('r');
        else
            putchar('-');
        if(S_IWRITE&mo)
            putchar('w');
        else
            putchar('-');
        if(S_IXUSR&mo)
            putchar('x');
        else
            putchar('-');
        if(S_IRGRP&mo)
            putchar('w');
        else
            putchar('-');
        if(S_IWRITE&mo)
            putchar('r');
        else
            putchar('-');
        if(S_IXGRP&mo)
            putchar('x');
        else
            putchar('-');
        if(S_IROTH&mo)
            putchar('w');
        else
            putchar('-');
        if(S_IWRITE&mo)
            putchar('r');
        else
            putchar('-');
        if(S_IXOTH&mo)
            putchar('x');
        else
            putchar('-');
        putchar(' ');
        printf("%d",buf.st_nlink);
        putchar(' ');
        pad=getpwuid(buf.st_uid);
        gp=getgrgid(buf.st_gid);
        tim=localtime(&buf.st_mtim);
        printf("%s %s %ld %d-%d-%d %d:%d %s\n", pad->pw_name,gp->gr_name,buf.st_size,\
        tim->tm_year+1900,tim->tm_mon,tim->tm_mday,tim->tm_hour,tim->tm_min,now_file);
    }

    if(closedir(dirent)==-1)
    {
        fprintf(stderr,"Can not close the dirent!");//An error occur that the current dirent can not be closed
        return 0;
    }
    free(now_dirent);
    return 0;
}
