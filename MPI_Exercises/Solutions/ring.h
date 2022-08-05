/*******************************************************************

NAME:    ring.h

PURPOSE: include file for the MPI ring tests.

HISTORY: Written by Tim Mattson, April 1999
*******************************************************************/

#include "mpi.h"
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define IS_ODD(x)  ((x)%2)           /* test for an odd int */
#define dabs(x) ((x)>0?(x):(-(x)))   /* absolute value */

#define False 0
#define True  1
#define TOL   0.001       /*  Tolerance used in floating point compares */
#define MB_CONV  1.0e-6   /*  conversion factor ... bytes to Megabytes */

