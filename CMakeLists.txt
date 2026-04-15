cmake_minimum_required(VERSION 3.15)

project(Dedup VERSION 0.1
                  DESCRIPTION "code dedup detector"
                  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



add_executable(dedup src/main.cpp src/src/SimilarityDetect.cpp src/src/Snippets.cpp)
