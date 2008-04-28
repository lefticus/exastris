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
	  m_planet_stats(Planetary_Stats(get_seed(m_initial_seed, 0), 0.0, 1.0) * ps),
	  m_x(random_scalar(random_generator(m_seed), 0.0, 1.0)),
	  m_y(random_scalar(random_generator(m_seed), 0.0, 1.0)),
	  m_size(random_scalar(random_generator(m_seed), .003, .005))
      {
      }

      double distance(const Planet &p)
      {
	return sqrt( pow(fabs(p.m_x - m_x), 2) + pow(fabs(p.m_y - m_y), 2));
      }

      const unsigned int m_initial_seed;
      unsigned int m_seed;

      const Planetary_Stats m_planet_stats;

      const double m_x;
      const double m_y;
      const double m_size;
  };
}
#endif
