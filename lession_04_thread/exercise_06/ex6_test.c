#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define ONE_MILLION_INTS 1000000
#define ONE_PART_FOUR (ONE_MILLION_INTS / NUM_THREADS)
#define NUM_THREADS 4
#define NUM_TESTS 20

long long sum = 0;
int array[ONE_MILLION_INTS];
pthread_mutex_t mutex;

void create_array_one_million_ints() {
    for (int i = 0; i < ONE_MILLION_INTS; i++) {
        array[i] = rand() % 20;
    }
}

long long calculate_total_sum() {
    long long total_sum = 0;
    for (int i = 0; i < ONE_MILLION_INTS; i++) {
        total_sum += array[i];
    }
    return total_sum;
}

void *sum_array(void *arg) {
    int value = *(int *)arg;

    pthread_mutex_lock(&mutex);
    sum += value;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *handle_one_threads(void *arg) {
    int thread_id = *(int *)arg;

    for (int i = thread_id * ONE_PART_FOUR; i < (thread_id + 1) * ONE_PART_FOUR; i++) {
        sum_array((void *)&array[i]);
    }
    return NULL;
}

void run_test(int test_number) {
    pthread_t thread[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    sum = 0;
    create_array_one_million_ints();

    for (int index = 0; index < NUM_THREADS; index++) {
        thread_ids[index] = index;
        pthread_create(&thread[index], NULL, handle_one_threads, (void *)&thread_ids[index]);
    }

    for (int index = 0; index < NUM_THREADS; index++) {
        pthread_join(thread[index], NULL);
    }

    long long expected_sum = calculate_total_sum();
    printf("Test %d - Expected Sum: %lld, Thread Sum: %lld -> %s\n", 
           test_number, expected_sum, sum, (expected_sum == sum) ? "PASS" : "FAIL");
}

int main() {
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);

    for (int i = 1; i <= NUM_TESTS; i++) {
        run_test(i);
    }

    pthread_mutex_destroy(&mutex);
    printf("\nAll tests completed!\n");
    return 0;
}
