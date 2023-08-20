#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define NX 100
#define LEFTVAL 1.0
#define RIGHTVAL 10.0
#define NSTEPS 10000

/* This function won't appear in the book example.   FIX THIS */
/* uk, ukp1 include "ghost cell" at each end */
void initialize(double uk[], double ukp1[], int numPoints, 
        int numProcs, int myID) {
    for (int i = 1; i <= numPoints; ++i)
        uk[i] = 0.0;
    /* left endpoint */
    if (myID == 0)
        uk[1] = LEFTVAL;
    /* right endpoint */
    if (myID == numProcs-1)
        uk[numPoints] = RIGHTVAL;
    /* copy values to ukp1 */
    for (int i = 1; i <= numPoints; ++i)
        ukp1[i] = uk[i];
}

/* This function won't appear in the book example.   It is ugly sloppy
   code but was fast to write.  FIX THIS */
#define OUTFILENAMEFMT "OUT.%04d"
FILE* outfile;
void printValues(double uk[], int step, int numPoints, int myID) {
    if (step == 0) {
        char name[strlen(OUTFILENAMEFMT)+1];
        snprintf(name, sizeof(name), OUTFILENAMEFMT, myID);
        outfile = fopen(name, "w");
    }
#ifndef DEBUG
    if (step < NSTEPS-1) return;
#endif
    fprintf(outfile, "values after step %d:\n", step);
    for (int i = 1; i <= numPoints; ++i) {
        fprintf(outfile, "uk[%d] = %g\n", (i-1) + myID*numPoints, uk[i]);
    }
    if (step == NSTEPS-1) {
        fclose(outfile);
    }
}

int main(int argc, char *argv[]) {
    /* pointers to arrays for two iterations of algorithm */
    double *uk;
    double *ukp1;
    double *temp;

    double dx = 1.0/NX;
    double dt = 0.5*dx*dx;

    int numProcs, myID, leftNbr, rightNbr;
    int numPoints;
    MPI_Status status; 

    /* MPI initialization */
    MPI_Init(&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    /* FIX THIS -- remove for book? */
    if ((NX % numProcs) != 0) {
        fprintf(stderr, "Number of processes must evenly divide %d\n", NX);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* initialization of other variables */
    leftNbr = myID - 1; // ID of left "neighbor" process
    rightNbr = myID + 1; // ID of right "neighbor" process
    numPoints = (NX / numProcs);
    /* uk, ukp1 include a "ghost cell" at each end */
    uk = malloc(sizeof(double) * (numPoints+2));
    ukp1 = malloc(sizeof(double) * (numPoints+2));

    initialize(uk, ukp1, numPoints, numProcs, myID);

    for (int k = 0; k < NSTEPS; ++k) {

        /* exchange boundary information */
        if (myID != 0) 
            MPI_Send(&uk[1], 1, MPI_DOUBLE, leftNbr, 0, MPI_COMM_WORLD);
        if (myID != numProcs-1)
            MPI_Send(&uk[numPoints], 1, MPI_DOUBLE, rightNbr, 0, MPI_COMM_WORLD);
        if (myID != 0)
            MPI_Recv(&uk[0], 1, MPI_DOUBLE, leftNbr, 0, MPI_COMM_WORLD, &status);
        if (myID != numProcs-1)
            MPI_Recv(&uk[numPoints+1],1, MPI_DOUBLE, rightNbr, 0, MPI_COMM_WORLD,
                    &status);

        /* compute new values for interior points */
        for (int i = 2; i < numPoints; ++i) {
          ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
        }
        /* compute new values for boundary points */
        if (myID != 0) {
            int i=1;
            ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
        }
        if (myID != numProcs-1) {
            int i=numPoints;
            ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
        }

        /* "copy" ukp1 to uk by swapping pointers */
        temp = ukp1; ukp1 = uk; uk = temp;

        printValues(uk, k, numPoints, myID);
    }
    
    /* clean up and end */
    MPI_Finalize();
    return 0;
}
