/*

NAME:   
   summation

Purpose: 
   Create an array of numbers and explore floating point 
   arithmetic issues as you sum the array
   
Usage:
   This program depends on a set of functions from the file
   UtilityFunctions.c.   For reasons of pedagogy, do not look
   at the functions in UtilityFunctions.c. Treat it as a block box.

History: 
   Written by Tim Mattson, 9/2023.

*/
#include <stdio.h>
#include <stdlib.h>
#include "UtilityFunctions.h"

#define N 100000  //length of sequence of numbers to work with  

//============================================================================
// Functions used from UtilityFunctions.h
//============================================================================
void FillSequence(float*, float*);       // fill sequence (>0), return "true" sum
int comp(const void *, const void *); // comparator func for qsort

//============================================================================

int main ()
{
   float seq[N];    //Sequence to sum, statically defined to keep the code simple
   float True_sum;  //The best estimate of the actual sum

   FillSequence(seq, &True_sum); // populate seq with random values > 0

   // basic, naive sum
   float sum_val =0.0f;
   for (int i=0; i<N; i++){
       sum_val += seq[i];
   }
   printf(" Basic sum = %f, best estimated sum = %f\n",sum_val,True_sum);

}

/**********************************************************************************************
//  This is a partial implementatioin of Kahan's compensated summation.  Using
//   what we've discussed about errors in addition and subtraction, complete
//   figure out how to compute the error correction term

float KahanSum(int Nlen, float *x)
{
   float ksum, correction, xcorrected, tmpSum;

   if(Nlen < 1) return -1;  // need at least two values to sum

   correction = 0.0f;
   ksum = 0.0f;
   for (int i = 0; i<Nlen;i++){
      xcorrected = x[i] - correction; 
      tmpSum =  ksum + xcorrected;
      
      correction = ??????????;
      
      ksum = tmpSum;
   }
   return ksum;
}
**********************************************************************************************/
