/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**            
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "complex" to "d_complex" to avoid collsion with 
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 1000
# define MAXITER 1000

void testpoint(double, double);

// Mandelbrot set computed through an iterative process 
//
//    z_n = z_(n-1) squared plus C 
//
// z and C are complex.  Start with z_0 = 0.  The set occupies the 
// complex plane where each (x,y) point is (Creal,Cimaginary)


int numoutside = 0;

int main(){
   int i, j;
   double area, error, eps  = 1.0e-5;
   double Creal;
   double Cimag;

//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
//   testing each point to see whether it is inside or outside the set.

#pragma omp parallel for private(Creal,Cimag,j)
   for (i=0; i<NPOINTS; i++) {
     for (j=0; j<NPOINTS; j++) {
       Creal = -2.0+2.5*(double)(i)/(double)(NPOINTS)+eps;
       Cimag = 1.125*(double)(j)/(double)(NPOINTS)+eps;
       testpoint(Creal, Cimag);
     }
   }

// Calculate area of set and error estimate and output the results
   
area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
   error=area/(double)NPOINTS;

   printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);
   printf("Correct answer should be around 1.510659\n");

}

void testpoint(double Creal, double Cimag){

// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set

       double zr, zi; 
       int iter;
       double temp;

       zr = Creal;     
       zi = Cimag;
       for (iter=0; iter<MAXITER; iter++){
         temp = (zr*zr)-(zi*zi)+Creal;
         zi = zr*zi*2+Cimag;
         zr = temp;
         if ((zr*zr+zi*zi)>4.0) {
           #pragma omp critical
              numoutside++;
           break;
         }
       }
}

