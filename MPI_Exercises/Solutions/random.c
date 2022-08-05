/*******************************************************************

NAME:    random

PURPOSE: This is a low quality, simple and hopefully reproducible
         random number generator. DO NOT USE THIS if you want HIGH 
		 QUALITY PSEUDO-RANDOM numbers.  It uses a simple linear 
		 congruential algorithm with poor statistical properties.. 

         The Random numbers are uniformly distributed between 
		 input low and high long values.  The comments within the 
		 program show you how to turn this into uniform deviates
		 between 0.0 and 1.0.
		 
USAGE:   long l_random()  returns the next pseudo-random number 
                        in the sequence. 

         void s_l_random(long seed, long lo, long hi) set the 
		                seed  Sets the seed and lower/upper  
						bounds for the nubers.
  
REF:     I took this algorithm and the input parameters from 
         Numerical recipies (of course).
		 
HISTORY: Written by Tim Mattson, April 1, 1999.
*******************************************************************/

#ifdef TEST
#include <stdio.h>
#endif

long last    = 0;
long low_val = 0;
long hi_val  = 1000;

// This is a carefully selected set of paremters designed to produce
// a reasonably good sequence.  Change them with care.
#define modulus     259200
#define multiplier  7141
#define increment   54773 

/******************************************************************
**  s_l_random:    set computation parameters
******************************************************************/
void s_l_random( long in_seed,     // Seed (between the bounds)
	      long in_low_val,  // Lower bound for the sequence
	      long in_hi_val)   // Upper bound for the sequence
{
	
	// Set bounds
	if (in_low_val > in_hi_val) {
		hi_val = in_low_val;
		low_val = in_hi_val; 
	}
	else if (in_low_val < in_hi_val) {
		low_val = in_low_val;
		hi_val  = in_hi_val;
	}

	// Set the seed forcing it to be within the bounds
	if (in_seed < low_val) last = low_val;
	else if (in_seed > hi_val)  last = hi_val;
	else last = in_seed;
}

/******************************************************************
**  l_random:     return next random number
******************************************************************/
long l_random()
{
    extern long last;
    last = (last*multiplier + increment)%modulus;
    return low_val + (hi_val - low_val+1)*last/modulus;
}

/*
//
// to get a Uniform deviate between 0 and 1, change return value
// of random() to the required floating point type and change the
// return statement to:
//
//       return (double)last/(double)modulus;
//  
*/
#ifdef TEST

/* test the random number generator */
void main(int argc, char **argv)
{
  long seed = 7;
  long lo   = 2;
  long hi   = 15;
  long count = 50;
  int i;
  long temp;
  
  s_l_random( seed, lo, hi);

  printf("\n %d random numbs between %d and %d\n",count,lo,hi);

  for(i=0; i<count; i++){
      if(i%5==0)printf("\n");
	  temp = l_random();
	  printf(" %8d ",temp);
  }
  printf("\n");

}
#endif
