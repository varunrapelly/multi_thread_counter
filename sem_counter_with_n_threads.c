#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_COUNT 1000
sem_t sem;
pthread_barrier_t barrier;
volatile int count = 1;
volatile int flag = 0;
uint32_t num_threads = 0;
#define MAX_THREAD 1024

void* count_number(void* arg)
{
    int thr_id = *((int*)arg);
    printf("thread ID : %d waiting on barrier \n",thr_id);
    pthread_barrier_wait(&barrier);
    printf("Barrier completed=== %d\n", thr_id);
    while(count < MAX_COUNT)
    {
        while (flag != thr_id) {        }
        printf("COUNT : %d AND THREAD ID : %d\n", count,thr_id);
        if (flag == thr_id)
        {
            sem_wait(&sem);
            if (count == MAX_COUNT)
            {
                sem_post(&sem);
                if (flag == (num_threads -1)) flag = 0;
                else flag++;
                break;
            }
            count++;
            if (flag == (num_threads -1)) flag = 0;
            else flag++;
        }

        sem_post(&sem);
    }
}


int main()
{
    sem_init(&sem, 0, 1);
    pthread_t t1[MAX_THREAD];
    int tid[MAX_THREAD];
    do
    {
        printf("Enter number of threads\n");
        scanf("%d", &num_threads);
        if (num_threads > MAX_THREAD)
        {
            printf("Max threads supported is %d\n", MAX_THREAD);
        }
    } while (num_threads > MAX_THREAD);
    pthread_barrier_init(&barrier,NULL,num_threads);
    int i;
    for (i=0; i< num_threads; i++)
    {
        tid[i] = i;
        pthread_create(&t1[i],NULL,count_number,(void *)(&tid[i]));
    }
    for (i=0; i< num_threads; i++)
    {
        pthread_join(t1[i],NULL);
    }
    sem_destroy(&sem);
    return 0;
}
