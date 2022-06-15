//
// Created by David Frühwirth on 15.06.22.
//

#include "analyticalEngine.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include "Util.hpp"

analytical_engine::analytical_engine(std::vector<std::string> paths)
{
  for (const auto& path : paths)
  {
    parseFile(std::make_shared<std::fstream>(path), "");
  }
  setUpData();
}

void analytical_engine::setUpData()
{
  createDictVector();
  createIDF();
}

void analytical_engine::parseFile(const std::shared_ptr<std::fstream>& file, const std::string& file_name)
{
  parsed_file parsed_file;
  parsed_file.name_ = file_name.length() ? file_name : "file";
  parsed_file.splitFileIntoStrings(file);
  parsed_files_.emplace_back(parsed_file);
}

void analytical_engine::addDocument(const std::string& path)
{
  std::string filename{};
  Util::getFilenameFromPath(path, filename);
  parseFile(std::make_shared<std::fstream>(path), filename);
}

void analytical_engine::createDictVector()
{
  dict_.clear();

  for (const auto &parsed_file: parsed_files_)
  {
    for (const auto &word: parsed_file.words_)
      dict_.emplace_back(word);
  }

  std::sort(dict_.begin(), dict_.end());
  const auto last = std::unique(dict_.begin(), dict_.end());
  dict_.erase(last, dict_.end());
}

void analytical_engine::createIDF()
{
  for (const auto& word : dict_)
  {
    std::size_t include_doc_count{};
    for (const auto& file : parsed_files_)
      include_doc_count += file.getWordCount(word) ? 1 : 0;

    if (!include_doc_count)
    {
      idf_.emplace_back(0);
      continue;
    }

    idf_.emplace_back(std::log10(static_cast<double>(parsed_files_.size()) / static_cast<double>(include_doc_count)));
  }
}

void analytical_engine::createTermFrequenciesAndTFIDF()
{
  for (auto& parsed_file : parsed_files_)
  {
    parsed_file.createTermFrequency(dict_);
    parsed_file.calculateTFIDF(idf_);
  }
}

void analytical_engine::setQuery(const std::string& query_path)
{
  if (dict_.empty())
    throw std::logic_error("cannot compute query without dict vector!");
  Util::getFilenameFromPath(query_path, query_.name_);
  query_.splitFileIntoStrings(std::make_shared<std::fstream>(query_path));
  query_.createTermFrequency(dict_);
  query_.calculateTFIDF(idf_);
}

double analytical_engine::calculateCosineSim(const parsed_file& a, const parsed_file& b)
{
  const double dotProduct = a.tfidf_.dotP(b.tfidf_);
  const double multi_length = a.tfidf_.arithmetical_length() * b.tfidf_.arithmetical_length();
  return dotProduct/multi_length;
}

void analytical_engine::findMostRelevantDocument()
{
  typedef std::pair<double, std::string> row;
  std::vector<row> ranking;
  for (const auto& doc : parsed_files_)
  {
    auto& rank = ranking.emplace_back();
    rank.second = doc.name_;
    rank.first = calculateCosineSim(query_, doc);
  }

  std::sort(ranking.begin(), ranking.end(), [](const row& a, const row& b)
  {
    return a.first > b.first;
  });

  for (const auto& i : ranking)
  {
    std::cout << i.second << "   " << i.first << std::endl;
  }
}