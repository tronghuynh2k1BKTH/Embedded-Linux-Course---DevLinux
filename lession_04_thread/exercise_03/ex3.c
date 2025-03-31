#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int global_data = 0;
int ready_data = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *producer(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        global_data = rand() % 10 + 1;
        printf("Producer created data: %d, times: %d\n", global_data, i+1);
        ready_data = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1); // assume that consumer is slower than producer 1 second
    }
    return NULL;
}

void *consumer(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        while (ready_data == 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Consumer received data: %d, times: %d\n", global_data, i+1);
        ready_data = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;

}

int main()
{
    srand(time(NULL));
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    
    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Done\n");
    return 0;
}