#include <iostream>
#include "analyticalEngine.hpp"

int main(int argc, char *argv[])
{
  analytical_engine engine;
  engine.addDocument("musk1.txt");
  engine.addDocument("musk2.txt");
  engine.addDocument("musk3.txt");
  engine.addDocument("musk4.txt");
  engine.addDocument("musk5.txt");

  engine.addDocument("trump1.txt");
  engine.addDocument("trump2.txt");
  engine.addDocument("trump3.txt");
  engine.addDocument("trump4.txt");

  engine.setUpData();
  engine.createTermFrequenciesAndTFIDF();

  engine.setQuery("trump5.txt");

  for (const auto& file : engine.parsed_files_)
    std::cout << file << std::endl;

  std::cout << engine.query_ << std::endl;

  ranked_list list;
  engine.getOrderedRelevantDocumentList(list);
  std::cout << list;

  comparison_list<double> lst;
  engine.computeSimilarityBetweenDocuments(lst);
  std::cout << lst;

}