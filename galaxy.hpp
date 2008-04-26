#ifndef __galaxy_hpp__
#define __galaxy_hpp__

#include "planet.hpp"
#include <cassert>

namespace exastris
{
  class Galaxy
  {
    public:
      Galaxy(int seed, const Planetary_Stats &ps)
	: m_seed(seed),
	  m_num_planets(random_scalar(get_seed(m_seed, 0), 750, 1500)),
	  m_galaxy_modifier(Planetary_Stats(get_seed(m_seed, 1), 0.75, 1.25) * ps)
      {
      }

      Planet get_planet(int index)
      {
	assert(index < m_num_planets && index >= 0);
        return Planet(get_seed(m_seed, 2 + index), m_galaxy_modifier);
      }

      int num_planets()
      {
	return m_num_planets;
      }

    private:
      const int m_seed;
      const int m_num_planets;

      Planetary_Stats m_galaxy_modifier;
  };
}
#endif
