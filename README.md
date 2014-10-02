CS 537 - Program 2a (Fall 2014)

Partner 1: Corbin Schwalm <schwalm@wisc.edu> | schwalm
Partner 2: Zachary York <zyork@wisc.edu> | zyork

We wrote a C program to parse CLI in a shell. The CLI was then inspected and it was determined if a system call should be made. If not, a new process was cloned and the exec command wall called on the created child process.
We also checked if the input should be sent to stdout or to a file (either overwriting with > or appending with >>). If an instruction included a pipe (|), we piped the output of the first program into the second program.

This project gave us a better understanding of both how a shell works and creating children processes. This project also deepened our understanding of the dup function works and which file descriptors we had to close.
