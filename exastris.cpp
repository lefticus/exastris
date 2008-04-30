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

  
  std::cout << "Player: " << m_game.get_player().get_name() << " in galaxy: " << m_game.get_player().get_location().first << " at planet: " << m_game.get_player().get_location().second << std::endl;

  exastris::Galaxy g(m_game.get_current_galaxy());

  Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

  cr->scale(width, height);

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

  exastris::Planet p(g.get_planet(m_game.get_player().get_location().second));
  cr->set_line_width(0.01);
  cr->set_source_rgba(1.0, 1.0, 1.0, .5);

  double fuel = m_game.get_player().get_fuel_level();
  cr->arc(p.m_x, p.m_y, fuel, 0.0, 2 * M_PI);
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
  
  // Child widgets
  Gtk::VBox m_box0;
  Gtk::HBox m_box1;
  Gtk::VBox m_box2;
  Gtk::HBox m_box3; //empty box

  Gtk::Button m_button1;
  Gtk::Button m_button2;
  Gtk::ListViewText m_listviewtext;
  exastris::Game m_game;
  Gtk::Statusbar m_sb;

  PPI m_area;
  std::vector<std::pair<int, std::string> > m_actions;
};


Radar::Radar()
  : m_box0(/*homogeneous*/false, /*spacing*/5), m_box1(false, 5), m_box2(false, 5), m_box3(false, 5), 
    m_button1("Select"), 
    m_button2("Quit"), 
    m_listviewtext(2),
    m_game(0, "Jameson"),
    m_sb(), 
    m_area(m_game)
{


  // box2
  m_button2.signal_clicked().connect(sigc::mem_fun(*this, &Radar::on_button2_clicked));
  m_button1.signal_clicked().connect(sigc::mem_fun(*this, &Radar::on_button1_clicked));
  m_area.signal_game_changed.connect(sigc::mem_fun(*this, &Radar::on_game_changed));

//  m_box2.pack_start(m_box3, /*Gtk::PackOptions*/Gtk::PACK_EXPAND_WIDGET, /*padding*/5);
  m_box2.pack_start(m_listviewtext, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box2.pack_start(m_button1, Gtk::PACK_SHRINK, 5);
  m_box2.pack_start(m_button2, Gtk::PACK_SHRINK, 5);
  
  // box1
  m_area.set_size_request(300, 300);
  m_listviewtext.set_size_request(200, 100);
  m_box1.pack_start(m_area, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box1.pack_start(m_box2, Gtk::PACK_SHRINK, 5);
    
  // box0
  m_box0.pack_start(m_box1, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box0.pack_start(m_sb, Gtk::PACK_SHRINK, 5);
 
  m_listviewtext.set_column_title(0, "Action");

  set_border_width(10);
  add(m_box0);
  show_all();

}


Radar::~Radar()
{
}

void Radar::on_button1_clicked()
{
  Gtk::ListViewText::SelectionList list = m_listviewtext.get_selected();
  m_game.perform_action(m_actions[list.front()].first);
  on_game_changed();
}

void Radar::on_button2_clicked()
{
  hide();
}

void Radar::on_game_changed()
{
  m_area.queue_draw();
  std::vector<std::pair<int, std::string> > options 
    = m_game.get_current_actions();

  m_actions = options;

  m_listviewtext.clear_items();
  for (std::vector<std::pair<int, std::string> >::const_iterator itr =
         options.begin();
       itr != options.end();
       ++itr)
  {
    guint row_number = m_listviewtext.append_text();
    m_listviewtext.set_text(row_number, 0, itr->second);
  } 
  
  std::cout << "game changed event" << std::endl;
  
}

int main(int argc, char** argv)
{
  Gtk::Main main_instance (argc, argv);

  Radar radar;
  Gtk::Main::run(radar);

  return 0;
}

