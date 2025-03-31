#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int share_data = 0;
pthread_rwlock_t rwlock;

void *reader(void *arg)
{
    pthread_rwlock_rdlock(&rwlock);
    printf("Reader %ld: %d\n", pthread_self(), share_data);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

void *writer(void *arg)
{
    pthread_rwlock_wrlock(&rwlock);
    share_data++;
    printf("Writer %ld: %d\n", pthread_self(), share_data);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

int main()
{
    // create 5 reader threads and 2 writer threads
    pthread_t thread_reader[5], thread_writer[2];

    // init rw lock
    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < 5; i++){
        pthread_create(&thread_reader[i], NULL, reader, NULL);
    }
    for (int i = 0; i < 2; i++){
        pthread_create(&thread_writer[i], NULL, writer, NULL);
    }

    // wait for all threads to finish
    for (int i = 0; i < 5; i++){
        pthread_join(thread_reader[i], NULL);
    }
    for (int i = 0; i < 2; i++){
        pthread_join(thread_writer[i], NULL);
    }

    // destroy rw lock
    pthread_rwlock_destroy(&rwlock);

    printf("Done program \n");
    return 0;
}