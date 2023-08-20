#include <stdio.h>
#include <stdlib.h>

#define NX 100
#define LEFTVAL 1.0
#define RIGHTVAL 10.0
#define NSTEPS 10000

void initialize(double uk[], double ukp1[]) {
    uk[0] = LEFTVAL; uk[NX-1] = RIGHTVAL;
    for (int i = 1; i < NX-1; ++i)
        uk[i] = 0.0;
    for (int i = 0; i < NX; ++i)
        ukp1[i] = uk[i];
}

/* This function won't appear in the book example.   FIX THIS */
void printValues(double uk[], int step) {
#ifndef DEBUG
    if (step < NSTEPS-1) return;
#endif
    printf("values after step %d:\n", step);
    for (int i = 0; i < NX; ++i) {
        printf("uk[%d] = %g\n", i, uk[i]);
    }
}

int main(void) {
    /* pointers to arrays for two iterations of algorithm */
    double *uk = malloc(sizeof(double) * NX);
    double *ukp1 = malloc(sizeof(double) * NX);
    double *temp; 

    double dx = 1.0/NX;
    double dt = 0.5*dx*dx;

    initialize(uk, ukp1);

    for (int k = 0; k < NSTEPS; ++k) {

        /* compute new values */
        for (int i = 1; i < NX-1; ++i) {
          ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
        }

        /* "copy" ukp1 to uk by swapping pointers */
        temp = ukp1; ukp1 = uk; uk = temp;

        printValues(uk, k);
    }
    return 0;
}
