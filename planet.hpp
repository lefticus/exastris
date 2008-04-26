#ifndef __planet_hpp__
#define __planet_hpp__

#include "planetary_stats.hpp"
#include <cmath>


namespace exastris
{
  class Planet
  {
    public:
      Planet(int seed, const Planetary_Stats &ps)
	: m_initial_seed(seed),
	  m_seed(seed),
	  m_planet_stats(Planetary_Stats(get_seed(m_initial_seed, 0), 0.0, 255.0) * ps),
	  m_x(round(round_to(random_scalar(random_generator(m_seed), 0.0, 85.0), 5)) * 3),
	  m_y(round(round_to(random_scalar(random_generator(m_seed), 0.0, 85.0), 5)) * 3)
      {
      }

      int distance(const Planet &p)
      {
	return round(sqrt( pow(fabs(p.m_x - m_x), 2) + pow(fabs(p.m_y - m_y), 2)));
      }

      unsigned int m_initial_seed;
      unsigned int m_seed;

      const Planetary_Stats m_planet_stats;

      const int m_x;
      const int m_y;
  };
}
#endif
