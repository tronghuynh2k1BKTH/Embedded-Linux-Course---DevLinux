# Exercises

## EXERCISE 01

1. Create a pipe: `pipe(pipefd)`
2. Fork a child process: `pid = fork()`
3. Parent process:
    - Close the read end: `close(pipefd[0])`
    - Write data to the pipe: `write(pipefd[1], message, strlen(message) + 1)`
    - Close the write end: `close(pipefd[1])`
4. Child process:
    - Close the write end: `close(pipefd[1])`
    - Read data from the pipe: `read(pipefd[0], buffer, BUFFER_SIZE)`
    - Print to the screen: `printf()`
    - Close the read end: `close(pipefd[0])`

## EXERCISE 02

1. Create two pipes: `pipe1` and `pipe2`
2. Fork child process 1:
    - Receive data from the parent through `pipe1`
    - Modify the content (add " - Modified by Child 1")
    - Send data to child 2 through `pipe2`
3. Fork child process 2:
    - Receive data from child 1 through `pipe2`
    - Print the result to the screen
4. Parent process:
    - Send data to child 1 through `pipe1`
    - Wait for both child processes to complete: `wait(NULL)`

## EXERCISE 03

1. Create a pipe: `pipefd[2]`
2. Fork a child process
3. Parent process:
    - Send a string to the pipe
    - Close the pipe after sending
    - Wait for the child process to handle it
4. Child process:
    - Read the string from the pipe
    - Count the number of characters using `strlen()`
    - Print the result to the screen