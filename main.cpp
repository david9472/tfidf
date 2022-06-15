#include <iostream>
#include "analyticalEngine.hpp"


int main(int argc, char *argv[])
{
  analytical_engine engine;
    engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                       "AI/MISC/Klausur_1/tfidf/cmake-build-debug/musk1.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/musk2.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/musk3.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/musk4.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/musk5.txt");

  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/trump1.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/trump2.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/trump3.txt");
  engine.addDocument("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                     "AI/MISC/Klausur_1/tfidf/cmake-build-debug/trump4.txt");

  engine.setUpData();
  engine.createTermFrequenciesAndTFIDF();

  engine.setQuery("/Users/davidfruhwirth/Documents/Uni/2_Semester/Introduction DS and "
                  "AI/MISC/Klausur_1/tfidf/cmake-build-debug/trump5.txt");

  for (const auto& file : engine.parsed_files_)
    std::cout << file << std::endl;

  std::cout << engine.query_ << std::endl;

  engine.findMostRelevantDocument();
}