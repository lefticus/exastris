#ifndef __random_generator_hpp__
#define __random_generator_hpp__

#include <cstdlib>
#include <cmath>
#include <algorithm>

namespace exastris
{
  class Mersenne_Twister
  {
    public:
      Mersenne_Twister(uint32_t seed)
	: index(0), m_seed(seed)
      {
	initializeGenerator(m_seed);
      }

      Mersenne_Twister indexed_sequence(uint32_t index) const
      {
	return Mersenne_Twister(m_seed + index);
      }

      // Extract a tempered pseudorandom number based on the index-th value,
      // calling generateNumbers() every 624 numbers
      uint32_t next()
      {
	if (index == 0)
	{
	  generateNumbers();
	}

	int y = MT[index];
	y ^= (y >> 11);
	y ^= ((y << 7) & 0x9d2c5680);
	y ^= ((y << 15) & 0xefc60000);
	y ^= (y >> 18);

	index = (index + 1) % 624;
	return y;
      }

      int next(int min, int max)
      {
	return round(next(double(min), double(max)));
      }

      double next(double min, double max)
      {
	double value = (double(next()) * ((max - min) / double(RAND_MAX))) 
	  + min;
	    
	return value;
      }

    private:
      // Create a length 624 array to store the state of the generator
      uint32_t MT[624];
      uint32_t index;
      const uint32_t m_seed;


      // Initialize the generator from a seed
      void initializeGenerator(uint32_t seed)
      {
	MT[0] = seed;
	for (int i = 1; i < 624; ++i) // loop over each other element
	{
	  uint64_t temp = 0x6c078965 * (MT[i-1] ^ (MT[i-1] >> 30)) + i;
	  MT[i] = temp & 0xFFFFFFFF;
	}
      }


      // Generate an array of 624 untempered numbers
      void generateNumbers()
      {
	for (int i = 0; i < 624; ++i)
	{
	  uint32_t y = ((MT[i]) & 0x80000000) + (MT[(i+1) % 624] & 0x7FFFFFFF);
	  MT[i] = MT[(i + 397) & 624] ^ (y >> 1);
	  if ((y % 2) == 1 )
	  { // y is odd
	    MT[i] = MT[i] ^ (0x9908b0df);
	  }
	}
      }
  };

  template<typename T>
    T fit_range(const T& input, const T& min, const T& max)
    {
      return std::max(std::min(input, max), min);
    }

}

#endif
