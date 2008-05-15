#ifndef __player_hpp__
#define __player_hpp__

#include "universe.hpp"
#include "wares.hpp"
#include <string>
#include <map>
#include <vector>

namespace exastris
{
  class Player
  {
    public:
      struct Ware
      {
        double m_average_cost;
        int m_quantity;
      };

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
	  m_fuel_capacity(.08),
	  m_fuel_level(.08),
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

      void sell_wares(const Ware_For_Purchase &t_wfp, int quantity)
      {
	std::map<std::string, Ware>::iterator itr = 
	  m_wares.find(t_wfp.m_name);

	assert(itr != m_wares.end());
	assert(itr->second.m_quantity >= quantity);
	
	m_money += t_wfp.m_price_per_unit * quantity;
	itr->second.m_quantity -= quantity;
      }

      void purchase_wares(const Ware_For_Sale &t_wfs, int quantity)
      {
	Ware &ware = m_wares[t_wfs.m_name];

	assert(quantity <= t_wfs.m_quantity_available);
        assert(t_wfs.m_price_per_unit * quantity <= m_money);

	m_money -= t_wfs.m_price_per_unit * quantity;

	ware.m_average_cost = ((ware.m_average_cost * ware.m_quantity) + (t_wfs.m_price_per_unit * quantity))/(ware.m_quantity + quantity);

	ware.m_quantity += quantity;
      }

      double get_fuel_level() const
      {
	return m_fuel_level;
      }

      double distance_able_to_travel() const
      {
	return m_fuel_level;
      }

      std::vector<std::pair<std::string, Ware> > get_owned_wares()
      {
	return std::vector<std::pair<std::string, Ware> >
	  (m_wares.begin(), m_wares.end());
      }

      void use_fuel(double t_fuel)
      {
	m_fuel_level -= t_fuel;
      }

      Stats &get_stats()
      {
	return m_stats;
      }

      const Stats &get_stats() const
      {
	return m_stats;
      }

      double get_money() const
      {
	return m_money;
      }



    private:
      std::string m_name;
      Location m_location;

      double m_fuel_capacity;
      double m_fuel_level;

      double m_money;

      std::map<std::string, Ware> m_wares;

      Stats m_stats;
  };
}

#endif
