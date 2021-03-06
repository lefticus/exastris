
#include "universe.hpp"
#include "player.hpp"
#include "game.hpp"
#include "begin_game_state.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>

namespace exastris
{
  Game::Game(int t_seed)
    : m_random_number_generator(t_seed),
      m_universe(m_random_number_generator),
      m_player("Jameson", m_universe.random_location()),
      m_distancetraveled(0),
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
    Planet p = get_current_planet();
    std::vector<Ware_For_Sale> wares;

    const int max = 20;
    const Planetary_Stats &pstats = p.m_planet_stats;

    const int ore = pstats.m_red * max;
    const int food = pstats.m_green * max;
    const int water = pstats.m_blue * max;

    wares.push_back(Ware_For_Sale("Ore", get_ware_price("Ore", false), ore-m_wares_purchased["Ore"]));
    wares.push_back(Ware_For_Sale("Food", get_ware_price("Food", false), food-m_wares_purchased["Food"]));
    wares.push_back(Ware_For_Sale("Water", get_ware_price("Water", false), water-m_wares_purchased["Water"])) ;
    return wares;
  }

  double Game::get_ware_price(const std::string &t_name, bool purchase) const
  {
    const Planetary_Stats &pstats = get_current_planet().m_planet_stats;
    const int nominalvalue = 10;

    double baseprice (0); 

    if (t_name == "Ore")
    {
      baseprice = (1 - ((pstats.m_red * 2 - 1) * .8)) * nominalvalue;
    } else if (t_name == "Food") {
      baseprice = (1 - ((pstats.m_green * 2 - 1) * .8)) * nominalvalue;
    } else if (t_name == "Water") {
      baseprice = (1 - ((pstats.m_blue * 2 - 1) * .8)) * nominalvalue;
    } else {
      baseprice = 0;
    }

    double negotiating_skill = m_player.get_stats()["Negotiating"];

    double negotiation_modifier = (1 - negotiating_skill) * baseprice * .5;

    if (purchase)
    {
      return baseprice - negotiation_modifier;
    } else {
      return baseprice + negotiation_modifier;
    }
  }

  Warp_Encounters Game::get_warp_encounters(const Planet &t_p)
  {
    Planet curplanet = get_current_planet();
    int clicks = t_p.distance(curplanet) * 150;
    std::cout << clicks << " clicks traveled" << std::endl;

    Warp_Encounters encounters;

    const double navigation_skill = m_player.get_stats()["Navigation"];

    for (int i = 1; i <= clicks; ++i)
    {
      const double gvmtcontrol = (curplanet.m_planet_stats.m_governmental_control_level * (clicks - i) + t_p.m_planet_stats.m_governmental_control_level * i)/clicks;

      const bool policeencounter = m_random_number_generator.next(0.0, 1.0) < gvmtcontrol;
      const bool pirateencounter = m_random_number_generator.next(0.0, 1.0) > gvmtcontrol;
      const bool detected = m_random_number_generator.next(0.0, 1.0) > navigation_skill;

      std::cout << i << " clicks from next planet policeencounter: " << policeencounter <<
        " pirateencounter: " << pirateencounter << " govmtcontrollevel " << gvmtcontrol << " detectedbyothership: " << detected << std::endl;

      if (policeencounter)
        encounters.push_back(Warp_Encounter(Warp_Encounter::Police, detected, clicks - i));
      else
        encounters.push_back(Warp_Encounter(Warp_Encounter::Pirate, detected, clicks - i));
    }

    return encounters;
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
	    get_ware_price(itr->first, true), itr->second.m_average_cost, itr->second.m_quantity));
    }

    return wares;
  }

  std::vector<std::pair<double, Location> > Game::get_planets_in_range()
  {
    std::vector<std::pair<double, Location> > planets;
    Location curloc = m_player.get_location();
    Galaxy curgal = m_universe.get_galaxy(curloc.first);
    Planet curp = curgal.get_planet(curloc.second);

    double distance_can_travel = m_player.distance_able_to_travel();

    for (int i = 0; i < curgal.num_planets(); ++i)
    {
      Planet p = curgal.get_planet(i);
      double distance = p.distance(curp);
      if (distance <= distance_can_travel)
      {
        planets.push_back(std::make_pair(distance, Location(curloc.first, i)));
      }
    }

    return planets;
  }

  Galaxy Game::get_current_galaxy()
  {
    return m_universe.get_galaxy(m_player.get_location().first);
  }

  Planet Game::get_planet(const Location &t_loc)
  {
    return m_universe.get_galaxy(t_loc.first).get_planet(t_loc.second);
  }

  Planet Game::get_selected_planet()
  {
    return m_universe.get_galaxy(m_selected_planet.first)
      .get_planet(m_selected_planet.second);
  }

  void Game::purchase_wares(const Ware_For_Sale &t_wfs, int quantity)
  {
    m_player.purchase_wares(t_wfs, quantity);
    m_wares_purchased[t_wfs.m_name] += quantity;
  }
 
  Planet Game::get_current_planet() const
  {
    Location l = m_player.get_location();
    return m_universe.get_galaxy(l.first).get_planet(l.second);
  }

  void Game::set_selected_planet(const Location &t_loc)
  {
    m_selected_planet = t_loc;
  }

  double Game::get_price_of_fuel() const
  {
    return 75;
  }

  bool Game::move_to(const Planet &t_p)
  {
    return move_to(Location(m_player.get_location().first, t_p.get_number()));
  }

  bool Game::move_to(const Location &t_loc)
  {
    m_selected_planet = t_loc;
    Location curloc = m_player.get_location();
    if (curloc.first != t_loc.first)
    { 
      return false;
    } else {
      Planet curplanet = m_universe.get_galaxy(curloc.first).get_planet(curloc.second);
      Planet newplanet = m_universe.get_galaxy(t_loc.first).get_planet(t_loc.second);

      double distance = curplanet.distance(newplanet);
      m_distancetraveled += distance;

      if (distance <= m_player.get_fuel_level())
      {
	m_player.use_fuel(distance);
	m_player.set_location(t_loc);
	m_wares_purchased.clear();
	return true;
      } else {
	return false;
      }

    }
  }
}

