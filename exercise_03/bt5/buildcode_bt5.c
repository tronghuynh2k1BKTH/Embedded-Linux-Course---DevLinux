#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid_child = fork();

    if (pid_child < 0)
    {
        // Fork failed
        perror("fork");
        exit(1);
    }
    else if (pid_child == 0)
    {
        // Child process
        printf("Child process (pid_child: %d) is running and will exit now.\n", getpid());
        exit(0);
    }
    else
    {
        // Parent process
        printf("Parent process (pid_child: %d) created a child process (pid_child: %d).\n", getpid(), pid_child);
        printf("Child process is now a zombie. Press any key to exit parent process.\n\n\n");
        getchar(); // Wait for user input to keep the parent process running
    }
    // Create an orphan process
    pid_t pid_orphan = fork();

    if (pid_orphan < 0)
    {
        // Fork failed
        perror("fork");
        exit(1);
    }
    else if (pid_orphan == 0)
    {
        // Orphan process
        printf("Orphan process (pid_orphan: %d) is running and will sleep for 10 seconds.\n", getpid());
        sleep(20);
        printf("Orphan process (pid_orphan: %d) has finished sleeping and will exit now.\n", getpid());
        while (1)
            ; // Infinite loop to keep the orphan process running
    }
    else
    {
        // Parent process
        printf("Parent process (pid_child: %d) created an orphan process (pid_orphan: %d).\n", getpid(), pid_orphan);
    }
    return 0;
}

// check zombie process
// gcc -o buildcode_bt5 buildcode_bt5.c
// ./buildcode_bt5
// // Output:
// // Parent process (pid_child: 12345) created a child process (pid_child: 12346).
// // Child process (pid_child: 12346) is running and will exit now.
// // Child process is now a zombie. Press any key to exit parent process.
// //
// check zombie process
// ps aux | grep Z
// // Output:
// USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
// huynh      57836  0.0  0.0      0     0 pts/1    Z+   09:24   0:00 [buildcode_bt5] <defunct>
// huynh      57838  0.0  0.0   6140  2048 pts/0    S+   09:24   0:00 grep --color=auto Z

// check orphan process
// gcc -o buildcode_bt5 buildcode_bt5.c
// ./buildcode_bt5
// // Output:
// Parent process (pid_child: 58162) created an orphan process (pid_orphan: 58167).
// Orphan process (pid_orphan: 58167) is running and will sleep for 10 seconds.
// Orphan process (pid_orphan: 58167) has finished sleeping and will exit now.
// check on terminal
// ps -o pid,ppid 58167
// Output:
//   PID  PPID
// 58167     1
