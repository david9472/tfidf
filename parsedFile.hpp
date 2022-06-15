//
// Created by David Frühwirth on 15.06.22.
//

#ifndef _PARSEDFILE_
#define _PARSEDFILE_
#include <string>
#include <vector>
#include "analyticalVector.hpp"
#include <ostream>
#include <memory>

struct parsed_file
{
  std::string name_;
  std::vector<std::string> words_;
  analytical_vector term_freq_;
  analytical_vector tfidf_;
  parsed_file() = default;

  std::size_t getWordCount(const std::string& word) const;
  static void splitString(std::string& s, std::vector<std::string>& strings, const std::string& delimiter);
  void splitFileIntoStrings(const std::shared_ptr<std::fstream>& file_);
  void createTermFrequency(const std::vector<std::string>& dict_);
  void calculateTFIDF(const analytical_vector& idf);

  friend std::ostream& operator<<(std::ostream& out, const parsed_file& file);
  bool operator==(const parsed_file& other) const
  {
    return (name_ == other.name_);
  }
};

#endif //_PARSEDFILE_
