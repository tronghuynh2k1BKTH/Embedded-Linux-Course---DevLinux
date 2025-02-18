Question: What happens if the SIGTSTP signal is not handled and the user presses Ctrl+Z?

-> The program will be suspended.
The SIGTSTP signal is sent to a process when the user presses the Ctrl+Z key combination in the terminal.
By default, when this signal is received, the process will be suspended and can be resumed using the
fg (foreground) or bg (background) command in the terminal.

The program will not be suspended but will instead print a message and continue running.

If you do not have a handler for the SIGTSTP signal, the program will be suspended when the user presses Ctrl+Z.
