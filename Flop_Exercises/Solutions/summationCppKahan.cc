#include <algorithm>
#include <execution>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>
#include "UtilityFunctions.h"  // where FillSequence comes from

#define N 100'000'000  // length of sequence of numbers to work with

//============================================================================

int main()
{
  std::vector<float> seq(N);  // Sequence to sum
  float True_sum;             // An estimate of the actual sum

  FillSequence(
      N, seq.data(), &True_sum);  // populate seq with N random values > 0

  std::cout << std::setw(20) << "TrueSum: " << std::setprecision(10) << True_sum
            << '\n';

  auto kahan = [correction = 0.f](auto ksum, auto x) mutable {
    auto xcorrected = x - correction;
    auto tmpSum = ksum + xcorrected;
    correction = (tmpSum - ksum) - xcorrected;
    return tmpSum;
  };

  {
    auto sum = std::accumulate(seq.begin(), seq.end(), 0.f);
    std::cout << std::setw(20) << "accumulate basic: " << std::setprecision(10)
              << sum << '\n';
  }
  {
    auto sum = std::accumulate(seq.begin(), seq.end(), 0.f, kahan);
    std::cout << std::setw(20) << "accumulate kahn: " << std::setprecision(10)
              << sum << '\n';
  }
  {
    auto sum = std::reduce(seq.begin(), seq.end(), 0.f);
    std::cout << std::setw(20) << "reduce basic: " << std::setprecision(10)
              << sum << '\n';
  }
  {
    auto sum = std::reduce(seq.begin(), seq.end(), 0.f, kahan);
    std::cout << std::setw(20) << "reduce kahn: " << std::setprecision(10)
              << sum << '\n';
  }
}
