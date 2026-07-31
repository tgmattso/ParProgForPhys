
This directory contains solutions for a set of MPI exercises.
They are all built from a single makefile.  

     make         ..... to build all the programs
     make clean   ..... to remove object files and executables
     make test    ..... to build and test each program

The programs in this directory include:

A simple "hello world" exercise:
   hello_mpi.c

A "pi" program using a "Bulk Synchronis Style" of the SPMD pattern
   pi_bsp.c

A ping pong program to time the passing of messages between a pair of
processes:
   ping_pong.c
   random.c
   time_stats.c
   wt_overhead.c

Several versions of a program to measure bandwidth on a network by 
passing messages around a ring:
   ring.c
   ring.h
   ring_naive.c
   ring_persistent.c
   ring_sndrcv.c
   ring_split.c

A program to tranpose a matrix stored with a column block disstribution
   trans_isnd_ircv.c
   trans_sendrcv.c
   trans_snd_rcv.c
   trans_utility.c
   trans_utility.h
   transpose.c
