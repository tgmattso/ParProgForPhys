//
//  This is a very simple version of ping pong ... along the lines
//  of what I'd expect a student to write in an introduction to MPI
//  tutorial.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define VAL 42
#define NREPS  10
#define TAG 5
int main(int argc, char **argv)
{

   int rank, size;
   double t0;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if(size <2){
      printf("too few ranks\n");
      exit(-1);
   }

   int bsend = VAL;
   int brecv = 0;
   MPI_Status stat;
   if(rank == 0) t0 = MPI_Wtime();

   for(int i=0;i<NREPS; i++){
      if(rank == 0){
        MPI_Send(&bsend, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(&brecv, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &stat);
        if(brecv != VAL)printf("error: interation %d %d != %d\n",i,brecv,VAL);
        brecv = 0;
      }
      else if(rank == 1){
        MPI_Recv(&brecv, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
        MPI_Send(&bsend, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        if(brecv != VAL)printf("error: interation %d %d != %d\n",i,brecv,VAL);
        brecv = 0;
      }
   }
   if(rank == 0){
      double t = MPI_Wtime() - t0;
      double lat = t/(2*NREPS);
      printf(" lat = %f seconds\n",(float)lat);
   }
   MPI_Finalize();
}



 
