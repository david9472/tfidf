#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <regex>

namespace Util
{
  void getFilenameFromPath(const std::string& path, std::string& filename)
  {
    const auto it = path.find_last_of('/');
    if (it == std::string::npos)
    {
      filename.clear();
      return;
    }
    filename = path.substr(it+1, path.size()-it);
  }
}


struct analytical_vector
{
  private:
  std::vector<double> data_;

  public:
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
};

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
    std::cout << "parsed_file not open!" << std::endl;
    return;
  }
  std::string line{};
  while (getline(*file_, line))
  {
    splitString(line, words_, " ");
  }
}


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
  void parseFiles();
  void addDocument(const std::string& path);
  void setQuery(const std::string& path);
  double calculateCosineSim(const parsed_file& a, const parsed_file& b);
  void findMostRelevantDocument();

  analytical_engine(std::vector<std::string> paths);
  analytical_engine() = default;
};

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

  std::sort(ranking.begin(), ranking.end(), [](row a, row b)
  {
    return a.first > b.first;
  });

  for (const auto& i : ranking)
  {
    std::cout << i.second << "   " << i.first << std::endl;
  }
}

int main(int argc, char *argv[])
{
  std::vector<std::string> paths;
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