#ifndef __player_hp__
#define __player_hp__

#include "universe.hpp"
#include <string>

namespace exastris
{
  class Player
  {
    public:
      class Stats
      {
	private:
          typedef std::map<std::string, double> Map_Type;

	public:
	  struct unknown_stat : std::runtime_error
	  {
	    unknown_stat(const std::string &name)
	      : std::runtime_error("Unknown requested stat: " + name)
	    {
	    }
	  };

	  double &operator[](const std::string &stat)
	  {
	    Map_Type::iterator itr = m_stats.find(stat);
	    if (itr == m_stats.end())
	    {
	      throw unknown_stat(stat);
	    } else {
	      return itr->second;
	    }
	  }

	  const double &operator[](const std::string &stat) const
	  {
	    Map_Type::const_iterator itr = m_stats.find(stat);
	    if (itr == m_stats.end())
	    {
	      throw unknown_stat(stat);
	    } else {
	      return itr->second;
	    }
	  }

	  void add(const std::string &stat, double initial_value)
	  {
	    m_stats.insert(std::make_pair(stat, initial_value));
	  }

	  std::vector<Map_Type::key_type> get_names() const
	  {
            std::vector<Map_Type::key_type> names;

	    for (Map_Type::const_iterator itr = m_stats.begin();
		 itr != m_stats.end();
		 ++itr)
	    {
	      names.push_back(itr->first);
	    }

	    return names;
	  }

	  std::vector<std::pair<std::string, double> > get_values() const
	  {
	    return std::vector<std::pair<std::string, double> >(m_stats.begin(), m_stats.end());
	  }

	private:
	  Map_Type m_stats;
      };

      Player(const std::string &t_name, const Location &t_location)
	: m_name(t_name),
          m_location(t_location),
	  m_fuel_capacity(.1),
	  m_fuel_level(.1),
	  m_money(10)
      {
	m_stats.add("Weaponry", .1);
	m_stats.add("Piloting", .1);
      }

      void set_name(const std::string &s)
      {
	m_name = s;
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

      Stats &get_stats()
      {
	return m_stats;
      }

      const Stats &get_stats() const
      {
	return m_stats;
      }



    private:
      std::string m_name;
      Location m_location;

      double m_fuel_capacity;
      double m_fuel_level;

      int m_money;

      Stats m_stats;
  };
}

#endif
