#ifndef __wares_hpp__
#define __wares_hpp__

#include <string>
#include <vector>

namespace exastris
{

  struct TradeItem
  {
    std::string m_name;
    int m_numavailable;
    double m_unitcost;

  };

  struct Ware_For_Sale
  {
    Ware_For_Sale(const std::string &t_name,
	double t_price_per_unit,
	int t_quantity_available)
      : m_name(t_name),
        m_price_per_unit(t_price_per_unit),
	m_quantity_available(t_quantity_available)
    {
    }

    std::string m_name;
    double m_price_per_unit;
    int m_quantity_available;
  };

  struct Ware_For_Purchase
  {
    Ware_For_Purchase(const std::string &t_name,
	double t_price_per_unit,
	double t_price_paid_per_unit,
	int t_quantity_available)
      : m_name(t_name),
        m_price_per_unit(t_price_per_unit),
	m_price_paid_per_unit(t_price_paid_per_unit),
	m_quantity_available(t_quantity_available)
    {
    }

    std::string m_name;
    double m_price_per_unit;
    double m_price_paid_per_unit;
    int m_quantity_available;
  };

}


#endif
