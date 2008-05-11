#ifndef __buy_goods_state_hpp__
#define __buy_goods_state_hpp__

#include "game.hpp"
#include "universe.hpp"
#include "player.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "on_planet_state.hpp"

namespace exastris
{
  struct Buy_Goods_State : State
  {
    Buy_Goods_State(Game &t_game)
      : State(t_game, "Buy Goods")
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
