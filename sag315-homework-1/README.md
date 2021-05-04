# Summation

## Information

    My program gets the start and end values, which can be given as command lines arguements or if not given defaults to start = 1 & end = 100. Then a pipe is created which will be used for communication between the parent and child process.The child computes the first half of the summation and the parent computes the second half of the summation. The parent waits for the child to finish executing before it executes.

## Running

Summation 1 - 100
```bash
    ./sum
```
Summation start - end
```bash
    ./sum <start> <end>
```