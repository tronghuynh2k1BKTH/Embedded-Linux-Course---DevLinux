
### Program:

1. Open a file with the `O_APPEND` flag.
2. Use `lseek()` to move the file pointer to the beginning of the file.
3. Write new data to the file.

### Run Command:

```sh
gcc bt1.c -o bt1
./bt1
cat test.txt
```

### Input:

`test.txt`

```
This is content for text.txt, BT1
```

### Output:

`test.txt`

```
This is content for text.txt, BT1HELLO
```

Although the program seeks to the beginning of the file, the data will still be appended to the end of the file. The reason is that the `O_APPEND` flag always ensures that data is written to the end of the file, regardless of the file pointer position.

### Why does `O_APPEND` work this way?

- When a file is opened with `O_APPEND`, the system automatically moves the file pointer to the end of the file before each write (`write()`).
- Therefore, even if `lseek()` changes the file pointer position, `write()` will still write to the end of the file.

