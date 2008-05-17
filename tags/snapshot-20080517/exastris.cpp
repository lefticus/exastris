//$Id: radar.cc 969 2008-02-23 18:39:51Z murrayc $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * This example was originally provided by Alain Roughe <alain.rouge@enac.fr>
*/


#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <gdkmm/window.h>


#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/window.h>
#include <gtkmm/main.h>

#include <math.h> //Needed by the IRIX MipsPro compiler, for sin().
#include <sstream>
#include <iostream>
#include "game.hpp"
#include <vector>

#include <pangomm/fontdescription.h>

class PPI : public Gtk::DrawingArea
{
public:
  PPI(exastris::Game &);
  virtual ~PPI();
  bool timer_callback();

  sigc::signal< void > signal_game_changed;

protected:
  //Overridden default signal handlers:
  virtual void on_realize();
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_button_press_event(GdkEventButton* event);


private:
  exastris::Game &m_game;
};


PPI::PPI(exastris::Game &t_game)
  : m_game(t_game)
{
  add_events(Gdk::EXPOSURE_MASK|Gdk::BUTTON_PRESS_MASK );
}


PPI::~PPI()
{
}


void PPI::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

}

bool PPI::on_button_press_event(GdkEventButton*e)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  double x = double(e->x)/double(width);
  double y = double(e->y)/double(height);

  try {
    exastris::Planet p = m_game.get_current_galaxy().get_planet(x, y);
    std::cout << "Planet found!" << std::endl;
    m_game.move_to(exastris::Location(m_game.get_player().get_location().first, p.get_number()));
    signal_game_changed.emit();
  } catch (std::exception &e) {
  }
  std::cout << "Button pressed: " << x << " " << y << std::endl;
  return true;
}

bool PPI::on_expose_event(GdkEventExpose*)
{
  // we need a ref to the gdkmm window
  Glib::RefPtr<Gdk::Window> window = get_window();

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  double displayedwidth;
  double displayedheight;

  double range = m_game.get_player().get_fuel_level();
  double displayedrange = range * 3.5;

  if (width < height)
  {
    displayedwidth = displayedrange;
    displayedheight = displayedrange*height/width;
  } else {
    displayedheight = displayedrange;
    displayedwidth = displayedrange*width/height;
  }
  
  std::cout << "Player: " << m_game.get_player().get_name() << " in galaxy: " << m_game.get_player().get_location().first << " at planet: " << m_game.get_player().get_location().second << std::endl;

  exastris::Galaxy g(m_game.get_current_galaxy());

  Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

  exastris::Planet p(g.get_planet(m_game.get_player().get_location().second));

//  cr->scale(width, height);
    cr->scale(double(width)/displayedwidth, double(height)/displayedheight);
    cr->translate(-p.m_x + (displayedwidth/2), -p.m_y + (displayedheight/2));

  cr->save();
  cr->set_source_rgba(0.0,0.0,0.0, 0.9);  
  cr->paint();



  for (int i = 0; i < g.num_planets(); ++i)
  {
    exastris::Planet p = g.get_planet(i);
    cr->set_source_rgba(p.m_planet_stats.m_red, p.m_planet_stats.m_green, p.m_planet_stats.m_blue, 1.0);
    cr->arc(p.m_x, p.m_y, p.m_size, 0.0, 2 * M_PI);
    cr->fill();

  }

  cr->set_line_width(0.003);
  cr->set_source_rgba(1.0, 1.0, 1.0, .5);

  cr->arc(p.m_x, p.m_y, range, 0.0, 2 * M_PI);
  cr->stroke();

  exastris::Planet selectedp = m_game.get_selected_planet();
  cr->set_line_width(0.002);

  cr->arc(selectedp.m_x, selectedp.m_y, 
      selectedp.m_size + cr->get_line_width() / 2, 0.0, 2*M_PI);

  cr->set_source_rgba(1.0, 1.0, 0.0, .75);
  cr->stroke();



  return true;
}



class Radar : public Gtk::Window
{
public:
  Radar();
  virtual ~Radar();
  
protected:
  //signal handlers:
  void on_button1_clicked();
  void on_button2_clicked();
  void on_game_changed();
  void on_cursor_changed();

  // Child widgets
  Gtk::VBox m_box0;
  Gtk::HBox m_box1;
  Gtk::VBox m_box2;
  Gtk::HBox m_box3; //empty box

  Gtk::Entry m_entry;
  Gtk::SpinButton m_spinbutton;
  Gtk::Button m_button1;
  Gtk::Button m_button2;
  Gtk::Label m_label;
  Gtk::ListViewText m_listviewtext;
  exastris::Game m_game;
  Gtk::Statusbar m_sb;

  PPI m_area;
  std::vector<exastris::Action> m_actions;
};


Radar::Radar()
  : m_box0(/*homogeneous*/false, /*spacing*/5), m_box1(false, 5), m_box2(false, 5), m_box3(false, 5), 
    m_entry(),
    m_spinbutton(),
    m_button1("Select"), 
    m_button2("Quit"), 
    m_label(),
    m_listviewtext(2),
    m_game(0),
    m_sb(), 
    m_area(m_game)
{


  // box2
  m_button2.signal_clicked().connect(sigc::mem_fun(*this, &Radar::on_button2_clicked));
  m_button1.signal_clicked().connect(sigc::mem_fun(*this, &Radar::on_button1_clicked));
  m_area.signal_game_changed.connect(sigc::mem_fun(*this, &Radar::on_game_changed));

  m_listviewtext.signal_cursor_changed().connect(sigc::mem_fun(*this, &Radar::on_cursor_changed));
 

  m_box2.pack_start(m_label, Gtk::PACK_SHRINK, 5);
//  m_box2.pack_start(m_box3, /*Gtk::PackOptions*/Gtk::PACK_EXPAND_WIDGET, /*padding*/5);
  m_box2.pack_start(m_listviewtext, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box2.pack_start(m_entry, Gtk::PACK_SHRINK, 5);
  m_box2.pack_start(m_spinbutton, Gtk::PACK_SHRINK, 5);
  m_box2.pack_start(m_button1, Gtk::PACK_SHRINK, 5);
  m_box2.pack_start(m_button2, Gtk::PACK_SHRINK, 5);
  
  // box1
  m_area.set_size_request(300, 300);
  m_listviewtext.set_size_request(300, 100);
  m_box1.pack_start(m_area, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box1.pack_start(m_box2, Gtk::PACK_SHRINK, 5);
    
  // box0
  m_box0.pack_start(m_box1, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box0.pack_start(m_sb, Gtk::PACK_SHRINK, 5);
 
  m_listviewtext.set_column_title(0, "Action");
  m_listviewtext.set_column_title(1, "Current Value");

  set_border_width(10);
  add(m_box0);

  on_game_changed();
  show_all();

  m_entry.hide();
  m_spinbutton.hide();
}


Radar::~Radar()
{
}

void Radar::on_cursor_changed()
{
  Gtk::ListViewText::SelectionList list = m_listviewtext.get_selected();
  if (!list.empty())
  {
    std::cout << " new row selected: " << list.front() << std::endl;
    exastris::Action action = m_actions[list.front()];

    exastris::Action::String *stringtype(0);
    exastris::Action::Integer *inttype(0);

    m_entry.hide();
    m_spinbutton.hide();

    if ((stringtype = 
	  dynamic_cast<exastris::Action::String *>(action.m_type.get())))
    {
      m_entry.set_text(stringtype->m_value);
      m_entry.set_max_length(stringtype->m_maxlength);
      m_entry.show();
    } else if ((inttype = 
	  dynamic_cast<exastris::Action::Integer *>(action.m_type.get())))
    {
      std::cout << "Setting range: " << inttype->m_minvalue << " " <<  inttype->m_maxvalue << std::endl;
      m_spinbutton.set_numeric(true);
      m_spinbutton.set_increments(1, 1);
      m_spinbutton.set_range(inttype->m_minvalue, inttype->m_maxvalue);
      m_spinbutton.set_value(inttype->m_value);
      m_spinbutton.show();
    } else if ((dynamic_cast<exastris::Action::None *>(action.m_type.get()))) {
      on_button1_clicked();
    }
  }
}



void Radar::on_button1_clicked()
{
  Gtk::ListViewText::SelectionList list = m_listviewtext.get_selected();
  if (!list.empty())
  {
    exastris::Action action = m_actions[list.front()];
    exastris::Action::String *stringtype(0);
    exastris::Action::Integer *inttype(0);

    if ((stringtype = 
	  dynamic_cast<exastris::Action::String *>(action.m_type.get())))
    { 
      stringtype->m_value = m_entry.get_text();
    } else if ((inttype = 
	  dynamic_cast<exastris::Action::Integer *>(action.m_type.get())))
    {
      inttype->m_value = m_spinbutton.get_value_as_int();
    }

    m_game.perform_action(action);
    on_game_changed();
  }
}

void Radar::on_button2_clicked()
{
  hide();
}

void Radar::on_game_changed()
{
  m_area.queue_draw();
  std::vector<exastris::Action> options 
    = m_game.get_current_actions();

  m_actions = options;

  m_listviewtext.clear_items();
  for (std::vector<exastris::Action>::const_iterator itr =
         options.begin();
       itr != options.end();
       ++itr)
  {
    guint row_number = m_listviewtext.append_text();
    m_listviewtext.set_text(row_number, 0, itr->m_name);
    m_listviewtext.set_text(row_number, 1, itr->value_as_string());
  } 
  
  m_label.set_text(m_game.get_current_state_description());
 
}

int main(int argc, char** argv)
{
  Gtk::Main main_instance (argc, argv);

  Radar radar;
  Gtk::Main::run(radar);

  return 0;
}

