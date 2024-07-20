/*

This program shows how to change rounding modes in a C program.
We do this while exploring simple summations of 0.01.  The 
goal is to explore properties of floating point arithmetic.

History: Written by Tim Mattson, 09/2023.

*/

#include <stdio.h>
#include <fenv.h>

// This pragma is supposed to enable access to control over the 
// floating point environment.  It will be enabled until it is 
// turned off or until we reach the end of this translation unit.
//
// unfortunately, most (if not all) compilers do not support it.
// This is OK, since the rounding mode adjustments still seem to 
// work.
// #pragma STCD FENV_ACCESS ON

#define TYPE    float 
float epsilon() {
   TYPE one = (TYPE)1.0;
   TYPE eps = (TYPE)1.0;
   long iters = 0;

   while( (one+(TYPE)eps)>one){
      eps = eps/(TYPE)2.0;
      iters++;
   }
   return eps;
}

int main()
{
   float sum=0.0f, dt = 0.01f;
   int Count = 10000;
   float eps;
   eps = epsilon();

   // save originial rounding mode
   const int origRound = fegetround();

   // change rounding mode.  Options are FE_DOWNWARD, FE_TOWARDZERO,
   // FE_UPWARD, FE_TONEAREST
   fesetround(FE_TOWARDZERO);
   for(int i = 0; i<Count;i++){
     sum += dt;
   }
   printf(" round towardZero sum = %f or %f\n",sum,dt*Count);
   printf(" %g \n",1.0+eps);
  

   fesetround(FE_DOWNWARD);
   for(int i = 0,sum=0.0f; i<Count;i++){
     sum += dt;
   }
   printf(" round downward sum = %f or %f\n",sum,dt*Count);
   printf(" %g \n",1.0+eps);

   fesetround(FE_UPWARD);
   for(int i = 0,sum=0.0f; i<Count;i++){
     sum += dt;
   }
   printf(" round upward sum = %f or %f\n",sum,dt*Count);
   printf(" %g \n",1.0+eps);

   fesetround(FE_TONEAREST);
   for(int i = 0,sum=0.0f; i<Count;i++){
     sum += dt;
   }
   printf(" round to nearest sum = %f or %f\n",sum,dt*Count);
   printf(" %g \n",1.0+eps);

   // Return to orignial rounding mode.  
   fesetround(origRound);
//   for(int i = 0,sum=0.0f; i<Count;i++){
//     sum += dt;
//   }
//
//   printf(" original model: sum = %f or %f\n",sum,dt*Count);
//   printf(" %g \n",1.0+eps);

}
