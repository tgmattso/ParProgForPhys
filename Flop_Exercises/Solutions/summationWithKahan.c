/*

NNAME:   
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
// we use the following function from UtilityFunctions.c
//
// void FillSequence(float*, float*);    // fill sequence (>0), return "true" sum
// float KahanSum(int, float*);          // Kahan Sum of a sequence
// int comp(const void *, const void *); // comparator func for qsort
//
//============================================================================

int main ()
{
   float seq[N];       //Sequence to sum, statically defined to keep the code simple
   float True_sum;  //The best estimate of the actual sum

   FillSequence(seq, &True_sum); // populate seq with random values > 0


   // basic, naive sum
   float sum_val =0.0f;
   for (int i=0; i<N; i++){
       sum_val += seq[i];
   }
   printf(" Basic sum = %f, best estimated sum = %f\n",sum_val,True_sum);

   // Do the Kahan Summation on the original sequence
   sum_val = KahanSum(N, seq);
   printf(" Kahan sum of unsorted sequence = %f, best estimated sum = %f\n",sum_val,True_sum);
   
   // Sort the sequence then sum the sorted sequence
   sum_val = 0.0f;
   qsort(seq, N, sizeof(float), comp);
   for (int i=0; i<N; i++){
       sum_val += seq[i];
   }
   printf(" Basic sum Sorted seq = %f, best estimated sum = %f\n",sum_val,True_sum);

   // Do the Kahan Summation on the sorted sequence
   sum_val = KahanSum(N, seq);
   printf(" Kahan sum of sorted seq = %f, best estimated sum = %f\n",sum_val,True_sum);

}

