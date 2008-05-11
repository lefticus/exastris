#include "on_planet_state.hpp"
#include "buy_goods_state.hpp"
#include "sell_goods_state.hpp"
#include "shipyard_state.hpp"
#include "warp_state.hpp"
#include "characters_state.hpp"

namespace exastris
{
  On_Planet_State::On_Planet_State(Game &t_game)
    : State(t_game, "On Planet")
  {
  }

  std::vector<Action> On_Planet_State::get_current_actions()
  {

    //      std::stringstream ss;
    //      ss << "Create Character (" << 100-total << " points available)";
    //      set_description(ss.str());

    std::vector<Action> actions;

    actions.push_back(
	Action("Buy Goods", 1, boost::shared_ptr<Action::InputType>
	  (new Action::None())));
    actions.push_back(
	Action("Sell Goods", 2, boost::shared_ptr<Action::InputType>
	  (new Action::None())));
    actions.push_back(
	Action("Shipyard", 3, boost::shared_ptr<Action::InputType>
	  (new Action::None())));
    actions.push_back(
	Action("Warp", 4, boost::shared_ptr<Action::InputType>
	  (new Action::None())));
    actions.push_back(
	Action("Characters", 5, boost::shared_ptr<Action::InputType>
	  (new Action::None())));

    return actions;
  }

  boost::shared_ptr<State> On_Planet_State::perform_action(const Action &t_a)
  {
    switch (t_a.m_id)
    {
      case 1:
	return boost::shared_ptr<State>(new Buy_Goods_State(m_game));
	break;

      case 2:
	return boost::shared_ptr<State>(new Sell_Goods_State(m_game));
	break;

      case 3:
	return boost::shared_ptr<State>(new Shipyard_State(m_game));
	break;

      case 4:
	return boost::shared_ptr<State>(new Warp_State(m_game));
	break;

      case 5:
	return boost::shared_ptr<State>(new Characters_State(m_game));
	break;

      default:
	return boost::shared_ptr<State>(new On_Planet_State(m_game));
    };
  }
}

