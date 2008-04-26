#ifndef __universe_hpp__
#define __universe_hpp__

#include "galaxy.hpp"
#include "random_generator.hpp"
#include <cassert>
namespace exastris
{
  class Universe
  {
    public:
      Universe(int seed)
	: m_seed(seed),
	  m_num_galaxies(random_scalar(get_seed(m_seed, 0), 3, 10)),
	  m_universe_modifier(get_seed(m_seed, 1), .75, 1.25)
      {
      }

      Galaxy get_galaxy(int index)
      {
	assert(index < m_num_galaxies && index >= 0);
        return Galaxy(get_seed(m_seed, 2 + index), m_universe_modifier);
      }

      int num_galaxies()
      {
	return m_num_galaxies;
      }


    private:
      const int m_seed;
      const int m_num_galaxies;

      Planetary_Stats m_universe_modifier;
  };
}
#endif
