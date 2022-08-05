
/*******************************************************************

NAME:    ring_sndrcv

PURPOSE: This function does the ring communication, but this time
         we use the MPI_Sendrecv() function to do the communication.

         This is supposed to prevent deadlock due to buffer 
         management on the sends.
  
HISTORY: Written By Tim Mattson, 4/99

*******************************************************************/
#include "ring.h"

ring_sndrcv(
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
** In this ring method, odd nodes snd/rcv and even nodes rcv/snd.
*******************************************************************/
   next       = (my_ID +1 )%num_procs;
   prev       = ((my_ID==0)?(num_procs-1):(my_ID-1));

   
   for(i=0;i<num_shifts; i++){
      MPI_Sendrecv (x,        buff_count, MPI_DOUBLE, next, 3, 
                    incoming, buff_count, MPI_DOUBLE, prev, 3, 
                                          MPI_COMM_WORLD, &stat);
#ifdef DEBUG
          test_clear(x, incoming, buff_count, i, my_ID);
#endif
   }

}

