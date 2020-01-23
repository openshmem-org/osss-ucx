# hello.c

This is a trivial example program that just shows how you might
compile and run an OpenSHMEM program.

N.B. different systems may have different commands for compiling and
launching programs especially through job schedulers, so please check
locally.

## Compiling

```shell
    host$ oshcc hello.c
``` 

## Running

```shell
    host$ oshrun -n 4 ./a.out
    host: Hello from PE    2 of    4
    host: Hello from PE    0 of    4
    host: Hello from PE    1 of    4
    host: Hello from PE    3 of    4
```
