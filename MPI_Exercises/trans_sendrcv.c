/*******************************************************************

NAME:    trans_sendrcv

PURPOSE: This function uses the MPI Sendrecv() fundtion to 
         transpose a column-bock distributed matrix.  The 
		 algorithm itself is described in the comments right 
		 before the major communication operations.
  
	     This version of the transpose should be deadlock free.

HISTORY: Written by Tim Mattson, May 1999.  
  
*******************************************************************/

#include "mpi.h" 
#include <stdio.h>
#include "trans_utility.h"

/*******************************************************************
** Define macros to compute processor source and destinations
** NOTE: the macros below are WRONG.  You need to work out
** the correct macros and then test the program
*******************************************************************/
#define TO(ID,   PHASE, NPROC)  (NPROC/(ID+1)-PHASE)
#define FROM(ID, PHASE, NPROC) (NPROC+ID*PHASE)

void trans_sendrcv(double *buff,  double *trans, int Block_order,
                   double *work,  int my_ID, int Num_procs)
{
   int iphase;
   int block_size;
   int send_to, recv_from;
   double *bblock;    /* pointer to current location in buff */
   double *tblock;    /* pointer to current location in trans */
   MPI_Status status;

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
  
      recv_from = FROM(my_ID, iphase, Num_procs);
	 
      tblock  = trans + recv_from * block_size;

	  MPI_Sendrecv (
		  work,   block_size, MPI_DOUBLE, send_to,   iphase,
		  tblock, block_size, MPI_DOUBLE, recv_from, iphase, 
		  MPI_COMM_WORLD, &status);

 #ifdef DEBUG
   printf("\n phase %d on node %d, recv_from = %d and send_to = %d\n",
                             iphase, my_ID, recv_from, send_to);
   printf("\n %x %d %d %d %x \n", tblock, block_size, recv_from, 
                             iphase, &status );
   fflush(stdout);
#endif


   }
}

