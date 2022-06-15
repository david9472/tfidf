//
// Created by David Frühwirth on 15.06.22.
//

#ifndef _ANALYTICALVECTOR_
#define _ANALYTICALVECTOR_
#include <vector>


class analytical_vector
{
  private:
  std::vector<double> data_;

  public:
  analytical_vector() = default;

  void emplace_back(const double d){data_.emplace_back(d);};
  const std::vector<double>& getData() const {return data_;}

  double dotP(const analytical_vector& other) const;
  double arithmetical_length() const;

  analytical_vector operator*(const analytical_vector& other) const
  {
    if (data_.size() != other.getData().size())
      throw std::logic_error("cannot create dot product of different size!");

    analytical_vector ret{};
    std::size_t index{};
    for (const auto& data_other : other.getData())
    {
      ret.emplace_back(data_other * data_.at(index));
      index++;
    }
    return ret;
  }

};



#endif //_ANALYTICALVECTOR_
