//
// Created by David Frühwirth on 15.06.22.
//

#ifndef _ANALYTICALENGINE_
#define _ANALYTICALENGINE_
#include <vector>
#include <memory>
#include <string>
#include "analyticalVector.hpp"
#include "parsedFile.hpp"
#include <algorithm>
#include "ComparisonList.hpp"
#include "rankedList.hpp"

struct analytical_engine
{
  std::vector<parsed_file> parsed_files_;
  parsed_file query_;
  std::vector<std::string> dict_;
  analytical_vector idf_;

  void createDictVector();
  void createIDF();
  void setUpData();
  void createTermFrequenciesAndTFIDF();
  void parseFile(const std::shared_ptr<std::fstream>& file, const std::string& file_name);
  void addDocument(const std::string& path);
  void setQuery(const std::string& path);
  double calculateCosineSim(const parsed_file& a, const parsed_file& b);
  void getOrderedRelevantDocumentList(ranked_list& list);
  void computeSimilarityBetweenDocuments(comparison_list<double>& list);

  analytical_engine(std::vector<std::string> paths);
  analytical_engine() = default;
};
#endif //_ANALYTICALENGINE_
