#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_COUNT 10
sem_t sem;
sem_t barrier;
volatile int count = 0;
volatile int flag = 1;
void* count_number(void* arg)
{
    int thr_id = *((int*)arg);
    printf("thread ID : %d waiting on barrier \n",thr_id);
    sem_wait(&barrier);
    printf("Barrier completed=== %d\n", thr_id);
    sem_post(&barrier);
    while(count < MAX_COUNT)
    {
        while (flag != thr_id) {  }
        printf("COUNT : %d AND THREAD ID : %d\n", count,thr_id);
        if (flag == thr_id)
        {
            sem_wait(&sem);
            count++;
            if (flag == 2) flag = 1;
            else if (flag == 1) flag = 2;
        }

        sem_post(&sem);
    }
}


int main()
{
    sem_init(&sem, 0, 1);
    sem_init(&barrier, 0, 2);
    pthread_t t1,t2;
    int arg1=1, arg2 =2;
    sem_wait(&barrier);
    pthread_create(&t1,NULL,count_number,(void *)&arg1);
    pthread_create(&t2,NULL,count_number,(void *)&arg2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_post(&barrier);
    sem_destroy(&sem);
    sem_destroy(&barrier);
    return 0;
}
