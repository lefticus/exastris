#ifndef __game_hpp__
#define __game_hpp__

#include "universe.hpp"
#include "player.hpp"

namespace exastris
{
  class Game
  {
    public:
      Game(int t_seed, const std::string &t_player_name)
	: m_universe(t_seed),
          m_player(t_player_name, m_universe.random_location())
      {
      }

      const Player &get_player()
      {
	return m_player;
      }

      const Universe &get_universe()
      {
	return m_universe;
      }

      bool move_to(const Location &t_loc)
      {
	Location curloc = m_player.get_location();
	if (curloc.first != t_loc.first)
	{ 
	  return false;
	} else {
	  Planet curplanet = m_universe.get_galaxy(curloc.first).get_planet(curloc.second);
          Planet newplanet = m_universe.get_galaxy(t_loc.first).get_planet(t_loc.second);

	  int distance = curplanet.distance(newplanet);

	  if (distance <= m_player.get_fuel_level())
	  {
	    m_player.use_fuel(distance);
	    m_player.set_location(t_loc);
	    return true;
	  } else {
	    return false;
	  }

	}
      }

    private:
      Universe m_universe;
      Player m_player;
  };
}


#endif
