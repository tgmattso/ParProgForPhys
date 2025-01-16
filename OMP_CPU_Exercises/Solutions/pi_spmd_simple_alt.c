/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdio.h>
#include <omp.h>
#define NUMT 4
static long num_steps = 100000000;
double step;
int main ()
{
          int nthreads;
	  double x, pi, sum[NUMT];
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;

        	 
	  start_time = omp_get_wtime();
#pragma omp parallel
{
   int id = omp_get_thread_num();
   int nthr = omp_get_num_threads();
   if(id==0) nthreads=nthr;
   int nstart, nend;
   nstart = id*(num_steps/nthr);
   nend = (id+1)*(num_steps/nthr)-1;
   sum[id]=0.0;
   if(id==nthr) nend = num_steps;
	  for (int i=nstart;i< nend; i++){
		  double x = (i+0.5)*step;
		  sum[id] = sum[id] + 4.0/(1.0+x*x);
	  }

}
          for(int i=0;i<nthreads;i++)pi +=sum[i];
	  pi = step * pi;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
          printf(" number of threads = %d\n",nthreads);
}	  





