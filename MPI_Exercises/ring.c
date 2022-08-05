/*******************************************************************

NAME:    ring

PURPOSE: This MPI program measures communication times for
         shifting messages around a ring of nodes (or processes).  
         This is repeated for a number of iterations and timing 
         statistics are compiled.
  
USAGE:   Program input is the length of the message to shift
         around the ring and the number of times to carry out 
         the shift.  Input is taken from  the command line:

               ring.exe buffer_count shifts

         where buffer_count is the number of doubles to use in the
         message.
  
         The output consists of minimum and maximum times used
         by the nodes in the ring.  The aggregate bandwidth
         (based on the maximum time) is also reported.

         Compile with DEBUG defined if you want lots of output.

HISTORY: Written by Tim Mattson, April 1999.  
  
*******************************************************************/
#include "ring.h"
#define debug 1
#define MB_CONV  1.0e-6              /* convert bytes to Megabytes */

// support functions (included at the end of this file) //
void out_results(double, double, int, int, int);
void test_clear(double *, double *, int, int, int);

int main(int argc, char **argv)
{
  int num_shifts = 0;  /* number of times to shift the message */
  int buff_count = 0;  /* number of doubles in the message     */
  int num_procs  = 0;  /* number of processes in the ring      */
  int my_ID;           /* process (or node) id                 */
  int buff_size_bytes; /* message size in bytes                */
  int i;

  double t0;           /* Wall Time in Seconds                  */
  double ring_time;    /* ring com Time - on this node          */
  double min_time;     /* ring com Time - min time for all nodes*/
  double max_time;     /* ring com Time - max time for all nodes*/
  double *x;           /* The outgoing message                  */
  double *incoming;    /* The incoming message                  */

  MPI_Status stat;

/*********************************************************************
** Initialize the MPI environment
*********************************************************************/
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_ID);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if (my_ID == 0) 
     printf("\nMPI Ring Test on %d nodes",num_procs);

#ifdef DEBUG
  printf("\n %d after MPI_Init and MPI_Comm_rank\n",my_ID);
  fflush(stdout);
#endif

/*********************************************************************
** process, test and broadcast input parameters
*********************************************************************/
  if(my_ID == 0){
     if (argc == 3){
        buff_count = atoi(*++argv);
        num_shifts = atoi(*++argv);
     }
     else{
        printf("Usage: %s <size of message> <Num of shifts> \n",*argv);
        printf("Program will proceed with defaults\n");
        buff_count = 100;
        num_shifts = 10;
     }


     printf(": shift %d doubles %d times \n",buff_count, num_shifts);
     fflush(stdout);

      /*  Test Input */
     if(num_procs < 1) {
        printf("ERROR: Must have at least 1 process: %d \n",num_procs);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }
     if(buff_count< 0){
        printf("ERROR: must use 0 or more doubles: %d \n",buff_count);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }
     if(num_shifts < 1){
        printf("ERROR: num_shifts must be >= 1 : %d \n",num_shifts);
        fflush(stdout);
        MPI_Abort (MPI_COMM_WORLD, 1);
     }

  }

  /*  Broadcast benchmark data to all processes */
  MPI_Bcast (&buff_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast (&num_shifts, 1, MPI_INT, 0, MPI_COMM_WORLD);

#ifdef DEBUG
  printf("\n ID = %d , nodes  = %d ",my_ID,num_procs);
  printf("buff_count=%d , num_shifts=%d \n",buff_count,num_shifts);
  fflush(stdout);
#endif

/*********************************************************************
** Allocate space and fill the outgoing ("x") and "incoming" vectors.
*********************************************************************/
  buff_size_bytes = buff_count * sizeof(double);
  if ( (x = (double*)malloc(buff_size_bytes)) == False){
    printf(" %d Could not malloc X for %d doubles\n",my_ID, buff_count);
    fflush(stdout);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  if ( (incoming = (double*)malloc(buff_size_bytes)) == False){
    printf(" %d Could not malloc incoming for %d doubles\n",my_ID, buff_count);
    fflush(stdout);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  for(i=0;i<buff_count;i++){
     x[i]   = (double) i;
     incoming[i] = -1.0;
  }

  MPI_Barrier(MPI_COMM_WORLD);
#ifdef DEBUG
  printf("\n Node %d is beyond the barrier\n",my_ID);
  fflush(stdout);
#endif

/**********************************************************************
** Do the ring communication tests.  
** 
** In each case, we call it once to set everything up, and then call 
** it a second time to collect timing data.
*********************************************************************/

  /***********************
  ** The Naive Ring Test 
  ***********************/                    
  if(buff_count < 1001 ) {   /*  only call the ring_naive */
                             /*  test for small messages. */
     ring_naive(x,incoming,buff_count,num_procs,num_shifts,my_ID);
     MPI_Barrier(MPI_COMM_WORLD);

     t0 = MPI_Wtime();
     ring_naive(x,incoming,buff_count,num_procs,num_shifts,my_ID);
     ring_time = MPI_Wtime() - t0;

#ifdef DEBUG
  printf("\n Node %d has finished with the ring_naive call \n",my_ID);
  fflush(stdout);
#endif

      /*
      ** Analyze results 
      */
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Reduce(&ring_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0, 
                                                  MPI_COMM_WORLD);
      MPI_Reduce(&ring_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, 
                                                  MPI_COMM_WORLD);

      if(my_ID == 0){
        printf("\n\n Naive Ring Test Results "); 
        out_results(min_time, max_time, num_procs, buff_count, 
                                                      num_shifts);
      }
  }

/*********************************************************************
** Wait for end and then shut down MPI
*********************************************************************/
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

}  /* end of main */

///////********  Start Support functions ********///////////

/*******************************************************************

NAME:    out_results

PURPOSE: This simple C-functions takes the input from the ring test
         programs, and outputs the desired results. 
  
*******************************************************************/


void out_results(
        double min_time,    /* minimum time for ring test */
        double max_time,    /* maximum time for ring test */
        int num_nodes,      /* number of nodes in the ring */
        int size,           /* size of the message passed around the ring */
        int num_shifts)     /* number of times the message is shifted */
{
    double bandwidth;    /* bandwidth */

#ifdef DEBUG
    printf("\nRING: %d nodes, length = %d,",num_nodes,size);
    printf(" %d shifts:",num_shifts);
#endif
    printf("\n Times:  %f to %f secs, ",min_time,max_time);
    bandwidth = (double)(num_shifts * num_nodes * size *sizeof(double));
    bandwidth = MB_CONV * bandwidth / max_time;
    printf(" Bandwidth = %5.4f megabytes/sec \n",bandwidth);
}

/*******************************************************************

NAME:    test_clear

PURPOSE: Function to test that the right values were passed. After
         the test, the "input" buffer is cleared to make way for 
         the next shift around the ring.
  
USAGE:   Pass in two buffers containing "buff_count" doubles and
         make sure they are equal element by element to within TOL.

         Compile with VERBOSE defined if you want output detailing
         precisely where the two buffers disagree.

*******************************************************************/

#include "ring.h"

void test_clear( 
           double *x,         /* the buffer sent */
           double *in,        /* incoming buffer */
           int    buff_count, /* size of buffer  */
           int    i_shift,    /* index identifying which shift */ 
           int    id)         /* node id */
{
  int j;
  double diff;

  for(j=0,diff=0.0; j<buff_count; j++){
    diff += dabs(x[j] - in[j]);

#ifdef VERBOSE
      if(diff > TOL){
       printf("\nerror x[ %d ] = %f, in[ %d ] = %f \n",j,x[j],j,in[j]);
       fflush(stdout);
      }  
#endif
    in[j] = 0.0;
  }
#ifdef VERBOSE
  printf("\n TEST: shift %d on node %d: error = %f\n",i_shift,id,diff);
#else
  if (diff > TOL)
  printf("\n TEST: shift %d on node %d: error = %f\n",i_shift,id,diff);
#endif
  fflush(stdout);
}

///////********  END Support functions ********///////////
