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

#include <gtkmm/window.h>
#include <gtkmm/main.h>

#include <math.h> //Needed by the IRIX MipsPro compiler, for sin().
#include <sstream>
#include <iostream>
#include "game.hpp"
#include <vector>

#include <pangomm/fontdescription.h>

const double DEG2RAD = 3.1415928 / 180.0;
#ifndef M_PI
#define M_PI 3.14159265359
#endif /* M_PI */

// PPI Plan Position Indicator: radar screen
class PPI : public Gtk::DrawingArea
{
public:
  PPI(exastris::Game &);
  virtual ~PPI();
  bool timer_callback();


protected:
  //Overridden default signal handlers:
  virtual void on_realize();
  virtual bool on_expose_event(GdkEventExpose* event);

  Glib::RefPtr<Gdk::GC> gc_;
  Gdk::Color blue_, red_, green_, black_, white_, grey_, yellow_;

private:
  int loop;
  double alpha;
  exastris::Game &m_game;
};


PPI::PPI(exastris::Game &t_game)
  : m_game(t_game)
{
  // get_window() would return 0 because the Gdk::Window has not yet been realized
  // So we can only allocate the colors here - the rest will happen in on_realize().
  Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap();

  blue_ = Gdk::Color("blue");
  red_ = Gdk::Color("red");
  green_ = Gdk::Color("green");

  black_ = Gdk::Color("black");
  white_ = Gdk::Color("white");
  grey_ = Gdk::Color("grey");

  yellow_ = Gdk::Color("yellow");

  colormap->alloc_color(blue_);
  colormap->alloc_color(red_);
  colormap->alloc_color(green_);
  
  colormap->alloc_color(black_);
  colormap->alloc_color(white_);
  colormap->alloc_color(grey_);

  colormap->alloc_color(yellow_);

  // timeout
//  Glib::signal_timeout().connect(sigc::mem_fun(*this, &PPI::timer_callback), 50);

  // loop, alpha
  loop = 0;
  alpha = 0.0;

  add_events(Gdk::EXPOSURE_MASK);
}


PPI::~PPI()
{
}


void PPI::on_realize()
{
  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  // Now we can allocate any additional resources we need
  Glib::RefPtr<Gdk::Window> window = get_window();

  gc_ = Gdk::GC::create(window);

  window->set_background(black_);
  window->clear();

  gc_->set_foreground(green_);
}


bool PPI::on_expose_event(GdkEventExpose*)
{
  // we need a ref to the gdkmm window
  Glib::RefPtr<Gdk::Window> window = get_window();

  // window geometry: x, y, width, height, depth
  int winx, winy, winw, winh, wind;
  window->get_geometry(winx, winy, winw, winh, wind);

  window->clear();


  std::cout << "Player: " << m_game.get_player().get_name() << " in galaxy: " << m_game.get_player().get_location().first << " at planet: " << m_game.get_player().get_location().second << std::endl;

  exastris::Galaxy g(m_game.get_universe().get_galaxy(m_game.get_player().get_location().first));


  for (int i = 0; i < g.num_planets(); ++i)
  {
    // draw_arc( gc_, int filled, int x, int y, int width, int height, int angle1, int angle2)
    // x :x coordinate of the left edge of the bounding rectangle 
    // Y :y coordinate of the top edge of the bounding rectangle 
    // width, height: width, height of the bounding rectangle 
    // angle1: start angle of the arc relative to the 3 o'clock position counter-clockwise in 1/64 ths of a degree
    // angle2: end angle of the arc relative to angle1 in 1/64 ths of a degree
    exastris::Planet p = g.get_planet(i);
    Gdk::Color c;
  //  c.set_rgb(p.m_planet_stats.m_red*10, p.m_planet_stats.m_green, p.m_planet_stats.m_blue);
//    c.set_rgb_p(, 0, 0);
    gc_->set_foreground(green_);
    gc_->set_line_attributes(/*line_width*/0, 
	/*LineStyle*/Gdk::LINE_SOLID,/*CapStyle*/Gdk::CAP_NOT_LAST,/*JoinStyle*/Gdk::JOIN_MITER);  

    window->draw_arc(gc_, true, p.m_x-2, p.m_y-2, 5, 5, 0, 360*64);
  }

  exastris::Planet p(g.get_planet(m_game.get_player().get_location().second));
  Gdk::Color c;
  gc_->set_foreground(white_);
  gc_->set_line_attributes(/*line_width*/1, 
      /*LineStyle*/Gdk::LINE_SOLID,/*CapStyle*/Gdk::CAP_NOT_LAST,/*JoinStyle*/Gdk::JOIN_MITER);  

  int fuel = m_game.get_player().get_fuel_level();
  window->draw_arc(gc_, false, p.m_x-(fuel/2), p.m_y-(fuel/2), fuel, fuel, 0, 360*64);


  return true;
}



class Radar : public Gtk::Window
{
public:
  Radar();
  virtual ~Radar();
  
protected:
  //signal handlers:
  void on_button2_clicked();
  
  // Child widgets
  Gtk::VBox m_box0;
  Gtk::HBox m_box1;
  Gtk::VBox m_box2;
  Gtk::HBox m_box3; //empty box

  Gtk::Button m_button2;
  exastris::Game m_game;
  Gtk::Statusbar m_sb;

  PPI m_area;
};


Radar::Radar()
  : m_box0(/*homogeneous*/false, /*spacing*/5), m_box1(false, 5), m_box2(false, 5), m_box3(false, 5), 
    m_button2("Quit"), 
    m_game(101, "Jameson"),
    m_sb(), 
    m_area(m_game)
{


  // box2
  m_button2.signal_clicked().connect(sigc::mem_fun(*this, &Radar::on_button2_clicked));
  m_box2.pack_start(m_box3, /*Gtk::PackOptions*/Gtk::PACK_EXPAND_WIDGET, /*padding*/5);
  m_box2.pack_start(m_button2, Gtk::PACK_SHRINK, 5);
  
  // box1
  m_area.set_size_request(300, 300);
  m_box1.pack_start(m_area, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box1.pack_start(m_box2, Gtk::PACK_SHRINK, 5);
    
  // box0
  m_box0.pack_start(m_box1, Gtk::PACK_EXPAND_WIDGET, 5);
  m_box0.pack_start(m_sb, Gtk::PACK_SHRINK, 5);
  
  set_border_width(10);
  add(m_box0);
  show_all();

}


Radar::~Radar()
{
}

void Radar::on_button2_clicked()
{
  hide();
}


int main(int argc, char** argv)
{
  Gtk::Main main_instance (argc, argv);

  Radar radar;
  Gtk::Main::run(radar);

  return 0;
}

