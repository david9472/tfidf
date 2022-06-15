cmake_minimum_required(VERSION 3.21)
project(tfidf)

set(CMAKE_CXX_STANDARD 17)

add_executable(tfidf main.cpp analyticalVector.cpp analyticalVector.hpp parsedFile.cpp parsedFile.hpp analyticalEngine.cpp analyticalEngine.hpp Util.hpp)
