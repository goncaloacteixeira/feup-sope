# Point C

- When we input `^C` we kill the program and its processes;
- When we input `kill -SIGINT <PID>` on the terminal, it only sends the SIGINT signal to the parent-process and the child processes keep running.
