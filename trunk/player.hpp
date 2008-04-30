#ifndef __player_hp__
#define __player_hp__

#include "universe.hpp"
#include <string>

namespace exastris
{
  class Player
  {
    public:

      Player(const std::string &t_name, const Location &t_location)
	: m_name(t_name),
          m_location(t_location),
	  m_fuel_capacity(.1),
	  m_fuel_level(.1),
	  m_money(10)
      {
      }

      std::string get_name() const
      {
	return m_name;
      }

      Location get_location() const
      {
	return m_location;
      }

      void set_location(const Location &t_loc)
      {
	m_location = t_loc;
      }

      double get_fuel_level() const
      {
	return m_fuel_level;
      }

      void use_fuel(double t_fuel)
      {
	m_fuel_level -= t_fuel;
      }

      void fill_up()
      {
	m_fuel_level = m_fuel_capacity;
      }



    private:
      std::string m_name;
      Location m_location;

      double m_fuel_capacity;
      double m_fuel_level;

      int m_money;
  };
}

#endif
