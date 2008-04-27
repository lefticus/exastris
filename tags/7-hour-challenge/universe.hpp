#ifndef __universe_hpp__
#define __universe_hpp__

#include "galaxy.hpp"
#include "random_generator.hpp"
#include <cassert>
namespace exastris
{
  typedef std::pair<int, int> Location;

  class Universe
  {
    public:
      Universe(int seed)
	: m_initial_seed(seed),
	  m_seed(seed),
	  m_num_galaxies(random_scalar(random_generator(m_seed), 3, 10)),
	  m_universe_modifier(get_seed(m_initial_seed, 1), .75, 1.25)
      {
      }

      Galaxy get_galaxy(int index) const
      {
	assert(index < m_num_galaxies && index >= 0);
        return Galaxy(get_seed(m_initial_seed, 2 + index), m_universe_modifier);
      }

      int num_galaxies() const
      {
	return m_num_galaxies;
      }

      Location random_location()
      {
        Location l;
	l.first = round(random_scalar(random_generator(m_seed), 0, m_num_galaxies - 1));
	l.second = round(random_scalar(random_generator(m_seed), 0, get_galaxy(l.first).num_planets() - 1));

	return l;
      }

    private:
      const unsigned int m_initial_seed;
      unsigned int m_seed;
      const int m_num_galaxies;

      Planetary_Stats m_universe_modifier;
  };
}
#endif
