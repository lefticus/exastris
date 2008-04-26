#ifndef __random_generator_hpp__
#define __random_generator_hpp__

#include <cstdlib>
#include <algorithm>

namespace exastris
{
  int get_seed(unsigned int starterseed, unsigned int index)
  {
    unsigned int seed = starterseed + index;
    return rand_r(&seed);
  }

  int random_generator(unsigned int &seed)
  {
    return rand_r(&seed);
  }

  double random_scalar(int number, double min, double max)
  {
    return (double(number) * ((max - min) / double(RAND_MAX))) + min;
  }

  double round_to(double number, int prec)
  {
    int num = number * 10;
    int mod = num % prec;
    if (mod < prec / 2)
    {
      num -= mod;
    } else {
      num += (prec - mod);
    }

    return double(num)/10;
  }

  template<typename T>
    T fit_range(const T& input, const T& min, const T& max)
    {
      return std::max(std::min(input, max), min);
    }

}

#endif
