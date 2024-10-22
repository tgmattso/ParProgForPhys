//  The utility functions used in the summation program follow: 

void seed(float, float);                  // seed pseudorandom number generator
float frandom();                          // Get next pseudorandom in the sequence
void FillSequence(int, float*, float*);   // fill sequence (>0), return "true" sum
float KahanSum(int, float*);              // Kahan Sum of a sequence
int comp(const void *, const void *);     // comparator func for qsort

