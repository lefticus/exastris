#ifndef __galaxy_hpp__
#define __galaxy_hpp__

#include "planet.hpp"
#include <cassert>

namespace exastris
{
  class Galaxy
  {
    public:
      struct No_Planet_Found : std::runtime_error
      {
	No_Planet_Found()
	  : std::runtime_error("Invalid index or location for planet")
	{
	}
      };

      Galaxy(const Mersenne_Twister &mt, const Planetary_Stats &ps)
	: m_mersenne_twister(mt),
	  m_num_planets(m_mersenne_twister.next(500, 1250)),
	  m_galaxy_modifier(Planetary_Stats(
		m_mersenne_twister.indexed_sequence(1), 0.75, 1.25) * ps)
      {
      }

      Planet get_planet(int index)
      {
	if (index >= m_num_planets || index < 0)
	{
	  throw No_Planet_Found();
	}
        return Planet(m_mersenne_twister.indexed_sequence(2 + index), 
	    m_galaxy_modifier, index);
      }

      Planet get_planet(double x, double y)
      {
	for (int i = 0; i < m_num_planets; ++i)
	{
	  Planet p = get_planet(i);
	  if (p.located_at(x, y))
	  {
	    return p;
	  }
	}

	throw No_Planet_Found();
      }

      int num_planets()
      {
	return m_num_planets;
      }

    private:
      Mersenne_Twister m_mersenne_twister;
      const int m_num_planets;

      Planetary_Stats m_galaxy_modifier;
  };
}
#endif
