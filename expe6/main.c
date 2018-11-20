#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define BRAINNUM 25
void* brain_lock(const int*para);
void appear_lock();
void not_appear_lock();
pthread_mutex_t mlock[BRAINNUM];

int main(int argc,char**argv) {

    appear_lock();
    //not_appear_lock();
    return 0;
}
void* brain_lock(const int*const para)
{
    printf("I am the %d brain.\n",*para);
    pthread_mutex_lock(&mlock[*para]);
    pthread_mutex_lock(&mlock[((*para)+1)%BRAINNUM]);
        printf("I am %d brain,and I am eating the mutex.\n", *para);
        pthread_mutex_unlock(&mlock[*para]);
        pthread_mutex_unlock(&mlock[((*para) + 1) % BRAINNUM]);
        if (pthread_mutex_destroy(&mlock[*para]) != 0 || pthread_mutex_destroy(&mlock[((*para) + 1)]) != 0) {
            printf("I am %d brain,I can not destroy my chop.Because it is in others hands. \n", *para);
            pthread_exit(NULL);
        }
        printf("I am %d brain,I destroy my food. \n", *para);
    pthread_exit(NULL);
}
void* brain_not_lock(const int*const para)
{

        printf("I am the %d brain.\n",*para);
Tryeat: if(pthread_mutex_trylock(&mlock[*para])==0&&pthread_mutex_trylock(&mlock[((*para)+1)%BRAINNUM])==0) {
        printf("I am %d brain,and I am eating the mutex.\n", *para);
        pthread_mutex_unlock(&mlock[*para]);
        pthread_mutex_unlock(&mlock[((*para)+1)%BRAINNUM]);
        if (pthread_mutex_destroy(&mlock) != 0) {
            printf("I am %d brain,I can not destroy my chop.Because it is in others hands.  \n", *para);
            pthread_exit(NULL);
        }
        printf("I am %d brain,I destroy my chop. \n", *para);
        pthread_exit(NULL);
    }
    else
        printf("I am %d brain.I can not eat it",*para);
        goto Tryeat;
}
void appear_lock()
{
    for(int i=0;i<BRAINNUM;++i)
        pthread_mutex_init(&mlock[i],NULL);
    pthread_t pi[BRAINNUM];
    for(int i=0;i<BRAINNUM;++i)
    {
        pthread_create(&pi[i],NULL,(void*)brain_lock,&i);
    }
    for(int i=0;i<BRAINNUM;++i)
    {
        pthread_join(pi[i],NULL);
    }

}
void not_appear_lock()
{
    for(int i=0;i<BRAINNUM;++i)
        pthread_mutex_init(&mlock[i],NULL);
    pthread_t pi[BRAINNUM];
    for(int i=0;i<BRAINNUM;++i)
    {
        pthread_create(&pi[i],NULL,(void*)brain_not_lock,&i);
    }
    for(int i=0;i<BRAINNUM;++i)
    {
        pthread_join(pi[i],NULL);
    }
}