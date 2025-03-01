## BÀI THỰC HÀNH BUỔI 4

### Exercise 1: Basic Thread Creation and Execution
Write a program using the pthread.h library to perform the following tasks:
- Create two threads. Each thread will print a message along with the thread ID (e.g., "Thread 1: Hello from thread").
- Use the pthread_join function to wait for both threads to complete before the program ends.
- How to use pthread_create and pthread_join? When does a thread end?
- Hint: Use pthread_create to create threads and pthread_join to wait for the threads to complete.

### Expected Output

Thread 1: Hello from thread  
Thread 2: Hello from thread  
Main thread: All threads completed.  

### Key Functions
- `pthread_create()`: Creates a new thread.
- `pthread_join()`: Waits for a thread to finish.

### Exercise 2: Synchronizing Threads with Mutex
Write a program using pthread and mutex to perform the following tasks:
- Create a global variable counter initialized to 0.
- Create three threads, each incrementing the counter by 1, 1,000,000 times.
- Use mutex to ensure that incrementing the counter is done safely.
- Print the final value of the counter when all threads have completed.
- Question: Why is mutex needed in this exercise? What happens if mutex is omitted?
- Hint: Use pthread_mutex_lock and pthread_mutex_unlock to lock and unlock the mutex when accessing the counter.

### Expected Output
Counter: 3000000

### Key Functions
- `pthread_create()`: Creates threads to execute increment_counter.
- `pthread_mutex_lock()` / `pthread_mutex_unlock()`: Ensures only one thread modifies counter at a time.
- `pthread_join()`: Waits for all threads to complete.
- `pthread_mutex_init()` / `pthread_mutex_destroy()`: Initializes and cleans up the mutex.

### Important Notes
- Without `pthread_mutex_lock()`, race conditions may occur, leading to incorrect values.
- Mutex ensures safe access to shared resources in multi-threaded programs.

### Exercise 3: Using Condition Variables
Write a program with two threads: producer and consumer as follows:
- Producer: Generates a random number from 1 to 10 and stores it in a global variable data.
- Consumer: Waits for the producer to provide data, then reads and prints the data.
- Use pthread_cond_wait and pthread_cond_signal to synchronize between producer and consumer, ensuring that the consumer only reads data when the producer has provided it.
- Repeat the above process 10 times and print all values read by the consumer.
- Hint: Use pthread_cond_wait to make the consumer wait until there is a signal from the producer that the data is ready.

### Expected Output

Producer created data: 6, times: 1  
Consumer received data: 6, times: 1  
Producer created data: 9, times: 2  
Consumer received data: 9, times: 2  
Producer created data: 2, times: 3  
Consumer received data: 2, times: 3  
Producer created data: 2, times: 4  
Consumer received data: 2, times: 4  
Producer created data: 7, times: 5  
Consumer received data: 7, times: 5  
Producer created data: 6, times: 6  
Consumer received data: 6, times: 6  
Producer created data: 2, times: 7  
Consumer received data: 2, times: 7  
Producer created data: 5, times: 8  
Consumer received data: 5, times: 8  
Producer created data: 4, times: 9  
Consumer received data: 4, times: 9  
Producer created data: 1, times: 10  
Consumer received data: 1, times: 10  
Done

### Key Functions
- `pthread_mutex_init()`, `pthread_mutex_lock()`, `pthread_mutex_unlock()`, `pthread_mutex_destroy()`: Manage mutex for thread synchronization.
- `pthread_cond_init()`, `pthread_cond_wait()`, `pthread_cond_signal()`, `pthread_cond_destroy()`: Manage condition variables for signaling between threads.
- Use a flag to avoid the consumer waiting forever.

### Exercise 4: Counting Even and Odd Numbers with Threads
Write a program to perform the following steps:
- Create a random array containing 100 integers from 1 to 100.
- Create two threads: one thread counts the even numbers and one thread counts the odd numbers in the array.
- When both threads complete counting, print the total number of even and odd numbers.
- Use pthread_join to ensure that the program only ends when both threads have completed their tasks.
- Hint: Each thread will handle a different part of the work, so mutex is not needed in this exercise.

### Expected Output

Array:
62 42 90 43 72 34 63 69 21 4  
86 60 14 4 55 70 95 57 37 97  
2 55 4 45 92 31 59 71 33 66  
18 94 7 7 89 31 40 3 99 12  
6 36 71 19 39 77 40 86 34 28  
82 87 35 37 31 26 19 90 49 51  
7 66 97 65 24 37 95 15 39 93  
78 97 80 49 67 19 77 7 4 10  
86 37 48 20 74 79 98 44 20 46  
47 26 63 95 90 38 31 85 52 21  

Sum of even numbers: 2198  
Sum of odd numbers: 2804  

### Key Function
- Use `srand(time(NULL))` to initialize randomness and `rand()` to generate random numbers.

### Exercise 5: Sharing Resources with Read-Write Lock
Write a program to simulate a situation where multiple threads read and write data:
- Create a variable data containing an integer.
- Create 5 reader threads and 2 writer threads:
    - Reader threads will only read the data without changing it.
    - Writer threads will increment the value of data by 1.
- Use read-write lock to ensure that multiple threads can read simultaneously, but only one thread can write at any given time.
- Print the value of data after all threads have completed.
- Hint: Use pthread_rwlock_rdlock and pthread_rwlock_wrlock for read-write synchronization.

### Expected Output

Reader 280437626827168: 0  
Reader 280437616341408: 0  
Reader 280437595369888: 0  
Reader 280437605855648: 0  
Writer 280437563912608: 1  
Reader 280437584884128: 1  
Writer 280437574398368: 2  
Done program

### Key Functions
- `pthread_rwlock_rdlock()`: Locks data for reading.
- `pthread_rwlock_wrlock()`: Locks data for writing.
- Use a loop to create threads (e.g., `for()` loop).


### Exercise 6: Summing an Array with Threads and Mutex
Write a program to sum a large array of 1 million integers.
- Divide the array into 4 equal parts.
- Create 4 threads, each calculating the sum of one part of the array.
- Use a global total variable and mutex to aggregate the results from all threads.
- Print the total sum of the array after all threads have completed.
- Hint: Use pthread_mutex_lock to protect the total variable when threads add their results.

### Key Functions
- `pthread_create()`: Creates threads to handle parts of the array.
- `srand(time(NULL))` and `rand()`: Generate a random array.
- `pthread_mutex_lock()` / `pthread_mutex_unlock()`: Lock and unlock the mutex for updating the global total.
- Divide the array into 4 parts, each handled by a separate thread.
- Ensure the array size is up to 1,000,000 elements.
- Create 20 test cases to validate the implementation.

