In Linux, pressing the Ctrl+C key combination sends the SIGINT (Signal Interrupt)
signal to the running process. This signal requests the process to stop immediately.
This is a quick and effective way to terminate a process from the command line without
having to close the entire terminal.

If a program does not register a custom handler for the SIGINT signal
(e.g., does not have a specific function to handle this signal), the operating system
will use the default action for that signal. For SIGINT, the default action is to
terminate the current process.

-----
Question: What happens if the SIGINT signal is ignored?

To ignore the SIGINT signal, you can register to ignore the signal as follows:
signal(SIGINT, SIG_IGN);

In this case, when Ctrl+C is pressed, the program will not be terminated and will continue running.
