/*

This program explores a function the evaluation of which
suffers from cancelation under floating point arithmetic.

The function is:

      f(x) = 1.0/(sqrt(x*x + 1) - x)

everything is fine for small x, but as x grows, we experience
a cancelation problem.  

The solution is to refactor the above function with lots of
algebra into the equivalent form:

      f(x) = sqrt(x*x + 1) + x

This program computes the function both ways and compares
results for a range of x values.

History: Written by Tim Mattson, 06/2024.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TYPE double 

TYPE funcOriginal(TYPE xIn){
      return 1.0/(sqrt(xIn*xIn + 1) - xIn);
}

TYPE funcRefactor(TYPE xIn){
      return sqrt(xIn*xIn + 1) + xIn;
}

int main(int argc, char *argv[])
{

   TYPE xInput, xmin = (TYPE)1000000.0, xmax = (TYPE)10000000.0, xdelta = (TYPE)500000.0; 

   if(argc == 2){
      xInput = atoi(argv[1]);
      xmin = xInput;
      xmax = xInput+1.0;
   }
   for(TYPE x = xmin; x<xmax; x = x+xdelta){
        TYPE outOriginal = funcOriginal(x); 
        TYPE outRefactor = funcRefactor(x); 
        printf("For %f, original = %f, refactored = %f\n",
                                   x,outOriginal, outRefactor);
   }
}
