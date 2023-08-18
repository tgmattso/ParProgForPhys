
/*******************************************************************

NAME:    ring_persistent

PURPOSE: This function uses teh persistent communication request
         mechanism to implement the ring communication in MPI.

HISTORY: Written By Tim Mattson, 4/99

*******************************************************************/
#include "ring.h"

void ring_persistent(
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
   MPI_Request snd_req; /* handle to the persistent send    */
   MPI_Request rcv_req; /* handle to the persistent recieve */
   MPI_Status stat;

/*******************************************************************
** In this ring method, I post all the sends and then pick up
** the messages with the receives.
*******************************************************************/
   next       = (my_ID +1 )%num_procs;
   prev       = ((my_ID==0)?(num_procs-1):(my_ID-1));

   MPI_Send_init(x,        buff_count, MPI_DOUBLE, next, 3, 
                                  MPI_COMM_WORLD, &snd_req);
   MPI_Recv_init(incoming, buff_count, MPI_DOUBLE, prev, 3, 
                                  MPI_COMM_WORLD, &rcv_req);
   for(i=0;i<num_shifts; i++){

     MPI_Start(&snd_req);
     MPI_Start(&rcv_req);

     MPI_Wait(&snd_req, &stat);
     MPI_Wait(&rcv_req, &stat);
#ifdef DEBUG
          test_clear(x, incoming, buff_count, i, my_ID);
#endif
   }

}

