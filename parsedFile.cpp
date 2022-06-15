//
// Created by David Frühwirth on 15.06.22.
//
#include "parsedFile.hpp"
#include <regex>
#include <fstream>

std::ostream& operator<<(std::ostream& out, const parsed_file& file)
{
  out << file.name_ << std::endl;
  out.precision(5);
  for (const auto& item : file.tfidf_.getData())
    out << std::fixed << item << std::endl;

  return out;
}

std::size_t parsed_file::getWordCount(const std::string &searched_word) const
{
  std::size_t word_count{};
  for (const auto& word : words_)
  {
    word_count += (searched_word == word) ? 1 : 0;
  }
  return word_count;
}

void parsed_file::createTermFrequency(const std::vector<std::string>& dict_)
{
  for (const auto& entry : dict_)
  {
    term_freq_.emplace_back(static_cast<double>(getWordCount(entry))/static_cast<double>(words_.size()));
  }
}

void parsed_file::calculateTFIDF(const analytical_vector& idf)
{
  tfidf_ = idf*term_freq_;
}

void parsed_file::splitString(std::string& s, std::vector<std::string>& strings, const std::string& delimiter)
{
  size_t pos{};
  std::string token;
  const std::regex allowed_char("[^A-Za-z0-9 ]");

  auto prepString = [&allowed_char](std::string& s)
  {
    s = std::regex_replace(s, allowed_char, "");
    std::transform(s.begin(), s.end(),s.begin(), ::toupper);
  };



  while ((pos = s.find(delimiter)) != std::string::npos)
  {
    auto& it = strings.emplace_back(s.substr(0, pos));
    prepString(it);
    s.erase(0, pos + delimiter.length());
  }
  auto& it = strings.emplace_back(s);
  prepString(it);
}


void parsed_file::splitFileIntoStrings(const std::shared_ptr<std::fstream>& file_)
{
  if (!file_->is_open())
  {
    throw std::logic_error("file not opened!");
  }
  std::string line{};
  while (getline(*file_, line))
  {
    splitString(line, words_, " ");
  }
}