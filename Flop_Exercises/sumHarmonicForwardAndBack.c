#include<stdio.h>
// A harmonic number is one that can be represented
// as a sum over i from i=1 to i=N of 1.0/N
//
// We compute the Nth harmonic number by running the
// sum forwards and backwards.  Mathematically, the 
// order of the sum should not matter.
int main(){

    float sum=0.0f;
    long N = 10000000;

    for(int i= 1;i<N;i++){
       sum += 1.0/(float)i;
    }
    printf(" %ld'th Harmonic number, sum forward  = %14.8f\n",N,sum);

    sum = 0.0f;
    for(int i= N-1;i>=1;i--){
       sum += 1.0/(float)i;
    }
    printf(" %ld'th Harmonic number, sum backward = %14.8f\n",N,sum);
}
