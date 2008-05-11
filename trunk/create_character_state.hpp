#ifndef __create_character_state_hpp__
#define __create_character_state_hpp__

#include "game.hpp"
#include "universe.hpp"
#include "player.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "on_planet_state.hpp"

namespace exastris
{
  struct Create_Character_State : State
  {
    Create_Character_State(Game &t_game)
      : State(t_game, "Create Character")
    {
    }

    virtual std::vector<Action> get_current_actions()
    {
      std::vector<Action> actions;

      actions.push_back(
	  Action("Name", 2,
	    boost::shared_ptr<Action::InputType>(
	      new Action::String(2, 20, m_game.get_player().get_name()))));


      std::vector<std::pair<std::string, double> > values
	= m_game.get_player().get_stats().get_values();


      int total = 0;

      for (unsigned int i = 0; i < values.size(); ++i)
      {
	total += round(values[i].second * 100);
      }

      std::stringstream ss;
      ss << "Create Character (" << 100-total << " points available)";
      set_description(ss.str());

      for (unsigned int i = 0; i < values.size(); ++i)
      {
	actions.push_back(
	    Action(values[i].first, i + 3,
	      boost::shared_ptr<Action::InputType>(
		new Action::Integer(1, round(100-(total-(values[i].second*100))), round(values[i].second * 100)))));
      }

      if (total == 100)
      {
	actions.push_back(
	    Action("Start Game", 1, boost::shared_ptr<Action::InputType>
	      (new Action::None())));
      }

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      switch (t_a.m_id)
      {
	case 1:
	  return boost::shared_ptr<State>(new On_Planet_State(m_game));
	  break;

	case 2:
	  m_game.get_player().set_name(dynamic_cast<const exastris::Action::String &>(*t_a.m_type).m_value);
	  break;
	default:
	  m_game.get_player().get_stats()[m_game.get_player().get_stats().get_names()[t_a.m_id-3]] 
	    = double(dynamic_cast<const exastris::Action::Integer &>(*t_a.m_type).m_value)/100;
	  break;
      };


      return boost::shared_ptr<State>(new Create_Character_State(m_game));
    }
  };
}


#endif
