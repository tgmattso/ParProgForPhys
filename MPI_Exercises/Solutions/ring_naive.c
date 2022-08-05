
/*******************************************************************

NAME:    ring_naive 

PURPOSE: This function is a Naive implementation of ring 
         communication.  I post all the sends and then post all
         the receives.  

         The problem is, these sends are blocking sends meaning 
         they don't return until it is safe to modify the variables 
         referenced in the argument list.  This means that depending 
         on how the system buffers the messages, the sends may not 
         return until a receive has been posted meaning.

         The result: THIS CODE WILL DEADLOCK ON MOST SYSTEMS.  It
         may work for tiny messages, but as the message size 
         increases, deadlock is a real possibility with this code.
  
HISTORY: Written By Tim Mattson, 4/99

*******************************************************************/
#include "ring.h"

ring_naive(
           double *x,        /* message to shift around the ring */
           double *incoming, /* buffer to hold incoming message  */
           int buff_count,   /* size of message                  */
           int num_procs,    /* total number of nodes            */
           int num_shifts,   /* numb of times to shift messege   */
           int my_ID)        /* node id number                   */
{
   int next;            /* node id of the next node */      
   int prev;            /* node id of the prev node */      
   int i;
   MPI_Status stat;

/*******************************************************************
** In this ring method, I post all the sends and then pick up
** the messages with the receives.
*******************************************************************/
   next       = (my_ID +1 )%num_procs;
   prev       = ((my_ID==0)?(num_procs-1):(my_ID-1));

   for(i=0;i<num_shifts; i++){

MPI_Send (x,        buff_count, MPI_DOUBLE, next, 3, MPI_COMM_WORLD);
MPI_Recv (incoming, buff_count, MPI_DOUBLE, prev, 3, MPI_COMM_WORLD, &stat);

#ifdef DEBUG
          test_clear(x, incoming, buff_count, i, my_ID);
#endif
   }

}

