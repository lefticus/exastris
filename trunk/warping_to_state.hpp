#ifndef __warping_to_state_hpp__
#define __warping_to_state_hpp__

#include "game.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "on_planet_state.hpp"

namespace exastris
{
  struct Warping_To_State : State
  {
    Warping_To_State(Game &t_game, const Planet &t_on_route_to)
      : State(t_game, "Warp"),
        m_on_route_to(t_on_route_to),
        m_warp_encounters(t_game.get_warp_encounters(m_on_route_to))
    {
    }

    Warping_To_State(Game &t_game, const Planet &t_on_route_to, 
        const Warp_Encounters &t_warp_encounters)
      : State(t_game, "Warp"),
        m_on_route_to(t_on_route_to),
        m_warp_encounters(t_warp_encounters)
    {
    }

    virtual std::vector<Action> get_current_actions()
    {
      std::vector<Action> actions;

      if (m_warp_encounters.empty())
      {
        set_description("You have arrived on the planet");
        actions.push_back(
            Action("OK", 0, boost::shared_ptr<Action::InputType>
              (new Action::None())));
      } else {
        /*
        actions.push_back(
            Action("Next Planet In Range", 1, boost::shared_ptr<Action::InputType>
              (new Action::None())));

        actions.push_back(
            Action("Warp To", 2, boost::shared_ptr<Action::InputType>(new Action::None())));

        actions.push_back(
            Action("Back", 3, boost::shared_ptr<Action::InputType>
              (new Action::None())));
              */
      }

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      switch (t_a.m_id)
      {
	case 0:
	  m_game.move_to(m_on_route_to);
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));

	default:
	  return boost::shared_ptr<State>(new Warping_To_State(m_game, m_on_route_to, m_warp_encounters));
      };
    }

    private:
      Planet m_on_route_to;
      Warp_Encounters m_warp_encounters;
  };
}


#endif
