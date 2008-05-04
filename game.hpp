#ifndef __game_hpp__
#define __game_hpp__

#include "universe.hpp"
#include "player.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace exastris
{
  class Game
  {
    public:
      struct Action
      {
	struct InputType
	{
	  virtual ~InputType() {}
	};

	struct None : InputType
	{
	  virtual ~None() {}
	};

	struct String : InputType
	{
	  String(int t_minlength, int t_maxlength,
	      const std::string &t_defaultvalue)
	    : m_minlength(t_minlength),
	      m_maxlength(t_maxlength),
	      m_value(t_defaultvalue)
	  {
	  }

	  const int m_minlength;
	  const int m_maxlength;

	  std::string m_value;

	  virtual ~String() {}
	};

	template<typename T>
	  struct Range : InputType
	{
	  Range(T t_minvalue, T t_maxvalue, T t_defaultvalue)
	    : m_minvalue(t_minvalue),
	      m_maxvalue(t_maxvalue),
	      m_value(t_defaultvalue)
	  {
	  }

	  const T m_minvalue;
	  const T m_maxvalue;

	  T m_value;

	  virtual ~Range() {}
	};

	typedef Range<int> Integer;
	typedef Range<double> Float;

	template<typename T>
	struct List : InputType
	{
	  List(const std::vector<T> t_possiblevalues,
	      const T &t_defaultvalue)
	    : m_possiblevalues(t_possiblevalues),
	      m_value(t_defaultvalue)
	  {
	  }

	  const std::vector<T> m_possiblevalues;
	  T m_value;

	  virtual ~List() {}
	};

	typedef List<std::string> StringEnumeration;

	Action(const std::string &t_name,
	    int t_id, 
	    boost::shared_ptr<InputType> t_type)
	  : m_name(t_name),
	    m_id(t_id),
	    m_type(t_type)
	{
	}


	std::string m_name;
	int m_id;
	boost::shared_ptr<InputType> m_type;
      };

      struct State
      {
	State(Game &t_game)
	  : m_game(t_game)
	{
	}

	virtual boost::shared_ptr<State> perform_action(const Action &t_action) = 0;
	virtual std::vector<Action> get_current_actions() = 0;


	virtual ~State() {}

	Game &m_game;
      };

      struct Begin_Game_State : State
      {
	Begin_Game_State(Game &t_game)
	  : State(t_game)
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

      struct Create_Character_State : State
      {
	Create_Character_State(Game &t_game)
	  : State(t_game)
	{
	}

	virtual std::vector<Action> get_current_actions()
	{
	  std::vector<Action> actions;

	  actions.push_back(
	      Action("Change Name (" + m_game.get_player().get_name() + ")", 1,
	      boost::shared_ptr<Action::InputType>(
		new Action::String(2, 20, m_game.get_player().get_name()))));

	  actions.push_back(
	      Action("Set Piloting", 2,
	      boost::shared_ptr<Action::InputType>(
		new Action::Integer(1, 10, m_game.get_player().get_piloting()))));

	  return actions;
	}

	virtual boost::shared_ptr<State> perform_action(const Action &t_a)
	{
	  switch (t_a.m_id)
	  {
	    case 1:
	      m_game.get_player().set_name(dynamic_cast<const exastris::Game::Action::String &>(*t_a.m_type).m_value);
	      break;
	    case 2:
	      m_game.get_player().set_piloting(dynamic_cast<const exastris::Game::Action::Integer &>(*t_a.m_type).m_value);
	      break;
	  };

	  
	  return boost::shared_ptr<State>(new Create_Character_State(m_game));
	}
      };

      Game(int t_seed)
	: m_universe(Mersenne_Twister(t_seed)),
          m_player("Jameson", m_universe.random_location()),
	  m_current_state(new Begin_Game_State(*this))
      {
      }

      Player &get_player()
      {
	return m_player;
      }

      const Universe &get_universe()
      {
	return m_universe;
      }

      std::vector<Action> get_current_actions()
      {
	return m_current_state->get_current_actions();

      }

      void perform_action(const Action &a)
      {
        m_current_state = m_current_state->perform_action(a);
      }


      Galaxy get_current_galaxy()
      {
	return m_universe.get_galaxy(m_player.get_location().first);
      }

      bool move_to(const Location &t_loc)
      {
	Location curloc = m_player.get_location();
	if (curloc.first != t_loc.first)
	{ 
	  return false;
	} else {
	  Planet curplanet = m_universe.get_galaxy(curloc.first).get_planet(curloc.second);
          Planet newplanet = m_universe.get_galaxy(t_loc.first).get_planet(t_loc.second);

	  double distance = curplanet.distance(newplanet);

	  if (distance <= m_player.get_fuel_level())
	  {
	    m_player.use_fuel(distance);
	    m_player.set_location(t_loc);
	    return true;
	  } else {
	    return false;
	  }

	}
      }

    private:
      Universe m_universe;
      Player m_player;

      boost::shared_ptr<State> m_current_state;
  };
}


#endif
