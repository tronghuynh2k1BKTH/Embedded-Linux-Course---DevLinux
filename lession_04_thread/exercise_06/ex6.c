#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define ONE_MILLION_INTS 1000000
#define ONE_PART_FOUR (ONE_MILLION_INTS / NUM_THREADS)
#define NUM_THREADS 4

long long sum = 0;
int array[ONE_MILLION_INTS];
pthread_mutex_t mutex;

void create_array_one_million_ints() {
    for (int i = 0; i < ONE_MILLION_INTS; i++) {
        array[i] = rand() % 20;
        printf("%d ", array[i]);
    }
    printf("\n");
}

void *sum_array(void *arg) {
    int value = *(int *)arg;

    pthread_mutex_lock(&mutex);
    sum += value;
    printf("Sum now: %lld\n", sum);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *handle_one_threads(void *arg) {
    int thread_id = *(int *)arg;
    printf("\n\n Index of Thread: %d\n\n", thread_id);

    for (int i = thread_id * ONE_PART_FOUR; i < (thread_id + 1) * ONE_PART_FOUR; i++) {
        printf("Thread %d: %d\n", thread_id, array[i]);
        sum_array((void *)&array[i]);
        printf("Array that thread %d manages: %d\n", thread_id, array[i]);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t thread[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    create_array_one_million_ints();

    for (int index = 0; index < NUM_THREADS; index++) {
        thread_ids[index] = index;
        pthread_create(&thread[index], NULL, handle_one_threads, (void *)&thread_ids[index]);
    }

    for (int index = 0; index < NUM_THREADS; index++) {
        pthread_join(thread[index], NULL);
    }

    printf("Final Sum: %lld\n", sum);
    pthread_mutex_destroy(&mutex);

    printf("\nDone!\n");
    return 0;
}