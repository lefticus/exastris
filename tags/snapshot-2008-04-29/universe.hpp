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
      Universe(const Mersenne_Twister &mt)
	: m_mersenne_twister(mt),
	  m_num_galaxies(m_mersenne_twister.next(3, 10)),
	  m_universe_modifier(m_mersenne_twister.indexed_sequence(1), .75, 1.25)
      {
      }

      Galaxy get_galaxy(int index) const
      {
	assert(index < m_num_galaxies && index >= 0);
        return Galaxy(m_mersenne_twister.indexed_sequence(index + 2), 
	      m_universe_modifier);
      }

      int num_galaxies() const
      {
	return m_num_galaxies;
      }

      Location random_location()
      {
        Location l;
	l.first = m_mersenne_twister.next(0, 
	      m_num_galaxies - 1);
	l.second = m_mersenne_twister.next(0, 
	      get_galaxy(l.first).num_planets() - 1);

	return l;
      }

    private:
      Mersenne_Twister m_mersenne_twister;
      const int m_num_galaxies;


      Planetary_Stats m_universe_modifier;
  };
}
#endif
