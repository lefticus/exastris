#ifndef __planet_hpp__
#define __planet_hpp__

#include "planetary_stats.hpp"
#include <cmath>


namespace exastris
{
  class Planet
  {
    public:
      Planet(const Mersenne_Twister &mt, const Planetary_Stats &ps, int number)
	: m_mersenne_twister(mt),
	  m_planet_stats(Planetary_Stats(
		m_mersenne_twister.indexed_sequence(1), 0.0, 1.0) * ps),
	  m_x(m_mersenne_twister.next(0.0, 1.0)),
	  m_y(m_mersenne_twister.next(0.0, 1.0)),
	  m_size(m_mersenne_twister.next(.003, .005)),
	  m_number(number)
      {
      }

      double distance(double x1, double y1, double x2, double y2)
      {
	return sqrt(pow(fabs(x1 - x2), 2) + pow(fabs(y1 - y2), 2));
      }

      double distance(const Planet &p)
      {
	return distance(m_x, m_y, p.m_x, p.m_y);
      }

      bool located_at(double x, double y)
      {
	return distance(x, y, m_x, m_y) < m_size;
      }

      int get_number()
      {
	return m_number;
      }

      Mersenne_Twister m_mersenne_twister;

      const Planetary_Stats m_planet_stats;

      const double m_x;
      const double m_y;
      const double m_size;

      const int m_number;
  };
}
#endif
