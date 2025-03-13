#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int sum_odd = 0;
int sum_even = 0;
int array[100];

void *thread_read_odd(void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        if (array[i] % 2 == 0)
        {
            sum_even += array[i];
        }
    }
    printf("Sum of even number: %d\n", sum_even);
    return NULL;
}

void *thread_read_even(void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        if (array[i] % 2 != 0)
        {
            sum_odd += array[i];
        }
    }
    printf("Sum of odd number: %d\n", sum_odd);
    return NULL;
}

void print_array()
{
    printf("Array:");
    for (int i = 0; i < 100; i++)
    {
        if (i % 10 == 0)
        {
            printf("\n");
        }
        array[i] = random() % 100 + 1;
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    // create and print array
    srand(time(NULL));
    print_array();

    // create and handle thread (odd & even)
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread_read_odd, NULL);
    pthread_create(&tid2, NULL, thread_read_even, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}