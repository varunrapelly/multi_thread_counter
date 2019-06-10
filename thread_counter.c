/*
This program demonstrates multi thread counter which shares common mutex between the threads and different condition variables.
This will create 3 threads. 
Thread 1: will print 1, signals count_2 and then it will wait for condition count_1 
Thread 2: will print 2, signals count_3 and then it will wait for condition count_2,
Thread 3: will print 3, signals count_1 and then it will wait for condition count_3.
And this will go on, till the counter values reaches to maximum value.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS  3

volatile int     count = 0;
pthread_barrier_t our_barrier;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_1 = PTHREAD_COND_INITIALIZER,
               count_2 = PTHREAD_COND_INITIALIZER,
               count_3 = PTHREAD_COND_INITIALIZER;
int max_cnt = 1000;
volatile int turn = 0;

void *inc_count1(void *t)
{
        int i;
        long my_id = (long)t;
        pthread_barrier_wait(&our_barrier);
        printf("Barrier completed\n");

        while(count <= max_cnt )
        {
                pthread_mutex_lock(&count_mutex);
                while(turn != 0)
                        pthread_cond_wait(&count_1,&count_mutex);
                if (count == max_cnt)
                {
                        turn = 1;
                        pthread_cond_signal(&count_2);
                        pthread_mutex_unlock(&count_mutex);
                        break;
                }
                count++;
                turn = 1;
                printf("%d ", count);
                pthread_cond_signal(&count_2);
                pthread_mutex_unlock(&count_mutex);
        }
        printf("EXITING 1 NOW\n");
        pthread_exit(NULL);
}

void *inc_count2(void *t)
{
        int i;
        long my_id = (long)t;
        pthread_barrier_wait(&our_barrier);
        printf("Barrier completed\n");
        while(count <= max_cnt)
        {
                pthread_mutex_lock(&count_mutex);
                while(turn != 1)
                        pthread_cond_wait(&count_2,&count_mutex);
                if (count == max_cnt)
                {
                        turn = 2;
                        pthread_cond_signal(&count_3);
                        pthread_mutex_unlock(&count_mutex);
                        break;
                }
                count++;
                turn = 2;
                printf("%d ", count);
                pthread_cond_signal(&count_3);
                pthread_mutex_unlock(&count_mutex);
        }
        printf("EXITING 2 NOW\n");
        pthread_exit(NULL);
}
void *inc_count3(void *t)
{
        int i;
        long my_id = (long)t;
        pthread_barrier_wait(&our_barrier);
        printf("Barrier completed\n");

        while(count <= max_cnt)
        {
                pthread_mutex_lock(&count_mutex);
                while(turn != 2)
                        pthread_cond_wait(&count_3,&count_mutex);
                if (count == max_cnt)
                {
                        turn = 0;
                        pthread_cond_signal(&count_1);
                        pthread_mutex_unlock(&count_mutex);
                        break;
                }
                count++;
                turn = 0;
                printf("%d ", count);
                pthread_cond_signal(&count_1);
                pthread_mutex_unlock(&count_mutex);
        }
        printf("EXITING 3 NOW\n");
        pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
        int i, rc;
        long t1=1, t2=2, t3=3;
        pthread_t threads[3];
        pthread_attr_t attr;
        pthread_barrier_init(&our_barrier,NULL,3);

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&threads[0], &attr, inc_count1, (void *)t1);
        pthread_create(&threads[1], &attr, inc_count2, (void *)t2);
        pthread_create(&threads[2], &attr, inc_count3, (void *)t3);

        for (i = 0; i < NUM_THREADS; i++) {
                pthread_join(threads[i], NULL);
        }
        printf (In main: final value of count = %d\n", count);

        /* Clean up and exit */
        pthread_attr_destroy(&attr);
        pthread_mutex_destroy(&count_mutex);
        pthread_cond_destroy(&count_1);
        pthread_cond_destroy(&count_2);
        pthread_cond_destroy(&count_3);
        pthread_exit (NULL);
}

