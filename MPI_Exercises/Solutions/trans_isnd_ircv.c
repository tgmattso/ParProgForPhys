/*******************************************************************

NAME:    trans_isend_ircv

PURPOSE: This function uses MPI ISND's and IRCV's to transpose 
         of a column-bock distributed matrix.  The algorithm
         itself is described in the comments right before the
         major communication operations.

         This version of the code differs from the others by
		 using immediate mode communication to avoid deadlock
		 and to let me play around with overlapping computation 
		 with communication (though there really isn't much 
		 computation to overlap --- hence i don't expect this 
		 overlap to be noticible).
  
HISTORY: Written by Tim Mattson, April 1999.  
  
*******************************************************************/

#include "mpi.h" 
#include <stdio.h>
#include "trans_utility.h"

/*******************************************************************
** Define macros to compute processor source and destinations
*******************************************************************/
#define TO(ID,   PHASE, NPROC)  ((ID + PHASE        ) % NPROC)
#define FROM(ID, PHASE, NPROC)  ((ID + NPROC - PHASE) % NPROC)

void trans_isnd_ircv(double *buff,  double *trans, int Block_order,
                   double *work,  int my_ID, int Num_procs)
{
   int iphase;
   int block_size;
   int send_to, recv_from;
   double *bblock;    /* pointer to current location in buff */
   double *tblock;    /* pointer to current location in trans */
   MPI_Status status;
   MPI_Request send_req, recv_req;
 
   block_size = Block_order * Block_order;

/*******************************************************************
**  Do the tranpose in Num_procs phases.  
**
**  In the first phase, do the diagonal block.  Then move out 
**  from the diagonal copying the local matrix into a communication 
**  buffer (while doing the local transpose) and send to processor 
**  (diag+phase)%Num_procs.
*******************************************************************/
   bblock = buff  + my_ID*block_size;
   tblock = trans + my_ID*block_size;

   transpose(bblock, Block_order, tblock, Block_order, 
             Block_order, Block_order);

   for (iphase=1; iphase<Num_procs; iphase++){

      recv_from = FROM(my_ID, iphase, Num_procs);
	 
      tblock  = trans + recv_from * block_size;

      MPI_Irecv (tblock, block_size, MPI_DOUBLE, recv_from,
              iphase, MPI_COMM_WORLD, &recv_req);  

      send_to = TO(my_ID, iphase, Num_procs);

      bblock  = buff + send_to * block_size;

      transpose(bblock, Block_order, work, Block_order, 
             Block_order, Block_order);
	 
#ifdef DEBUG 
   printf("\n transposed block for phase %d on node %d is \n",
               iphase,my_ID);
   outmat(work, Block_order, Block_order);
   fflush(stdout);
#endif
  
      MPI_Isend (work, block_size, MPI_DOUBLE, send_to,
                iphase, MPI_COMM_WORLD, &send_req);

#ifdef DEBUG
   printf("\n phase %d on node %d, recv_from = %d and send_to = %d\n",
                             iphase, my_ID, recv_from, send_to);
   printf("\n %x %d %d %d %x \n", tblock, block_size, recv_from, 
                             iphase, &status );
   fflush(stdout);
#endif

   MPI_Wait(&recv_req,  &status);
   MPI_Wait(&send_req, &status);

   }
}

