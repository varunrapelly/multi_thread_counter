/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : count_numbers.c

* Purpose :

* Creation Date : 22-06-2019

* Last Modified : Sat Jun 22 22:19:20 2019

* Created By :  
COMPILE USing: TODO
g++ count_numbers.c -std=c++0x -lpthread -g


_._._._._._._._._._._._._._._._._._._._._.*/

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#define MAX 10000
volatile int     count = 0;
pthread_barrier_t our_barrier;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[MAX];// = {PTHREAD_COND_INITIALIZER};
volatile int turn = 0;
int num;
int max_cnt;

void *count_number(void *t)
{
        int i;
        int my_id = *((int*)t);
        pthread_barrier_wait(&our_barrier);
        printf("Barrier completed=== %d\n", my_id);

        while(count <= max_cnt )
        {
        	printf("%s GOT LOCK\n", __FUNCTION__);
                pthread_mutex_lock(&count_mutex);
                while(turn != my_id)
                        pthread_cond_wait(&cond[my_id],&count_mutex);
                if (count == max_cnt)
                {
			if (turn < num-1)
			{
                        	turn++;
			}
			else
				turn = 0;
                        pthread_cond_signal(&cond[turn]);
                        pthread_mutex_unlock(&count_mutex);
                        break;
                }
                count++;
                printf("%d ", count);
		if (turn < num-1)
		{
                	turn++;
		}
		else
			turn = 0;
                pthread_cond_signal(&cond[turn]);
                pthread_mutex_unlock(&count_mutex);
        }
        printf("EXITING %d NOW\n", my_id);
        pthread_exit(NULL);
}
int main()
{
        int i, rc;
        int tid[MAX];
        pthread_t threads[MAX];
	printf("Enter number of threads\n");
	scanf("%d", &num);
	printf("Enter max count value to count\n");
	scanf("%d", &max_cnt);
        pthread_attr_t attr;
        pthread_barrier_init(&our_barrier,NULL,num);
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (i =0; i < num; i++)
	{
		cond[i] = PTHREAD_COND_INITIALIZER;
	}
	for (i =0; i < num; i++)
	{
		tid[i] = i;
        	pthread_create(&threads[i], &attr, count_number, (void *)&(tid[i]));
	}
        for (i = 0; i < num; i++) {
                pthread_join(threads[i], NULL);
        }
        printf ("In main: final value of count = %d\n", count);
        pthread_attr_destroy(&attr);
        pthread_mutex_destroy(&count_mutex);
        pthread_exit (NULL);
	return 0;
}
