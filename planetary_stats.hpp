#ifndef __planetary_stats_hpp__
#define __planetart_stats_hpp__

#include "random_generator.hpp"

namespace exastris
{
  class Planetary_Stats
  {
    public:
      Planetary_Stats(unsigned int seed, double min, double max)
	: m_seed(seed),
	  m_red(random_scalar(random_generator(m_seed), min, max)),
	  m_green(random_scalar(random_generator(m_seed), min, max)),
      	  m_blue(random_scalar(random_generator(m_seed), min, max)),
      	  m_technology_level(random_scalar(random_generator(m_seed), min, max)),
      	  m_governmental_control_level(random_scalar(random_generator(m_seed), min, max))
      {
      }

      Planetary_Stats operator*(const Planetary_Stats &ps)
      {
	return Planetary_Stats(
	    fit_range(ps.m_red * m_red, 0.0, 255.0),
	    fit_range(ps.m_green * m_green, 0.0, 255.0),
	    fit_range(ps.m_blue * m_blue, 0.0, 255.0),
	    fit_range(ps.m_technology_level * m_technology_level, 0.0, 255.0),
	    fit_range(ps.m_governmental_control_level * m_governmental_control_level, 0.0, 255.0)
	  );
      }

      unsigned int m_seed;

      const double m_red;
      const double m_green;
      const double m_blue;

      const double m_technology_level;
      const double m_governmental_control_level;

    private:
      Planetary_Stats(double t_red, double t_green, double t_blue,
	  double t_technology_level, double t_governmental_control_level)
	: m_seed(0),
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
