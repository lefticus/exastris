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
	  Action("Back", 0, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      std::vector<Ware_For_Sale> wares = m_game.get_wares_for_sale();

      int i = 1;
      for (std::vector<Ware_For_Sale>::const_iterator itr = wares.begin();
	   itr != wares.end();
	   ++itr, ++i)
      {
	std::stringstream desc;
	desc << itr->m_name << " (" << itr->m_price_per_unit << " ea)";
	actions.push_back(
	    Action(desc.str(), i,
	      boost::shared_ptr<Action::InputType>(
		new Action::Integer(0, itr->m_quantity_available,
		  itr->m_quantity_available))));
      }

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      switch (t_a.m_id)
      {
	case 0:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
	  break;
	default:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
      };
    }
  };
}


#endif
