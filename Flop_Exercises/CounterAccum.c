/*

This program looks at the floating point number 0.01

It's very simple ... just accumulate a counter with 0.01 
and see what you get.

History: Written by Tim Mattson, 09/2023.

*/

#include <stdio.h>
#define time_step  0.01f

float CountTime(int Count)
{
    float sum     = 0.0f;
    
    for (int i=0; i<Count;i++)
        sum += time_step;
        
    return sum;
}

int main()
{

   int Count = 100000;
   float time_val;
   
   time_val = CountTime(Count);
   printf(" sum = %f or %f\n",time_val,time_step*Count);

}
