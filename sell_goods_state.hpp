#ifndef __sell_goods_state_hpp__
#define __sell_goods_state_hpp__

#include "game.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "on_planet_state.hpp"

namespace exastris
{
  struct Sell_Goods_State : State
  {
    Sell_Goods_State(Game &t_game)
      : State(t_game, "Sell Goods")
    {
    }

    virtual std::vector<Action> get_current_actions()
    {
      std::vector<Action> actions;

      actions.push_back(
	  Action("Back", -1, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      m_wares = m_game.get_wares_for_purchase();

      int i = 0;
      for (std::vector<Ware_For_Purchase>::const_iterator itr = m_wares.begin();
	   itr != m_wares.end();
	   ++itr, ++i)
      {
	std::stringstream desc;
	desc << itr->m_name << " (" << itr->m_price_per_unit - itr->m_price_paid_per_unit << " ea)";

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
	case -1:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
	  break;
	default:
	  m_game.get_player().sell_wares(
	      m_wares[t_a.m_id],
	      dynamic_cast<const exastris::Action::Integer &>(*t_a.m_type).m_value);
	  return boost::shared_ptr<State>(new Sell_Goods_State(m_game));
      };
    }

    private:
      std::vector<Ware_For_Purchase> m_wares;
  };
}


#endif
