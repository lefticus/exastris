#include "universe.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

struct color
{
  color()
    : red(0), green(0), blue(0)
  {
  }

  color(int t_red, int t_green, int t_blue)
    : red(t_red), green(t_green), blue(t_blue)
  {
  }

  int red, green, blue;

};

void saveppm(const std::string &filename, color graphic[][256])
{
  std::ofstream f(filename.c_str());
  f << "P3 256 256 255" << std::endl;
  for (int y=0; y<256; y++)
  {
    for (int x=0; x<256; x++)
    {
      f << graphic[x][y].red << " " << graphic[x][y].green << " " << graphic[x][y].blue << "   ";
    }
    f << std::endl;
  }
}


int main(int, char **)
{
  using namespace std;

  exastris::Universe u(1000);

  cout << "num galaxies: " << u.num_galaxies() << endl;

  for (int i = 0; i < u.num_galaxies(); ++i)
  {
    exastris::Galaxy g(u.get_galaxy(i));

    cout << "num planets: " << g.num_planets() << endl;
    color graphic[256][256];

    for (int j = 0; j < g.num_planets(); ++j)
    {
      exastris::Planet p(g.get_planet(j));

      graphic[p.m_x][p.m_y] = 
	color(int(p.m_planet_stats.m_red),
	      int(p.m_planet_stats.m_green),
	      int(p.m_planet_stats.m_blue));
    }

    stringstream ss;
    ss << "galaxy" << i << ".ppm";
    saveppm(ss.str(), graphic);
  }
}
