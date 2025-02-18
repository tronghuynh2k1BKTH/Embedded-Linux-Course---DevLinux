## Exercises

### BT1
Write a program that opens any file and uses the `O_APPEND` flag, then seeks to the beginning of the file and writes some data to it. Where will the data appear in the file and why?

### BT2
After each write command below, specify which file the content will be written to, what the content is, and explain why:
```c
fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6);
write(fd2, "world", 6);
lseek(fd2, 0, SEEK_SET);
write(fd1, "HELLO,", 6);
write(fd3, "Gidday", 6);
```

### BT3
Write a program with 3 command-line arguments in the following format:
```sh
$ ./example_program filename num-bytes [r/w] "Hello"
```
Where:
1. `example_program`: The name of the executable file
2. `filename`: The name of the file
3. `num-bytes`: The number of bytes to read/write
4. `[r/w]`: 
	- `r` -> Read from the filename and print to the screen
	- `w` -> Write to the filename
5. `"Hello"`: Any content to read/write to the filename

### BT4
Create any file and write some data to it. Print the following information to the screen:
- File type
- File name
- Last modification time
- Size

Use `struct stat`.