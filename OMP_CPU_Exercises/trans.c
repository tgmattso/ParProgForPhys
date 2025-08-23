/*

NAME:
   trans

Purpose:
   This program generates and then transposes a square matrix.  The
   goal is to have some code to play with when exploring system NUMA
   issues.

Usage:
   To keep the program as simple as possible, you must edit the file
   and change the order of the matrices (N)

History: 
   Written by Tim Mattson, August 2025

*/
#include <stdlib.h> // needed on Apple for malloc. 
#include <stdio.h>
#include <omp.h>

//#define N 50000 // largest order tested on my apple laptop
#define N 30000
#define MByte 1000000.0

int main()
{
  double *A = (double *)malloc(sizeof(double)*N*N);
  double *B = (double *)malloc(sizeof(double)*N*N);

  // initialize arrays
  for (long i=0; i<N; i++)
     for (long j=0; j<N; j++){
         *(A+i+N*j) = (double)(2.0*i+j);
         *(B+i+N*j) = 0.0;
     }

  // carry out the transpose
  double Tstart = omp_get_wtime();
  for (long i=0; i<N; i++)
     for (long j=0; j<N; j++)
         *(B+i+N*j) = *(A+j+N*i);
  double RunTime = omp_get_wtime() - Tstart;
  double mbps = ((double)(sizeof(double)*N*N))/(RunTime * MByte);

  printf(" %f seconds or %f megabytes per second \n",RunTime,mbps);

  // test results
  double errsq = 0.0;
  for (long i=0; i<N; i++)
     for (long j=0; j<N; j++){
         double val = (*(B+i+N*j) - *(A+j+N*i));
         errsq += val*val;
     }

  printf("transpose squared error %lf \n",errsq);
  return 1;
}
