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
	  m_fuel_capacity(10),
	  m_fuel_level(10)
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

      int get_fuel_level()
      {
	return m_fuel_level;
      }

      void use_fuel(int t_fuel)
      {
	m_fuel_level -= t_fuel;
      }



    private:
      std::string m_name;
      Location m_location;

      int m_fuel_capacity;
      int m_fuel_level;

  };
}

#endif
