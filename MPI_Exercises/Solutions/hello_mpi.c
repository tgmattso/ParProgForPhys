#include "mpi.h"
#include <stdio.h>
int main (int argc, char **argv)
{
   int i, ID, Nprocs;
   char name[MPI_MAX_PROCESSOR_NAME];
   int namLen;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &ID);
   MPI_Comm_size(MPI_COMM_WORLD, &Nprocs);
   printf(" hello from %d of %d\n",ID, Nprocs);
   
   MPI_Get_processor_name(name,&namLen);
   printf("%s  %d\n",name,namLen);
   
   MPI_Finalize();
}	  





