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

### Exercise 4: Counting Even and Odd Numbers with Threads
Write a program to perform the following steps:
- Create a random array containing 100 integers from 1 to 100.
- Create two threads: one thread counts the even numbers and one thread counts the odd numbers in the array.
- When both threads complete counting, print the total number of even and odd numbers.
- Use pthread_join to ensure that the program only ends when both threads have completed their tasks.
- Hint: Each thread will handle a different part of the work, so mutex is not needed in this exercise.

### Exercise 5: Sharing Resources with Read-Write Lock
Write a program to simulate a situation where multiple threads read and write data:
- Create a variable data containing an integer.
- Create 5 reader threads and 2 writer threads:
    - Reader threads will only read the data without changing it.
    - Writer threads will increment the value of data by 1.
- Use read-write lock to ensure that multiple threads can read simultaneously, but only one thread can write at any given time.
- Print the value of data after all threads have completed.
- Hint: Use pthread_rwlock_rdlock and pthread_rwlock_wrlock for read-write synchronization.

### Exercise 6: Summing an Array with Threads and Mutex
Write a program to sum a large array of 1 million integers.
- Divide the array into 4 equal parts.
- Create 4 threads, each calculating the sum of one part of the array.
- Use a global total variable and mutex to aggregate the results from all threads.
- Print the total sum of the array after all threads have completed.
- Hint: Use pthread_mutex_lock to protect the total variable when threads add their results.


BT3. Sử Dụng Condition Variables
Viết một chương trình có hai threads: producer và consumer như sau:

Producer: Tạo ra một số ngẫu nhiên từ 1 đến 10 và lưu vào một biến toàn cục data.
Consumer: Đợi producer cung cấp dữ liệu, sau đó đọc và in ra dữ liệu đó.

Sử dụng pthread_cond_wait và pthread_cond_signal để đồng bộ hóa giữa producer và consumer, đảm bảo rằng consumer chỉ đọc dữ liệu khi producer đã cung cấp xong.

Lặp lại quá trình trên 10 lần và in ra tất cả các giá trị do consumer đọc được.
Gợi ý: Sử dụng pthread_cond_wait để cho consumer đợi cho đến khi có tín hiệu từ producer rằng dữ liệu đã sẵn sàng.

BT4. Đếm Số Chẵn và Số Lẻ với Threads
Viết một chương trình để thực hiện các bước sau:
Tạo một mảng ngẫu nhiên chứa 100 số nguyên từ 1 đến 100.
Tạo hai threads: một thread đếm số chẵn và một thread đếm số lẻ trong mảng.
Khi hai threads hoàn tất việc đếm, in ra tổng số số chẵn và tổng số số lẻ.
Sử dụng pthread_join để đảm bảo rằng chương trình chỉ kết thúc khi cả hai threads hoàn thành nhiệm vụ.
Gợi ý: Mỗi thread sẽ xử lý một phần công việc khác nhau, vì vậy không cần mutex trong bài này.

BT5. Chia Sẻ Tài Nguyên Với Read-Write Lock
Viết một chương trình mô phỏng tình huống có nhiều thread đọc và ghi dữ liệu:
Tạo một biến data chứa một số nguyên.
Tạo 5 threads đọc và 2 threads ghi:
Các thread đọc sẽ chỉ đọc data mà không thay đổi.
Các thread ghi sẽ tăng giá trị của data lên 1.
Sử dụng read-write lock để đảm bảo nhiều threads có thể đọc cùng lúc, nhưng chỉ một thread được ghi tại bất kỳ thời điểm nào.
In ra giá trị của data sau khi tất cả các threads hoàn thành.
Gợi ý: Sử dụng pthread_rwlock_rdlock và pthread_rwlock_wrlock cho đọc-ghi đồng bộ.

BT6. Tính Tổng Mảng với Threads và Mutex
Viết một chương trình tính tổng một mảng lớn gồm 1 triệu số nguyên.
Chia mảng thành 4 phần bằng nhau.
Tạo 4 threads, mỗi thread tính tổng một phần của mảng.
Sử dụng một biến tổng toàn cục và mutex để tổng hợp kết quả từ tất cả các threads.
In ra kết quả tổng của mảng sau khi các threads hoàn thành.
Gợi ý: Sử dụng pthread_mutex_lock để bảo vệ biến tổng khi các threads cộng kết quả của chúng.

