#ifndef __begin_game_state_hpp__
#define __begin_game_state_hpp__

#include "universe.hpp"
#include "player.hpp"
#include "game.hpp"
#include "create_character_state.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace exastris
{

  struct Begin_Game_State : State
  {
    Begin_Game_State(Game &t_game)
      : State(t_game, "Begin Game")
    {
    }

    virtual std::vector<Action> get_current_actions()
    {
      std::vector<Action> actions;

      actions.push_back(
	  Action("Create Character", 1,
	    boost::shared_ptr<Action::InputType>(new Action::None())));

      return actions;
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_a)
    {
      return boost::shared_ptr<State>(new Create_Character_State(m_game));
    }
  };

}


#endif
