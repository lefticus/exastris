
#include "universe.hpp"
#include "player.hpp"
#include "game.hpp"
#include "begin_game_state.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace exastris
{
  Game::Game(int t_seed)
    : m_universe(Mersenne_Twister(t_seed)),
    m_player("Jameson", m_universe.random_location()),
    m_current_state(new Begin_Game_State(*this))
  {
  }

  Player &Game::get_player()
  {
    return m_player;
  }

  const Universe &Game::get_universe()
  {
    return m_universe;
  }

  std::vector<Action> Game::get_current_actions()
  {
    return m_current_state->get_current_actions();

  }

  std::string Game::get_current_state_description()
  {
    return m_current_state->get_description();
  }

  void Game::perform_action(const Action &a)
  {
    m_current_state = m_current_state->perform_action(a);
  }

  std::vector<Ware_For_Sale> Game::get_wares_for_sale()
  {
    std::vector<Ware_For_Sale> wares;
    wares.push_back(Ware_For_Sale("Ore", .99, 5));
    return wares;
  }

  std::vector<Ware_For_Purchase> Game::get_wares_for_purchase()
  {
    std::vector<Ware_For_Purchase> wares;
    typedef std::vector<std::pair<std::string, Player::Ware> > Player_Wares;

    Player_Wares pwares = m_player.get_owned_wares();

    for (Player_Wares::const_iterator itr = pwares.begin();
	 itr != pwares.end();
	 ++itr)
    {
      wares.push_back(Ware_For_Purchase(itr->first,
	    .98, itr->second.m_average_cost, itr->second.m_quantity));
    }

    return wares;
  }


  Galaxy Game::get_current_galaxy()
  {
    return m_universe.get_galaxy(m_player.get_location().first);
  }

  bool Game::move_to(const Location &t_loc)
  {
    Location curloc = m_player.get_location();
    if (curloc.first != t_loc.first)
    { 
      return false;
    } else {
      Planet curplanet = m_universe.get_galaxy(curloc.first).get_planet(curloc.second);
      Planet newplanet = m_universe.get_galaxy(t_loc.first).get_planet(t_loc.second);

      double distance = curplanet.distance(newplanet);

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
}
