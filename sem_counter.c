#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_COUNT 10
sem_t sem;
pthread_barrier_t barrier;
volatile int count = 0;
volatile int flag = 1;
void* count_number(void* arg)
{
    int thr_id = *((int*)arg);
    printf("thread ID : %d waiting on barrier \n",thr_id);
    pthread_barrier_wait(&barrier);
    printf("Barrier completed=== %d\n", thr_id);
    while(count < MAX_COUNT)
    {
        while (flag != thr_id) {
            //printf("\n Thread %d waiting for signal ...\n",thr_id);
        }
        printf("COUNT : %d AND THREAD ID : %d\n", count,thr_id);
        if (flag == thr_id)
        {
            sem_wait(&sem);
            count++;
            if (flag == 2) flag = 1;
            else if (flag == 1) flag = 2;
        }

        //signal
        //printf("\nJust Exiting...\n");
        sem_post(&sem);
    }
}


int main()
{
    sem_init(&sem, 0, 1);
    pthread_t t1,t2;
    int arg1=1, arg2 =2;
    pthread_barrier_init(&barrier,NULL,2);
    pthread_create(&t1,NULL,count_number,(void *)&arg1);
    pthread_create(&t2,NULL,count_number,(void *)&arg2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&sem);
    return 0;
}
