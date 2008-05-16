#ifndef __warp_state_hpp__
#define __warp_state_hpp__

#include "game.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "on_planet_state.hpp"

namespace exastris
{
  struct Warp_State : State
  {
    Warp_State(Game &t_game)
      : State(t_game, "Warp"),
        m_planets_in_range(t_game.get_planets_in_range())
    {
      t_game.set_selected_planet(m_planets_in_range[0].second);
    }

    Warp_State(Game &t_game, const std::vector<std::pair<double, Location> > &t_loc)
      : State(t_game, "Warp"),
        m_planets_in_range(t_loc)
    {
      t_game.set_selected_planet(m_planets_in_range[0].second);
    }

    virtual std::vector<Action> get_current_actions()
    {
      Planet p = m_game.get_planet(m_planets_in_range[0].second);
      std::stringstream ss;
      
      ss << "Planet: " << p.get_number();
      set_description(ss.str());

      std::vector<Action> actions;

      actions.push_back(
	  Action("Previous Planet In Range", 0, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      actions.push_back(
	  Action("Next Planet In Range", 1, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      actions.push_back(
	  Action("Warp To", 2, boost::shared_ptr<Action::InputType>(new Action::None())));

      actions.push_back(
	  Action("Back", 3, boost::shared_ptr<Action::InputType>
	    (new Action::None())));

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      switch (t_a.m_id)
      {
	case 0:
	  if (m_planets_in_range.size() > 1)
	  {
	    std::vector<std::pair<double, Location> >::iterator itr = 
	      m_planets_in_range.end();
	    --itr;

	    std::rotate(m_planets_in_range.begin(),
		itr,
		m_planets_in_range.end());
	  }
	  return boost::shared_ptr<State>(new Warp_State(m_game, m_planets_in_range));

	case 1:
	  if (m_planets_in_range.size() > 1)
	  {
	    std::vector<std::pair<double, Location> >::iterator itr = 
	      m_planets_in_range.begin();
	    ++itr;

	    std::rotate(m_planets_in_range.begin(),
		itr,
		m_planets_in_range.end());
	  }
	  return boost::shared_ptr<State>(new Warp_State(m_game, m_planets_in_range));

	case 2:
	  m_game.move_to(m_planets_in_range[0].second);
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));

	case 3:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));

	default:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
      };
    }

    private:
      std::vector<std::pair<double, Location> > m_planets_in_range;
  };
}


#endif
