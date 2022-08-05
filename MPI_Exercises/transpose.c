/*******************************************************************

NAME:    transpose

PURPOSE: This MPI program measures communication times for the 
         transpose of a column-bock distributed matrix into a 
         row-block distributed matrix.  The goal is to provide 
         a scalable communication test of MPI -- not to produce 
         a library-quality, general purpose transpose function.
  
USAGE:   Program input is a pair of command line arguments that
         give the Matrix_order/Num_Proc (where Num_Proc is the 
         numb of processes) and the number of times to repeat 
         the operation (iterations):

               transpose Local_block_size iterations

         In other words, we are going to transpose a square
         matrix where the order is divisible by the number
         of processes.
  
         The output consists of diagnostics to make sure the 
         transpose worked and timing statistics.

         Compile with DEBUG defined if you want lots of output.


FUNCTIONS CALLED:

         Other than MPI or standard C functions, the following 
         functions are used in this program:

          trans_sendrcv()   Do the transpose with MPI_Sendrecv()
          fill_mat()        Fill the test matrix
          fill_garbage()    Fill transpose matrix with garbage
          test_results ()   Verify that the transpose worked
          analyze_results() Find min/max time and iteration numbs.
          output_timings () output times and bandwidths

HISTORY: Written by Tim Mattson, April 1999.  
  
*******************************************************************/

#include "mpi.h" 
#include <stdio.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include "trans_utility.h"

#define TOL        0.0001
#define MAX_ITERS  1000  /* chosen to match Results output formatting */

                         /* Max local_block_size for SND_RCV transpose */
                         /* to NOT deadlock.  Needs to be set by hand  */
#define MAX_SND_RCV  32  /* for each MPI implementation/system since   */
                         /* its dependent on how messages are buffered */

int main(int argc, char ** argv)
{
   int Block_order=5;    /* order of a local matrix block */
   int Num_procs=1;      /* Number of processors */
   int Global_order;     /* Order of Global matrix */
   int Col_block_size;   /* size of local col blck */
   int block_size;       /* size of a single block */
   int my_ID = 0;        /* Process ID (i.e. MPI rank) */
   int iterations;       /* number of times to do the transpose */
   int i, j, iters;

   double *buff;           /* buffer to hold local matrix block */
   double *trans;          /* buffer to hold transposed data */
   double *work;           /* workspace for the tranpose funtion */

   double errsq_sendrcv;   /* squared error, Sendrecv */

   double t0, trans_time;

   double min_time_sendrcv,   max_time_sendrcv, min_max_time_sendrcv;
   int    min_iter_sendrcv,   max_iter_sendrcv, min_max_iter_sendrcv;
   
   double test_results (int , int , int , double*);
   void   trans_sendrcv   (double*, double*, int, double*, int, int);
   void   fill_mat(int , int , int , double*);
   void   fill_garbage(int , int , int , double*);
   void   output_timings (char *, int , int , double , double, double, 
                          int, int , int);
   void   analyze_results (int, double, int, double *, double*, double*, 
                          int*, int*, int*);

/*********************************************************************
** Initialize the MPI environment
*********************************************************************/
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_ID);
  MPI_Comm_size(MPI_COMM_WORLD, &Num_procs);

#ifdef DEBUG
  printf("\n after MPI_Init and MPI_Comm_rank %d\n",my_ID);
#endif

/*********************************************************************
** process, test and broadcast input parameters
*********************************************************************/
  if(my_ID == 0){
     if (argc == 3){
        Block_order = atoi(*++argv);
        iterations  = atoi(*++argv);
     }
     else{
        printf("Usage: %s <Local block order> <Num of iterations> \n",
                                                               *argv);
        printf(" Default values will be used\n");
        fflush(stdout);
        Block_order = 100;
        iterations  = 4;
     }

     printf("\nMPI communication test: Matrix Transpose on %d nodes\n", 
                                                            Num_procs);

     printf("Transpose an order %d matrix %d times \n",
            Num_procs*Block_order, iterations);
     fflush(stdout);

      /*  Test Input */
     if(Num_procs < 1) {
        printf("ERROR: Must have at least 1 process: %d \n",Num_procs);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }
     if(Block_order < 0){
        printf("ERROR: Block Order must be greater than 0 : %d \n",
                Block_order);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }
     if(iterations < 1){
        printf("ERROR: iterations must be >= 1 : %d \n",iterations);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }
     if(iterations > MAX_ITERS){
        printf("ERROR: too many iterations : %d \n",iterations);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }
  }

  /*  Broadcast benchmark data to all processes */
  MPI_Bcast (&Block_order, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast (&iterations,  1, MPI_INT, 0, MPI_COMM_WORLD);

#ifdef DEBUG
  printf("\n ID = %d , nodes  = %d ",my_ID,Num_procs);
  printf("Block_order=%d , iterations =%d \n",Block_order,iterations);
  fflush(stdout);
#endif

/*********************************************************************
** Key Matrix parameters: The matrix is broken up into column blocks
** that are mapped one to a node.  Each column block is made of 
** Num_procs smaller blocks of order block_order.
**
**   Global_order   - order of the global matrix
**   Col_block_size - numb of elements in a local column block.
**   block_size     - numb of elements in a working block.
*********************************************************************/
   Global_order   = Block_order  * Num_procs;
   Col_block_size = Global_order * Block_order;
   block_size     = Block_order  * Block_order;

/*********************************************************************
** Create the column block of the test matrix, the row block of the 
** transposed matrix, and workspace.
*********************************************************************/
   buff   = (double *)malloc(Col_block_size*sizeof(double));
   if (buff == NULL){
        printf(" Error allocating space for buff on node %d\n",my_ID);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
   }
   trans  = (double *)malloc(Col_block_size*sizeof(double));
   if (trans == NULL){
        printf(" Error allocating space for trans on node %d\n",my_ID);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
   }
   work   = (double *)malloc(block_size*sizeof(double));
   if (work == NULL){
        printf(" Error allocating space for work on node %d\n",my_ID);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
   }
  
   /*
   **  Fill the original column matrix in buff. 
   **  Set the transpose matrix to a known garbage value.
   */ 

   fill_mat(my_ID, Global_order, Block_order, buff);

   fill_garbage(my_ID, Global_order, Block_order, trans);
   
/*********************************************************************
** Do the SENDRCV transpose..
*********************************************************************/
  if(my_ID == 0){
     min_time_sendrcv     = 1/TOL;
     max_time_sendrcv     = TOL;
     min_max_time_sendrcv = 1/TOL;
     min_iter_sendrcv     = -1;
     max_iter_sendrcv     = -1;
     min_max_iter_sendrcv = -1;
  }   
  fill_garbage(my_ID, Global_order, Block_order, trans);
  errsq_sendrcv = 0.0;
  for (iters = 0; iters<iterations; iters++){

     MPI_Barrier(MPI_COMM_WORLD);
     t0 = MPI_Wtime();

     trans_sendrcv(buff,  trans, Block_order,
                work,  my_ID, Num_procs);

     trans_time = MPI_Wtime() - t0;

#ifdef DEBUG
     printf("\n Node %d has finished with sendrcv transpose \n",my_ID);
     fflush(stdout);
#endif

    
     analyze_results (my_ID, trans_time, iters,
         &min_time_sendrcv, &max_time_sendrcv, &min_max_time_sendrcv, 
         &min_iter_sendrcv, &max_iter_sendrcv, &min_max_iter_sendrcv);

     errsq_sendrcv +=  test_results (my_ID, Block_order, Global_order, 
		                             trans);

   }  /* end of iters loop  */

/*********************************************************************
** Analyze and output results.
*********************************************************************/
   if(my_ID == 0){

     printf("\n Sendrecv Transpose Test Results ");
     output_timings ("Sendrecv ", Num_procs, Block_order,
         min_time_sendrcv, max_time_sendrcv, min_max_time_sendrcv, 
         min_iter_sendrcv, max_iter_sendrcv, min_max_iter_sendrcv);

     printf("\n squared errors: %f  \n",
              errsq_sendrcv );
   }

/*********************************************************************
** Wait for end and then shut down MPI
*********************************************************************/
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();


}  /* end of main */

