/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

This version of the program uses the Bulk Synchronous parallel
varriant of the SPMD pattern.  

History: Written by Tim Mattson, 11/99.

*/
#include "mpi.h"
#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main (int argc, char **argv)
{
   int i, ID, Nprocs;
   double x, pi, sum = 0.0;
   double start_time, run_time;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &ID);
   MPI_Comm_size(MPI_COMM_WORLD, &Nprocs);

   step = 1.0/(double) num_steps;

   start_time = MPI_Wtime();

   for (i=ID;i<= num_steps; i=i+Nprocs){
      x = (i+0.5)*step;
      sum = sum + 4.0/(1.0+x*x);
   }

   sum = step * sum;
   MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   run_time = MPI_Wtime() - start_time;
   if(ID==0) printf("\n %d procs, pi with %ld steps is %lf in %lf seconds\n ",
                          Nprocs,num_steps,pi,run_time);
   MPI_Finalize();
   return 0;
}	  
