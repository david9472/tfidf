//
// Created by David Frühwirth on 15.06.22.
//

#ifndef _RANKEDLIST_
#define _RANKEDLIST_
#include <string>
#include <vector>
#include <iostream>

struct rank
{
  double rank_;
  std::string name_;

  rank() = default;
  friend std::ostream& operator<<(std::ostream& out, const rank& rank)
  {
    out << rank.rank_ << "  " << rank.name_ << std::endl;
    return out;
  }
};


class ranked_list
{
  std::vector<rank> list_;

  public:
  const std::vector<rank>& getList() const {return list_;};
  rank& emplace_back(const rank& rank_ = rank()){return list_.emplace_back(rank_);}

  friend std::ostream& operator<<(std::ostream& out, const ranked_list& rank_ls)
  {
    for (const auto& rank : rank_ls.getList())
      out << rank;
    return out;
  }

  enum sorting
  {
    DESC,
    ASC
  };

  void sort(sorting sort = DESC);
};

#endif //_RANKEDLIST_
