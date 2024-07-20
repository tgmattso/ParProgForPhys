/*

This program shows how to change rounding modes in a C++ program.
We do this while exploring simple summations of 0.01.  The 
goal is to explore properties of floating point arithmetic.

History: Written by Tim Mattson, 09/2023.

*/

#include <iostream>
#include <cfenv>

// enable access to control over the floating point environment.
// It will be enabled until it is turned off or until we reach the
// end of this translation unit.
// #pragma STDC FENV_ACCESS ON

int main()
{
   float dt = 0.01f;
   float sum = 0.0f;

   // change rounding mode.  Options are:
   //         FE_DOWNWARD,   FE_TOWARDZERO,
   //         FE_UPWARD,          FE_TONEAREST
   std::fesetround(FE_TONEAREST);

   int Count = 10000;

   for(int i = 0; i<Count;i++){
     sum += dt;
   }

   printf(" sum = %f or %f\n",sum,dt*Count);

}

