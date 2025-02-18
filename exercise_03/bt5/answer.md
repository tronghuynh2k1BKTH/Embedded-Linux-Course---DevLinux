Zombie Process
Definition: A zombie process is a process that has completed execution (terminated), but its information still exists in the process table of the operating system.

Reason for occurrence: When a child process terminates, it sends a signal (SIGCHLD) to the parent process to notify about the termination. The parent process has to "collect" the information about the terminated child process to clean up. This process is called "wait".

If the parent process does not perform this "wait", the terminated child process will fall into a zombie state. It still exists in the process table but no longer performs any tasks.

-----

Orphan Process

Definition: An orphan process is a process whose parent process has terminated before it has finished.

Reason for occurrence: When a parent process terminates, all its child processes are automatically "adopted" by the init process (PID 1). init is a special process that always runs first when the system boots. It is responsible for "cleaning up" orphan processes to ensure they do not become zombies.

Characteristics: An orphan process continues to run normally until it completes its task or is terminated. After it terminates, init collects its information and removes it from the process table.
