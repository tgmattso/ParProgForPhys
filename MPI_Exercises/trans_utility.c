/*******************************************************************

NAME:    transpose utility functions

PURPOSE: These functions support the transpose timing test. 
 
         The functions provided here are:

             void fill_mat ()
             double test_results ()
             void fill_garbage ()
             void output_timings ()
             void analyze_results ()
             void outmat()
             void transpose()

HISTORY: Written by Tim Mattson, May 1999.  
  
*******************************************************************/
#include "mpi.h"
#include <stdio.h>
//#include "trans_utility.h"

double col_shift = 1000.00;/* Constant to shift column index */
double row_shift = 0.001;  /* Constant to shift row index */

/*********************************************************************
**  fill_mat
**
** Call on each node to fill the original column matrix in buff. 
**
** Note: this function is tightly linked to the test_results() 
** function.
**
*********************************************************************/	

void fill_mat(int my_ID, int Global_order, int Block_order, 
              double *buff)
{
   int i, j;
   double col_val;
   for (j=0;j<Block_order;j++) {
      col_val = col_shift * (my_ID*Block_order + j);
      for (i=0;i<Global_order; i++) {
         *(buff+i*Block_order+j) = col_val + row_shift * i;
      }
   }
#ifdef DEBUG
   printf(" Output original column block on node %d\n",my_ID);
   outmat(buff,Global_order,Block_order);
   fflush(stdout);
#endif
}

/*********************************************************************
** Test Results.  
**
** Call this on each node.  It will verify that the input array (trans)
** holds the block of the transpose matrix expected on the node 
** of rank my_ID
**
** Note: this function is tightly linked to the fill_mat() function.
*********************************************************************/	

double test_results (int my_ID, int Block_order, int Global_order, 
	             	double *trans)
{
   double diff;
   int i,j;
   double col_val;
   double errsq;

   errsq = 0.0;
   for (j=0;j<Block_order;j++) {
      for (i=0;i<Global_order; i++) {
         col_val = col_shift * i; 	
         diff = *(trans+i*Block_order+j) -
               (col_val  + row_shift * (my_ID*Block_order + j));
         errsq += diff*diff;
      }
   }
     

#ifdef DEBUG
      printf(" Squared sum of differences: %f\n",errsq);
      printf(" Output transposed column block on node %d \n",my_ID);
      outmat(trans,Global_order, Block_order);
      fflush(stdout);
#endif   
   return errsq;

}


/*********************************************************************
**  fill_garbage
**
** Call on each node to fill the column matrix with garbage. 
** In this case, we define garbage as "-1".
*********************************************************************/	

void fill_garbage(int my_ID, int Global_order, int Block_order, 
	             double *trans)
{
   int i, j;
   for (j=0;j<Block_order;j++) {
      for (i=0;i<Global_order; i++) {
         *(trans+i*Block_order+j) = -1.0;
      }
   }
}

/*********************************************************************
** Analyze Results
**
** Call  on every node.   Output values only relevant on node of 
** rank 0
** 
** This function will collect min and max time statistics for all
** processes in MPI_COMM_WORLD.  Make sure on the my_ID == 0 process,
** that max_time has been set to a small number and the two min times
** have been set to large numbers to start the process off. 
*********************************************************************/	

void analyze_results (int my_ID, double trans_time, int iters,
	 double *min_time, double *max_time, double *min_max_time,
	 int *min_iter, int *max_iter, int *min_max_iter)
{
   double new_min_time;
   double new_max_time;

   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Reduce(&trans_time, &new_min_time, 1, MPI_DOUBLE, MPI_MIN, 0, 
                                                    MPI_COMM_WORLD);
   MPI_Reduce(&trans_time, &new_max_time, 1, MPI_DOUBLE, MPI_MAX, 0, 
                                                    MPI_COMM_WORLD);

   if(my_ID == 0){
#ifdef DEBUG
      printf("\n on %d, min & max times %f %f \n",
               my_ID,new_min_time, new_max_time);
#endif
      if( new_min_time < *min_time) {
         *min_time = new_min_time;
         *min_iter = iters;
      }
      if( new_max_time > *max_time) {
         *max_time = new_max_time;
         *max_iter = iters;
      }
      if( new_max_time < *min_max_time ) {
         *min_max_time = new_max_time;
         *min_max_iter = iters;
      }
   }
}


/*********************************************************************
** output_timings
**
** Call on the node of rank 0 (my_ID == 0) to compute bandwidths
** and output resutls to stdout.
**
** Note: to be consistent with ring test and output what is the most
** defensible  single bandwidth number, the bandwidth to publish is
** the one computed from the shortest maximum communication time.
*********************************************************************/

void output_timings (char *lable, int Num_procs, int Block_order,
            double min_time, double max_time,  double min_max_time,
            int    min_iter, int    max_iter,  int    min_max_iter)
{
  double Mbytes_trans;       /* Total Megabytes transfered */
  int block_size;
  
  block_size = Block_order * Block_order;

  Mbytes_trans = block_size * Num_procs * (Num_procs-1)*sizeof(double);
  Mbytes_trans = Mbytes_trans/(1000000.0);

  printf("\n min_time(%3d) = %f; max_time(%3d) = %f \n",
              min_iter, min_time, max_iter,max_time);
  if(( min_time > 0) && (max_time > 0.0)){
      printf("Bandwidths (MB/sec) from %f to %f\n",
                   Mbytes_trans/max_time, Mbytes_trans/min_time); 
  }
  if (min_max_time > 0.0){
     printf(" %s: order %d matrix. %f Mbytes at %f Mbytes/sec \n", 
          lable, Block_order * Num_procs, Mbytes_trans, 
          Mbytes_trans/min_max_time);
  }
}
/*******************************************************************
** outmat:  output an array. Formatting is simple so use this only 
**          tiny matrices.
*********************************************************************/

void outmat(
   double* buff, /* The base address of the matrix to output */
   int Nrows,    /* The number of rows in the matrix */
   int Ncols)    /* The number of columns in the matirx */
{
   int i,j;
   for (i=0;i<Nrows;i++){
      for(j=0;j<Ncols;j++){
         printf(" %10f ",*(buff+i*Ncols+j));
      }	
      printf("\n");
   }
}


/*******************************************************************
** Copy the transpose of an array slice inside matrix A into 
** the an array slide inside matrix B.
**
**  Parameters:
**
**       A, B                base address of the slices
**       Acols               Numb or cols in full A matrix
**       Bcols               Numb or cols in full B matrix
**       sub_rows, sub_cols  Numb of rows/cols in the slice.
** 
*******************************************************************/
void transpose(
   double* A,    int Acols,      /* input matrix */
   double* B,    int Bcols,      /* transposed mat */
   int sub_rows, int sub_cols)   /* size of slice to  transpose */
{
   int i,j,jinc;
   double *Abase;
   double *Bbase;
	
/*  Transpose the  matrix.  */
   Abase = A;
   Bbase = B;
   for (i=0;i<sub_cols; i++) {
      jinc = 0;
      for (j=0;j<sub_rows;j++) {
         *(Bbase+jinc) = *(Abase+j);
         jinc += Bcols;
      }
      Abase += Acols;
      Bbase += 1;
   }
	
}
