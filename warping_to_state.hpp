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
        std::stringstream ss;
        ss << "At " << m_warp_encounters[0].m_num_clicks << " clicks to the next panet\n";
        ss << "you encounter a ";
        if (m_warp_encounters[0].m_type == Warp_Encounter::Police)
        {
          ss << "police";
        } else {
          ss << "pirate";
        }

        ss << " vessel.\n";

        if (!m_warp_encounters[0].m_detected)
        {
          ss << "The approaching vessel has not detected you";
          actions.push_back(
              Action("Ignore", 1, boost::shared_ptr<Action::InputType>
                (new Action::None())));
        } else {
          actions.push_back(
              Action("Flee", 2, boost::shared_ptr<Action::InputType>
                (new Action::None())));
        }


        set_description(ss.str());
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

	  return boost::shared_ptr<State>(new Warping_To_State(m_game, m_on_route_to, Warp_Encounters(++m_warp_encounters.begin(),m_warp_encounters.end())));
      };
    }

    private:
      Planet m_on_route_to;
      Warp_Encounters m_warp_encounters;
  };
}


#endif
