
/*******************************************************************

NAME:    ring_split 

PURPOSE: This function does the ring communication, but this time
         the odd numbered nodes send then receive while the even
         nodes receive then send.  

         The sends are blocking sends, but this version of the ring
         test still is deadlock free since each send always has a
         posted recieve.

  
HISTORY: Written By Tim Mattson, 4/99

*******************************************************************/
#include "ring.h"

void ring_split(
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

   if( IS_ODD(my_ID) ){
   
	   for(i=0;i<num_shifts; i++){

MPI_Send (x,        buff_count, MPI_DOUBLE, next, 3, MPI_COMM_WORLD);
MPI_Recv (incoming, buff_count, MPI_DOUBLE, prev, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

#ifdef DEBUG
          test_clear(x, incoming, buff_count, i, my_ID);
#endif
	   }
   }
   else{
      for(i=0;i<num_shifts; i++){

MPI_Recv (incoming, buff_count, MPI_DOUBLE, prev, 3, MPI_COMM_WORLD, &stat);
MPI_Send (x,        buff_count, MPI_DOUBLE, next, 3, MPI_COMM_WORLD);

#ifdef DEBUG
          test_clear(x, incoming, buff_count, i, my_ID);
#endif
      }
   }

}

