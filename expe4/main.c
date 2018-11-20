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
#include <pthread.h>

#define SHOW_P 0x01
#define SHOW_A 0x02
#define SHOW_S 0x04
#define SHOW_I 0x08
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
            pid_t pi=fork();
            char filename[60]="./";
            strcat(filename,now_file);
            if(pi==0)//Child thread
            {
                execl("/home/changkaiyan/linuxexperiment/linuxexpe/expe4/mycp","mycp",filename,"/home/changkaiyan/changkaiyan",NULL);
                exit(EXIT_SUCCESS);

            }


            wait(NULL);

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
        printf("%s %s %d %d-%d-%d %d:%d %s\n", pad->pw_name,gp->gr_name,buf.st_size,\
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