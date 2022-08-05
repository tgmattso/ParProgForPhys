/****************************************************************

NAME:      wt_overhead

PURPOSE:   Computes average calling time for MPI_Wtime to use as a 
           correction factor on timing calls.

RETURN:    Double value of the average MPI_Wtime calling overhead
           in seconds

ARGUMENTS: iterations  - input long, number of iterations for average.
          


****************************************************************/
#define  VERBOSE 0L   /* set to 1 for lots of diagnostic output */
#include <stdio.h>
#include <math.h>
#include "mpi.h"

double wt_overhead(iterations)
long   iterations;    /* Number of iterations for computing average*/
{
   double ave_time;   /* average time */
   double std_dev;    /* standard deviation */
   double twtime;     /* timer overhead */
   double t0;         /* initial time */
   double sum_t;      /* sum of times */
   double sum_t2;     /* sum of times squared */
   long   iters;

/*********************************************************************
** Time the timer accumulating values to compute average and std dev.
*********************************************************************/

  for(sum_t=0.0, sum_t2=0.0, iters = iterations ; iters-- ; ) {
      t0     = MPI_Wtime();
      twtime = MPI_Wtime();
      twtime -= t0;
      sum_t += twtime;               /* accumulate sum of times   */
      sum_t2 += twtime * twtime;     /* accumulate sum of squares */
      if(VERBOSE)printf( "MPI_Wtime overhead: %ld %lf\n",iters,twtime);
  }
   
/*********************************************************************
** Compute average and standard deviation 
*********************************************************************/
   ave_time = sum_t/iterations;
   std_dev = (sum_t2-((sum_t*sum_t)/(double)iterations))/(iterations-1);
   std_dev = sqrt(std_dev); 
#ifdef DEBUG
   printf("\n ave MPI_Wtime() = %f ",ave_time); 
   printf(" +/- %f seconds\n",std_dev);        
#endif

   return ave_time;
}  /* end of wt_overhead()*/
