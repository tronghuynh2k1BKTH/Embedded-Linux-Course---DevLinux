PRACTICE SESSION 5

Exercise 1: Catch SIGINT Signal
Requirement: Write a program to catch the SIGINT signal (sent when pressing Ctrl+C) and print a message when this signal is received.
Use the signal() function to register a handler for the SIGINT signal.
In the handler function, print the message "SIGINT received" each time the SIGINT signal is received.
The program will continue to run until it receives the SIGINT signal for the third time, then it will terminate.
Hint: Use a global counter variable to count the number of times the SIGINT signal is received and check this variable in the handler function.
Question: What happens if the SIGINT signal is ignored when pressing Ctrl+C?

Exercise 2: Use SIGALRM Signal to Create Timer
Requirement: Write a program to create a timer using the SIGALRM signal.
Use the alarm() function to set the SIGALRM signal to trigger every second.
When the SIGALRM signal is received, the program will increment a counter and print the line "Timer: <seconds> seconds".
Stop the program after counting to 10 seconds.
Hint: Use the signal() function to register a handler for the SIGALRM signal, and set alarm(1) to repeat the timer every second.
Question: What happens if alarm(1) is not called again in the handler function?

Exercise 3: Create Inter-Process Communication using Signals
Requirement: Write a program to create two processes (parent and child) and use the SIGUSR1 signal to communicate between them.
Implementation:
Use fork() to create a child process from the parent process.
The parent process will send the SIGUSR1 signal to the child process every 2 seconds.
When the child process receives the SIGUSR1 signal, it will print the message "Received signal from parent".
Stop the program after sending the signal 5 times.
Hint: Use kill() to send the signal from the parent process to the child process, and use signal() in the child process to catch the SIGUSR1 signal.
Question: Why do we need to use kill() to send the signal in this exercise?

Exercise 4: Write a program that can ignore the SIGTSTP signal (usually Ctrl+Z).
Use signal() or sigaction() to catch and ignore the SIGTSTP signal.
When the SIGTSTP signal is received, the program will not stop but continue to run and print the message "SIGTSTP ignored".
Hint: Ensure the program continues to run instead of being stopped when pressing Ctrl+Z.
Question: What happens if the SIGTSTP signal is not handled and the user presses Ctrl+Z?

Exercise 5: Program to wait for signals while waiting for user input
Objective: Write a program that can receive signals while allowing the user to input data from the keyboard.

Instructions:

Use select() or poll() to allow the program to handle signals without interrupting when receiving keyboard input.

When receiving the SIGINT signal, print the message “SIGINT received.”

If the SIGTERM signal is received, exit the program.

Hint: Combine signal handling and reading input to make the program more flexible.
