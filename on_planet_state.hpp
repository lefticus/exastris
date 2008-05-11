#ifndef __on_planet_state_hpp__
#define __on_planet_state_hpp__

#include "game.hpp"
#include "universe.hpp"
#include "player.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace exastris
{
  struct On_Planet_State : State
  {
    On_Planet_State(Game &t_game);

    virtual std::vector<Action> get_current_actions();

    virtual boost::shared_ptr<State> perform_action(const Action &t_a);
  };
}


#endif
