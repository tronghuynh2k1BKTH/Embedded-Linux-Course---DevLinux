#include <stdio.h>
#include <pthread.h>

long unsigned int counter = 0;
pthread_mutex_t mutex;

void *increment_counter(void *arg)
{
    for (int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t thread_id_1, thread_id_2, thread_id_3;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&thread_id_1, NULL, increment_counter, NULL);
    pthread_create(&thread_id_2, NULL, increment_counter, NULL);
    pthread_create(&thread_id_3, NULL, increment_counter, NULL);

    pthread_join(thread_id_1, NULL);
    pthread_join(thread_id_2, NULL);
    pthread_join(thread_id_3, NULL);

    printf("Counter: %lu\n", counter);
    pthread_mutex_destroy(&mutex);

    return 0;
}