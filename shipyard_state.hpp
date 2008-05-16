#ifndef __shipyard_state_hpp__
#define __shipyard_state_hpp__

#include "game.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "on_planet_state.hpp"

namespace exastris
{
  struct Shipyard_State : State
  {
    Shipyard_State(Game &t_game)
      : State(t_game, "Shipyard")
    {
      std::stringstream ss;
      ss << "Shipyard: " << m_game.get_player().get_fuel_level()
	<< " units of fuel in vechicle. " << m_game.get_player().get_money() << " credits available";
      set_description(ss.str());
    }

    virtual std::vector<Action> get_current_actions()
    {
      std::vector<Action> actions;

      std::stringstream ss;
      ss << "Buy Fuel (" << std::min(m_game.get_price_of_fuel() * 
	m_game.get_player().get_available_fuel_capacity(), 
	 m_game.get_player().get_money())
	<< " cr)";
      actions.push_back(
	  Action(ss.str(), 0, boost::shared_ptr<Action::InputType>
	    (new Action::None())));
      actions.push_back(
	  Action("Back", 1, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      switch (t_a.m_id)
      {
	case 0:
	  m_game.get_player().fill_up(m_game.get_price_of_fuel());
	  return boost::shared_ptr<State>(new Shipyard_State(m_game));
	case 1:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
	  break;
	default:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
      };
    }
  };
}


#endif
