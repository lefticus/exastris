#ifndef __planetary_stats_hpp__
#define __planetart_stats_hpp__

#include "random_generator.hpp"

namespace exastris
{
  class Planetary_Stats
  {
    public:
      Planetary_Stats(const Mersenne_Twister &mt, double min, double max)
	: m_mersenne_twister(mt),
	  m_red(m_mersenne_twister.next(min, max)),
	  m_green(m_mersenne_twister.next(min, max)),
      	  m_blue(m_mersenne_twister.next(min, max)),
      	  m_technology_level(m_mersenne_twister.next(min, max)),
      	  m_governmental_control_level(m_mersenne_twister.next(min, max))
      {
      }

      Planetary_Stats operator*(const Planetary_Stats &ps)
      {
	return Planetary_Stats(
	    m_mersenne_twister,
	    fit_range(ps.m_red * m_red, 0.0, 1.0),
	    fit_range(ps.m_green * m_green, 0.0, 1.0),
	    fit_range(ps.m_blue * m_blue, 0.0, 1.0),
	    fit_range(ps.m_technology_level * m_technology_level, 0.0, 1.0),
	    fit_range(ps.m_governmental_control_level * m_governmental_control_level, 0.0, 1.0)
	  );
      }

      Mersenne_Twister m_mersenne_twister;
      unsigned int m_seed;

      const double m_red;
      const double m_green;
      const double m_blue;

      const double m_technology_level;
      const double m_governmental_control_level;

    private:
      Planetary_Stats(
	  const Mersenne_Twister &mt,
	  double t_red, double t_green, double t_blue,
	  double t_technology_level, double t_governmental_control_level)
	: m_mersenne_twister(mt),
	  m_red(t_red),
	  m_green(t_green),
	  m_blue(t_blue),
	  m_technology_level(t_technology_level),
	  m_governmental_control_level(t_governmental_control_level)
      {
      }

  };
}
#endif
