//
// Created by David Frühwirth on 15.06.22.
//

#include "analyticalVector.hpp"
#include <cmath>

double analytical_vector::dotP(const analytical_vector& other) const
{
  if (data_.size() != other.getData().size())
    throw std::logic_error("cannot create dot product of different size!");

  double ret{};
  std::size_t index{};
  for (const auto& data_other : other.getData())
  {
    ret += data_other * data_.at(index);
    index++;
  }
  return ret;
}

double analytical_vector::arithmetical_length() const
{
  double sum_pow{};
  for (const auto& d : getData())
    sum_pow += std::pow(d, 2);

  return std::sqrt(sum_pow);
}