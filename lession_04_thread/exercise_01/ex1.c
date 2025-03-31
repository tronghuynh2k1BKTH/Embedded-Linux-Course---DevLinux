#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *hello_thread(void *arg)
{
    int num_thread = *((int *)arg);
    printf("Thread %d: Hello from thread\n", num_thread);
    return NULL;
}

int main()
{
    pthread_t thread_id_1, thread_id_2;
    int id1 = 1, id2 = 2;
    pthread_create(&thread_id_1, NULL, hello_thread, &id1);
    pthread_create(&thread_id_2, NULL, hello_thread, &id2);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);

    printf("Main thread: All threads completed.\n");
    return 0;
}