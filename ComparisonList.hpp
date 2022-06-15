//
// Created by David Frühwirth on 15.06.22.
//

#ifndef _COMPARISONLIST_
#define _COMPARISONLIST_
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

template<class t>
struct comp
{
  std::string name_a_;
  std::string name_b_;
  t comparison_val_;
  public:
  comp() = default;
  comp(const std::string& a, const std::string& b, const t& val) :
  name_a_(a), name_b_(b), comparison_val_(val){};

  friend std::ostream& operator<<(std::ostream& out, const comp<t>& cmp)
  {
    out << cmp.name_a_ << "  " << cmp.name_b_ << "  " << cmp.comparison_val_ << std::endl;
     return out;
  }
};

template<class t>
class comparison_list
{
  std::vector<comp<double>> list_;

  public:
  comparison_list()=default;

  comp<t>& emplace_back(const comp<t>& val = comp<t>()){return list_.emplace_back(val);};
  const std::vector<comp<t>>& getList() const{return list_;};

  friend std::ostream& operator<<(std::ostream& out, const comparison_list<t>& cmp_ls)
  {
    for (const auto& i : cmp_ls.list_)
      out << i;

    return out;
  }
};


#endif //_COMPARISONLIST_
