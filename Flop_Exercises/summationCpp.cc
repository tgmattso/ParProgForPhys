/*

NAME:   
   summation (the C++ version)

Purpose: 
   Create an array of numbers and explore floating point 
   arithmetic issues as you sum the array
   
Usage:
   This program depends on a set of functions from the file
   UtilityFunctions.c.   For reasons of pedagogy, do not look
   at the functions in UtilityFunctions.c. Treat it as a block box.

        g++ summation.cc UtilityFunctions.c

History: 
   Written by Tim Mattson, 9/2023.

*/
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include "UtilityFunctions.h"  // where FillSequence comes from

#define N 100000  //length of sequence of numbers to work with  

//============================================================================

int main ()
{
   std::vector<float> seq(N); //Sequence to sum 
   float True_sum;            //An estimate of the actual sum
   float sum = 0.0f;

   FillSequence(N, seq.data(), &True_sum); // populate seq with N random values > 0

// Add code here to sum the elements of the array of length N, seq

   printf(" Sum = %f, Estimated sum = %f\n",sum, True_sum);

}
