#ifndef __planet_hpp__
#define __planet_hpp__

#include "planetary_stats.hpp"
#include <cmath>


namespace exastris
{
  class Planet
  {
    public:
      Planet(const Mersenne_Twister &mt, const Planetary_Stats &ps)
	: m_mersenne_twister(mt),
	  m_planet_stats(Planetary_Stats(
		m_mersenne_twister.indexed_sequence(1), 0.0, 1.0) * ps),
	  m_x(m_mersenne_twister.next(0.0, 1.0)),
	  m_y(m_mersenne_twister.next(0.0, 1.0)),
	  m_size(m_mersenne_twister.next(.003, .005))
      {
      }

      double distance(const Planet &p)
      {
	return sqrt( pow(fabs(p.m_x - m_x), 2) + pow(fabs(p.m_y - m_y), 2));
      }

      Mersenne_Twister m_mersenne_twister;

      const Planetary_Stats m_planet_stats;

      const double m_x;
      const double m_y;
      const double m_size;
  };
}
#endif
