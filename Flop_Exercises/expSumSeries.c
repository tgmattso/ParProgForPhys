/*

This program computes e to the x using a series expansion:

      e^x = 1 + x + (x^2)/2! + (x^3)/3! + (x^4)/4! + ...

We check for a sequence of x values and compare 
to the result from the math library (exp() from math.h).   

Then we consider the series for negative x (-1, -3, -10, -50)
and compare to the reciprocal of the positive x result.

History: Written by Tim Mattson, 06/2024.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TYPE float
#define BIG  1000.0
#define TOL  1e-16
#define ABS(val) ((val)>0.0?(val):(-val))

// A naive implementation of the series solution which means 
// large powers of X and the factorial are computed.  Also, we do
// not take negative arguments into account (we assume delta is 
// always positive).
TYPE MyExpNANsNoNegArg(TYPE x)
{
    TYPE xterm   = (TYPE)1.0;
    TYPE nterm   = (TYPE)1.0;
    long counter = 0;
    TYPE delta = BIG;
    TYPE e_tothe_x = (TYPE)1.0;

    while(delta > (TYPE)TOL){
        counter++;
        xterm *=x;
        nterm *=counter;
        delta = xterm/nterm;
        e_tothe_x +=delta;
    }
//    printf(" %ld terms  ",counter);
    return e_tothe_x;
}

// Do not compute factorial and large powers of x
// but still do not take negative arguments into account
TYPE MyExpNoNegArg(TYPE x)
{
    long counter = 0;
    TYPE delta = (TYPE)1.0;
    TYPE e_tothe_x = (TYPE)1.0;

// the factorial grew too large.   Scale each term by the
// argument over the counter so I never form the huge factorial
// or the high powers of the input argument.  
    while(delta > (TYPE)TOL){
        counter++;
        delta *= x/counter;
        e_tothe_x +=delta;
    }
//    printf(" %ld terms  ",counter);
    return e_tothe_x;
}

// take negative arguments into account using
// absolute value on delta test
TYPE MyExp1(TYPE x)
{
    long counter = 0;
    TYPE delta = (TYPE)1.0;
    TYPE e_tothe_x = (TYPE)1.0;

// absolute value in while loop to handle negative x
    while(ABS(delta) > (TYPE)TOL){
        counter++;
        delta *= x/counter;
        e_tothe_x +=delta;
    }
//    printf(" %ld terms  ",counter);
    return e_tothe_x;
}


// Final version of my exponential function for positive and
// negative arguments.  To understand how to manage negative
// arguments for x, look at how I test for completion.
TYPE MyExp(TYPE x)
{
    long counter = 0;
    TYPE delta = (TYPE)1.0;
    TYPE e_tothe_x = (TYPE)1.0;

    while((1.0 + delta) != 1.0) { 
        counter++;
        delta *= x/counter;
        e_tothe_x +=delta;
    }
//    printf(" %ld terms  ",counter);
    return e_tothe_x;
}

// Same as above, but I separately combine positive and negative terms
// in the sum to see if this will diminish the magnitude of the cancelation effect
// for negative x.
TYPE MyExpSepPosNeg(TYPE x)
{
    long counter = 0;
    TYPE delta = (TYPE)1.0;
    TYPE e_tothe_x = (TYPE)1.0;

    if(x<0.0){
       TYPE PosSum=(TYPE)0.0, NegSum = (TYPE)0.0;
       while((1.0 + delta) != 1.0) { 
           counter++;
           delta *= x/counter;
//           printf("%d %g\n",counter,delta);
           if(delta>0.0)
               PosSum+=delta;
           else
               NegSum+=delta;
       }
//   printf(" PosSum and NegSum %g %g\n",PosSum,NegSum);
       e_tothe_x += PosSum+NegSum;
    }
    else{
       while((1.0 + delta) != 1.0) { 
           counter++;
//           printf("%d %g\n",counter,delta);
           delta *= x/counter;
           e_tothe_x +=delta;
       }
    }
//    printf(" %ld terms  ",counter);
    return e_tothe_x;
}


int main(int argc, char *argv[])
{

   int i,Count = 3;

   if(argc == 2){
      Count = atoi(argv[1]);
      printf("MyExp (%d)           = %g and mathlib value =%g\n",Count,MyExp(Count),exp(Count));
   }
   else{
   
     for(i = 20; i> -21; i=i-5){
        TYPE TrueVal      = exp((TYPE)i);
        TYPE MyExpVal     = MyExp((TYPE)i);
        TYPE MyExpValPosNeg = MyExpSepPosNeg((TYPE)i);
        if(i<0){
          TYPE OneOverMyExp = ((TYPE)1.0)/(MyExp((TYPE)(-i)));
          printf("exp(%d) = %g, 1/MyExp(x)=%g, MyExp(x)= %g, MyExpSepPosNeg(x) = %g \n",
                   i, TrueVal, OneOverMyExp, MyExpVal, MyExpValPosNeg);
        }
        else{
          printf("exp(%d) = %g, MyExp(x)= %g \n", i, TrueVal, MyExpVal);
        }
      }
   }
}
