cmake_minimum_required(VERSION 3.22)
project(GeneticSimulation)

find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(matplotplusplus)


add_executable(GeneticSimulation src/main.cpp src/defines.h src/organism.h src/organism.cpp src/optimiser.h src/optimiser.cpp src/defines.cpp)
target_link_libraries(GeneticSimulation PUBLIC matplot)

add_executable(Test test/test_organism.cpp src/organism.h src/organism.cpp test/test_defines.cpp src/defines.h src/defines.cpp test/test_optimiser.cpp src/optimiser.cpp src/optimiser.h)
target_link_libraries(Test PRIVATE Catch2::Catch2WithMain)


