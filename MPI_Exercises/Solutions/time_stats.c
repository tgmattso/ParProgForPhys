/****************************************************************

Name:      time_stats

Purpose:   Computes and prints various timeing statistics in units
           of milliseconds and mebegabytes/second

Arguments: timings     - input double array of times.
           iterations  - input number of iterations in timing study.
           size_bytes  - message size (one way) in bytes.


****************************************************************/

#define VERBOSE       0L     /* set to 1 for lots of output */
#define SEC_TO_MSEC   1000.0 /* secs to millisecs conversion */
#define BYTE_TO_MBYTE 1.0e-6 /* bytes to mbytes conversion */

#include <stdio.h>
#include <math.h>

/***************************************************************
** my_compare: Compare two real numbers.
****************************************************************/
static int my_compare(a1,a2) 
double *a1;
double *a2;                  /* Return an integer as folows:  */
{                          
   if ( *a1 > *a2 )          /*   1 if a1 > a2                */
         return 1; 
   if ( *a1 < *a2 )          /*  -1 if a1 < a2                */
         return -1;
   return 0;                 /*   0 if a1 = a2                */
}

/***************************************************************
** time_stats: Write timeing statistics to standard out.
****************************************************************/
void time_stats(timings, iterations, size_bytes)
int iterations;         /* Number of iterations to time */
double *timings;        /* array of times for each iteration */
int size_bytes;         /* size of message in bytes */
{
   int    min_ind;            /* index to minimum time */
   int    max_ind;            /* index to maximum time */
   double min_time;           /* minimum time */
   double max_time;           /* maximum time */
   double ave_time;           /* average time */
   double std_dev;            /* standard deviation */
   double median;             /* median time */
   double Mbytes;

   double *tp, sum_t, sum_t2;
   int  iters;

/****************************************************************
** Get Mean, Standard deviation, and min/max values.
****************************************************************/
   sum_t  = 0.0;
   sum_t2 = 0.0;
   min_time = max_time = *timings;
   min_ind  = max_ind  = 0;
   for( tp = timings, iters = 0 ; iters < iterations ; tp++,iters++){
      if(iters == 0){
        min_time = *tp;
        min_ind  = iters;          /* Init. minimum and maximum values*/
        min_time = *tp;
        max_ind  = iters;
      }
      else{
        if(*tp < min_time){
           min_time = *tp;         /* Update minimum value */
           min_ind  = iters;
        }
        else if(*tp > max_time){
           max_time = *tp;         /* Update maximum value */
           max_ind  = iters;
        }
      }
      sum_t += *tp;                /* accumulate sum of times   */
      sum_t2 += (*tp)*(*tp);       /* accumulate sum of squares */

      if(VERBOSE)printf( "%d\t%12.5e\n", iters, *tp );
   }
   
   ave_time = sum_t/iterations;
   std_dev = (sum_t2-((sum_t*sum_t)/(double)iterations))/(iterations-1);
   std_dev = sqrt(std_dev); 

/****************************************************************
** Sort to find median
****************************************************************/

   qsort( (char *)timings, (int)iterations, sizeof(double), my_compare);
   median = *(timings+iterations/2);

/****************************************************************
** Output results in milliseconds
****************************************************************/
   printf( "\n\n          Times in Milliseconds \n");
   printf( "Average time : %9.6f\n", ave_time * SEC_TO_MSEC );
   printf( "Standard dev : %9.6f\n", std_dev  * SEC_TO_MSEC );
   printf( "Median  time : %9.6f\n", median   * SEC_TO_MSEC );
   printf( "Minimum time : %9.6f ",  min_time * SEC_TO_MSEC );
   printf( "(%4d)\n",min_ind);
   printf( "Maximum time : %9.6f ",  max_time * SEC_TO_MSEC );
   printf( "(%4d)\n",max_ind);

/****************************************************************
** Output results in Megabytes/second.  Note: the factor of 2
** is needed since times are for round trip times.
****************************************************************/
   printf( "\n\n     Bandwidths in Megabytes/sec  \n");
   Mbytes = 2.0 * BYTE_TO_MBYTE * (double)size_bytes;
   printf( "Average bwidth : %9.3f\n", Mbytes / ave_time );
   printf( "Median  bwidth : %9.3f\n", Mbytes / median   );
   printf( "Minimum bwidth : %9.3f\n", Mbytes / max_time );
   printf( "Maximum bwidth : %9.3f\n", Mbytes / min_time );

}  /* end of stat_time */

