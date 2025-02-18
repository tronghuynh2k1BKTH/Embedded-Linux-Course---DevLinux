PRACTICAL EXERCISE SESSION 3

----------------------------------------------------------
Exercise 1: Process Creation and Monitoring

Description: Write a C program that creates a child process using fork().
After creating the process, the child process will print its PID and the parent process will print the PID of the child process.
----------------------------------------------------------

Exercise 2: Using exec and Environment Variables

Description: Write a program where the child process will replace the running program with another program, such as ls or date, using the execlp() or execvp() function.

Requirements:
- Pass environment variables to the program and perform different tasks (e.g., if the environment variable passed is 1, execute the ls command; if it is 2, execute the date command).
- Explain what happens to the original process after exec is called.
----------------------------------------------------------

Exercise 3: Process Signals

Description: Write a program that creates a parent and child process. The parent process will send the SIGUSR1 signal to the child process after a certain period of time.

Requirements:
- Use signal() to define the behavior of the child process when it receives the SIGUSR1 signal.
- Print a message when the child process receives the signal.
----------------------------------------------------------

Exercise 4: Recording Process Status

Description: Write a program that creates a child process, then the parent process uses wait() to wait for the child process to finish and record its exit status.

Requirements:
- In the child process, choose the exit status by calling exit() with different values.
- Use WIFEXITED() and WEXITSTATUS() to confirm and record the exit code of the child process.
----------------------------------------------------------

Exercise 5: Creating Zombie and Orphan Processes

Description: Write a program that creates a zombie process and an orphan process to understand these special types of processes.

Requirements:
- To create a zombie, let the parent process terminate early while the child process is still running, and use ps to observe the zombie state of the process.
- To create an orphan, let the child process sleep for a longer period than the parent process, so the child process becomes an orphan.
- Explain the zombie and orphan states, and why they appear in Linux.
----------------------------------------------------------
