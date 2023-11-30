project(set10108-cw02 LANGUAGES CXX)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

#### OPEN MP PACKAGES ####
find_package(OpenMP)
set(CMAKE_C_FLAGS "-L/opt/homebrew/opt/libomp/lib")
set(CMAKE_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")

add_executable(NQueens nqueens.cpp)
