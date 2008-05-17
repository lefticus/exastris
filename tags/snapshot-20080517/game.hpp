#ifndef __game_hpp__
#define __game_hpp__

#include "universe.hpp"
#include "player.hpp"
#include "wares.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>

namespace exastris
{
  class Game;


  struct Action
  {
    struct InputType
    {
      virtual std::string value_as_string() const = 0;
      virtual ~InputType() {}
    };

    struct None : InputType
    {
      virtual std::string value_as_string() const { return ""; }
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

      virtual std::string value_as_string() const { return m_value; }
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

      virtual std::string value_as_string() const
      { 
	std::stringstream ss;
	ss << m_value; 
	return ss.str();
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

      virtual std::string value_as_string() const
      { 
	std::stringstream ss;
	ss << m_value; 
	return ss.str();
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
    std::string value_as_string() const
    {
      return m_type->value_as_string();
    }
    boost::shared_ptr<InputType> m_type;
  };

  struct State
  {
    State(Game &t_game, const std::string &t_description)
      : m_game(t_game),
      m_description(t_description)
    {
    }

    virtual boost::shared_ptr<State> perform_action(const Action &t_action) = 0;
    virtual std::vector<Action> get_current_actions() = 0;


    virtual ~State() {}

    Game &m_game;

    std::string get_description()
    {
      return m_description;
    }

    void set_description(const std::string &t_desc)
    {
      m_description = t_desc;
    }

    private:

    std::string m_description;
  };


  class Game
  {
    public:
      Game(int t_seed);

      Player &get_player();

      const Universe &get_universe();
      std::vector<Action> get_current_actions();

      std::string get_current_state_description();
      void perform_action(const Action &a);

      Galaxy get_current_galaxy();
      Planet get_planet(const Location &t_loc);
      bool move_to(const Location &t_loc);

      std::vector<Ware_For_Sale> get_wares_for_sale();
      std::vector<Ware_For_Purchase> get_wares_for_purchase();

      Planet get_selected_planet();
      void set_selected_planet(const Location &t_loc);
      double get_price_of_fuel() const;
      double get_ware_price(const std::string &t_name, bool purchase) const;

      Planet get_current_planet() const;

      void purchase_wares(const Ware_For_Sale &t_wfs, int quantity);

      std::vector<std::pair<double, Location> > get_planets_in_range();

    private:
      Universe m_universe;
      Player m_player;
      Location m_selected_planet;
      double m_distancetraveled;

      std::map<std::string, int> m_wares_purchased;
      boost::shared_ptr<State> m_current_state;
  };
}


#endif
