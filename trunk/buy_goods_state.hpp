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

      std::stringstream ss;
      ss << "Buy Goods (" << m_game.get_player().get_money() << " credits available)";
      set_description(ss.str());


      actions.push_back(
	  Action("Back", -1, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      m_wares = m_game.get_wares_for_sale();

      int i = 0;
      for (std::vector<Ware_For_Sale>::const_iterator itr = m_wares.begin();
	   itr != m_wares.end();
	   ++itr, ++i)
      {
	std::stringstream desc;
	desc << itr->m_name << " (" << itr->m_price_per_unit << " ea)";

	int number_which_can_be_purchased=
	  std::min(itr->m_quantity_available,
	      int(m_game.get_player().get_money()/itr->m_price_per_unit));

	actions.push_back(
	    Action(desc.str(), i,
	      boost::shared_ptr<Action::InputType>(
		new Action::Integer(0, number_which_can_be_purchased,
		  number_which_can_be_purchased))));
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
	  m_game.get_player().purchase_wares(
	      m_wares[t_a.m_id],
	    dynamic_cast<const exastris::Action::Integer &>(*t_a.m_type).m_value);
	  return boost::shared_ptr<State>(new Buy_Goods_State(m_game));
      };
    }

    private:
      std::vector<Ware_For_Sale> m_wares;
  };
}


#endif
