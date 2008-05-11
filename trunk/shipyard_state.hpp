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
    }

    virtual std::vector<Action> get_current_actions()
    {
      std::vector<Action> actions;

      actions.push_back(
	  Action("Back", 1, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      switch (t_a.m_id)
      {
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
