#include <stdio.h>

//
// This program computes the "rounding machine epsilon". This is the largest relative 
// rounding error for round-to-nearest. A "relative" rounding error is normalized to 
// the value computed, hence we compute this by finding the largest number we can 
// add to one (in a floating type) and have the value stored equal one.
//
// Change the parameter TYPE to float or double to explore different floating point types.
//
//  For float, the rounding machine epsilon should be around 5.96e-08 or 2**-24
//  For double, the rounding machine epsilon should be 1.11e-16 or 2**-53
//

#define SUCCESS 1
#define FAIL    0
#define TYPE    float 
//#define TYPE   double 

int main()
{

   TYPE one = (TYPE)1.0;
   TYPE eps = (TYPE)1.0;
   long iters = 0;

   while( (one+(TYPE)eps)>one){
      eps = eps/(TYPE)2.0;
      iters++;
   }

   printf("For numbers of width %lu bytes, rounding mach epsilon is 2 to the -%ld or %g\n",
                         sizeof(TYPE),iters,eps);
   
   return SUCCESS;
}
