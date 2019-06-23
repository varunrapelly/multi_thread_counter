# multi_thread_counter
This program demonstrates multi thread counter, which shares common mutex between the threads and different condition variables.
This application will create 3 threads. 
Thread 1: will print 1, signals count_2 and then it will wait for condition count_1 
Thread 2: will print 2, signals count_3 and then it will wait for condition count_2,
Thread 3: will print 3, signals count_1 and then it will wait for condition count_3.
And this will go on, till the counter values reaches to maximum value.

gcc thread_counter.c -lpthread

count_numbers.c
This application asks your to enter the number (N) of threads (maximum value supported 1000) and the number to count (C).
Based on the input, it will create N threads and counts upto C value.

g++ count_numbers.c -std=c++0x -lpthread -g
