/*******************************************************************

NAME:    trans_utility.h

PURPOSE: Include file to support my matrix transpose programs

HISTORY: Written by Tim Mattson, April 1999.  
  
*******************************************************************/
void transpose(                  /*  transpose a local matrix*/
   double* A,    int Acols,      /* input matrix */
   double* B,    int Bcols,      /* transposed mat */
   int sub_rows, int sub_cols);  /* size of slice to  transpose */

void outmat(     /* output a matrix)
   double* buff, /* The base address of the matrix to output */
   int Nrows,    /* The number of rows in the matrix */
   int Ncols);   /* The number of columns in the matirx */
   
double test_results (
   int , 
   int , 
   int , 
   double*);

void  trans_snd_rcv   (
   double*, 
   double*, 
   int, 
   double*, 
   int, 
   int);

void   trans_isnd_ircv (
   double*, 
   double*, 
   int, 
   double*, 
   int,   
   int);

void   trans_sendrcv  (
   double*, 
   double*, 
   int, 
   double*, 
   int,
   int);

void fill_mat(
   int , 
   int , 
   int , 
   double*);

void fill_garbage( 
   int , 
   int , 
   int , 
   double*);

void output_timings (
   char *, 
   int , 
   int , 
   double , 
   double, 
   double, 
   int, 
   int , 
   int);

void analyze_results (
   int, 
   double, 
   int, 
   double*, 
   double*, 
   double*, 
   int*, 
   int*, 
   int*);

