Question: What happens if alarm(1) is not called again in the handler function?

-> The system will hang and remain in the while (1) loop because there is no condition to terminate
the program, unless other signals are sent to the program.