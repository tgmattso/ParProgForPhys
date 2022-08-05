/*******************************************************************

NAME:    ping_pong

PURPOSE: This MPI program measures communication times for
         bouncing a message between two processes.  This is 
         repeated for a number of iterations and timing
         statistics are compiled.
  
USAGE:   Program input is the length of the message to bounce 
         between the two processes and the number of iterations 
         to time.  Input is taken from  the command line:

               ping_pong buffer_count iterations

         where buffer_count is the number of longs to use in the
         message.
  
         The output consists of timing statistics.  See time_stats
         for information about what statistics are reported.  

         Compile with DEBUG defined if you want lots of output.

HISTORY: Written by Tim Mattson, April 1999.  This is based on the
         p4 ping_pong program I wrote long ago at Yale.
  
*******************************************************************/
#define DEBUG 1
#include "mpi.h"
#include <stdio.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <memory.h>

#define False   0L 
#define True    1L 

/*  Random number generation (see random.c) */
#define SEED    5      /* Common Seed  for uniform random sequences */
#define low_value  1   /* lower bound for pseudo random sequence */
#define hi_value  100  /* Higer bound for pseudo random sequence */
void s_l_random(long, long, long);
long l_random();


int MTag_1 = 1;       /* message Tag for MPI */
int MTag_2 = 2;       /* message Tag for MPI */
int MTag_3 = 3;       /* message Tag for MPI */
int MTag_4 = 4;       /* message Tag for MPI */
int MTag_5 = 5;       /* message Tag for MPI */

int main(int argc, char **argv)
{
  int buffer_count;    /* length of message to bounce  */
  int buff_size_bytes; /* length of message to bounce in bytes  */
  int iterations;      /* Number of iterations to time */
  long *buffer;         /* buffer to bounce between processes */
  long *incoming;       /* buffer to bounce between processes */
  double *timings,*tp;  /* arrays of times for each iteration */
  double wt_overhead(); /* MPI_Wtime correction factor in Seconds */
  double t0, twtime;    /* initial time and timer call time */
  int my_ID;            /* process id (or in MPI-speak, the rank) */
  int numP;             /* number of processes */
  MPI_Status stat;    /* MPI status parameter */
  int iters, i;
  int cpy_count;

#ifdef DEBUG
  long *result, *res; /* Hold original buffer for testing */
  long diff;          /* holds sum of differences for error testing */
  double check_sum;   /* check sum of random-filled buffer */
#endif

/*********************************************************************
** Initialize the MPI environment
*********************************************************************/
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_ID);
  MPI_Comm_size(MPI_COMM_WORLD, &numP);
  if(numP != 2){
    if(my_ID == 0){
       printf(" Ping Pong only runs with 2 process\n");
       printf(" Wrong number of processes (%d) ... abort \n",numP);
    }
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

#ifdef DEBUG
  printf("\nMPI Ping Pong Test \n");
  printf("\n after MPI_Init and MPI_Comm_rank\n");
#endif
/*********************************************************************
** pass input parameters on to slave
*********************************************************************/
  if(my_ID == 0){
      if (argc == 3){
         buffer_count = atoi(*++argv);
         iterations   = atoi(*++argv);
      }
      else{
        printf("Usage: %s <message_size in longs> <iterations> \n",*argv);
        printf(" program will proceed with default values\n");
        buffer_count =  50;
        iterations   =  20;
      }

#ifdef DEBUG
      printf("\n MPI ping/pong with %d longs",buffer_count);
      printf(" and %d iterations \n",iterations);
#endif
      buff_size_bytes = buffer_count * sizeof(long);

    MPI_Send (&buffer_count, 1, MPI_INT, 1, MTag_1, MPI_COMM_WORLD);
    MPI_Send (&iterations,   1, MPI_INT, 1, MTag_2, MPI_COMM_WORLD);
  }
  else if (my_ID==1){
    MPI_Recv (&buffer_count, 1, MPI_INT, 0, MTag_1, MPI_COMM_WORLD, &stat);
    MPI_Recv (&iterations,   1, MPI_INT, 0, MTag_2, MPI_COMM_WORLD, &stat);
#ifdef DEBUG
      printf("\n ID %d ping/pong with %d longs",my_ID,buffer_count);
      printf(" and %d iterations \n",iterations);
#endif
  }

  if (my_ID == 0){
  /*****************************************************************
  ** Time the timer and allocate memory used by node 0 only .
  *****************************************************************/

      twtime = wt_overhead(iterations);
//      twtime = 0.0;
#ifdef DEBUG
      printf("\n clock correction factor is time is %f\n",twtime);
#endif
      if ( (incoming= (long *)malloc(buffer_count* sizeof(long))) == False){
         printf(" Could not allocate buffer of %d longs\n",buffer_count);
         exit (1);
	  }
      if((tp=timings=(double *)malloc((unsigned)(iterations*sizeof(double))))==False){
         printf(" Could not allocate space for times array \n");
         exit (1);
	  }
 #ifdef DEBUG
      printf("\nDEBUG is on - communications will be tested\n");
      if ( (result = (long *)malloc(buff_size_bytes)) == False){
         printf("Could not allocate result buff of %d longs\n",buffer_count);
         exit (1);
	  }
 #endif
  }

/******************************************************************
** Allocate memory used on both nodes
******************************************************************/
  if ( (buffer = (long *)malloc(buffer_count * sizeof(long))) == False){
      printf(" Could not allocate buffer of %d longs\n",buffer_count);
      exit (1);
  }

/*********************************************************************
** Fill buffer up with junk.
*********************************************************************/
  if(my_ID == 0){
     s_l_random( SEED, low_value, hi_value );
     for (i=0; i<buffer_count; i++) buffer[i] = l_random();
#ifdef DEBUG
     check_sum = 0.0;
     for (i=0; i<buffer_count; i++) {
       result[i] = buffer[i];
       check_sum += (double)buffer[i];
     }
     printf("\n %d ping-pong data checksum %f\n",my_ID,check_sum);
#endif
  }

/*********************************************************************
** Synchronize everyone and collect timeing data.
*********************************************************************/
#ifdef DEBUG
  printf("\n ID %d do the barrier and start ping-poinging\n",my_ID);
#endif
  MPI_Barrier (MPI_COMM_WORLD);
  if (my_ID == 0){
    for( iters = iterations ; iters-- ; ) {
       t0 = MPI_Wtime();

MPI_Send (buffer,   buffer_count, MPI_LONG, 1, MTag_3, MPI_COMM_WORLD);
MPI_Recv (incoming, buffer_count, MPI_LONG, 1, MTag_4, MPI_COMM_WORLD, &stat);

       *tp = MPI_Wtime();
       *tp++ -= ( t0 + twtime);

#ifdef DEBUG
  for (diff=0, i=0; i<buffer_count; i++) diff+= abs(result[i]-incoming[i]);
  if(diff > 0) printf("\n error in MPI snd/rcv: diff = %ld\n",diff);
#endif

    } /* end ID 0 loop */
  }
  else {
    for( iters = iterations ; iters-- ; ) {

MPI_Recv (buffer, buffer_count, MPI_LONG, 0, MTag_3, MPI_COMM_WORLD, &stat);
MPI_Send (buffer, buffer_count, MPI_LONG, 0, MTag_4, MPI_COMM_WORLD);

    }/* end ID 1 loop */
  } 

/*********************************************************************
** Produce and output statistics
*********************************************************************/
  if(my_ID == 0){
    printf("Round trip ping-pong times \n");
    printf("\n %d iterations ",iterations);
    printf(" of %d byte messages\n\n",buff_size_bytes);
    time_stats (timings, iterations, buff_size_bytes);
  }
/*********************************************************************
**
** this is not working right now.  Until I debug this code, I will
** comment it out.
**
** Do a memcpy of the same size message to provide a point of
** comparison for the communiation data.  This will be particularly
** relevant when we are running on a shared memory system.
*********************************************************************/
//  MPI_Barrier (MPI_COMM_WORLD);
//  if(my_ID == 0){
//
//    s_l_random( SEED, low_value, hi_value );
//    tp = timings;
//    cpy_count = buffer_count * sizeof(long);
//    for (i=0; i<buffer_count; i++) {
//       incoming[i] = 0L;
//       buffer[i] = l_random();
//    }
//
//    for( iters = iterations ; iters-- ; ) { 
//       t0 = MPI_Wtime();
//
//       incoming = (long *) memcpy(incoming, buffer,   cpy_count);
//       buffer   = (long *) memcpy(buffer,   incoming, cpy_count);
//
//       *tp = MPI_Wtime();
//       *tp++ -= ( t0 + twtime);
//
//#ifdef DEBUG
//  for (diff=0, i=0; i<buffer_count; i++) diff+= abs(result[i]-incoming[i]);
//  if(diff > 0) printf("\n error in memcpy : diff = %ld\n",diff);
//#endif
//
//    }
//
//    printf("\n\nRound trip memcpy times \n");
//    time_stats (timings, iterations, buff_size_bytes);
//  }
/*********************************************************************
** Wait for end and then shut down MPI
*********************************************************************/
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();


}  /* end of main */

