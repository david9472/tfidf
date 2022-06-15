//
// Created by David Frühwirth on 15.06.22.
//

#include "rankedList.hpp"
void ranked_list::sort(sorting sort)
{
  std::sort(list_.begin(), list_.end(), [sort](const rank& a, const rank& b)
  {
    return sort == DESC ? a.rank_ > b.rank_ : a.rank_ < b.rank_;
  });
}