/*

     This program tests a few specific cases used
     in the Floating Point Arithmetic  slides.  In particular
     they highlight the lack of associativity and the
     fact the distributive law doesn't hold in general.

*/

#include<stdio.h>
int main(){

    float a, b, c;

// explore associativity of floating point arithmetic
    a = 0.7f;   b= 0.1f;   c = 0.3f;

    printf(" (a + b) + c = %f\n",(a+b)+c);
    printf(" a + (b + c) = %f\n",(a+b)+c);

    a = 1.e20f;  b= -1.e20f;   c = 1.0f;

    printf(" (a + b) + c = %f\n",(a+b)+c);
    printf(" a + (b + c) = %f\n",(a+b)+c);
    
    a = 11111113;  b= -11111113;   c = 7.51111111f;
    printf("\n a = 11111113;  b= -11111113;   c = 7.51111111f;\n");
    printf(" (a + b) + c = %f\n",(a+b)+c);
    printf("  a + (b + c) = %f\n",a+(b+c));
    printf(" correct answer = 7.51111111\n");

// explore a case where a simple decimal number
// cannot be represented in binary
     b = 1000.2f;     
     c = b - 1000.0f;
     printf(" for b = 1000.2, (b - 1000.0) = %f\n",c);

// explore a case where the distributive law does not hold 
    a= 20000; b= -6; c=6.0000003;
    printf("\n a= 20000; b= -6; c=6.0000003;\n");
    printf(" (a*b + a*c) = %f\n",(a*b + a*c));
    printf("  a*(b + c) = %f\n",a*(b + c));
    printf(" correct answer = 0.006000\n");

}
