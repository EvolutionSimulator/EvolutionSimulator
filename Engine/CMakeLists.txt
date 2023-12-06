cmake_minimum_required(VERSION 3.16)

project(Engine LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(Engine STATIC
  include/engine.h src/engine.cpp
  include/simulation.h src/simulation.cpp
  include/simulationdata.h
  include/neat/neat-neuron.h src/neat/neat-neuron.cpp
  include/neat/neat-link.h src/neat/neat-link.cpp
  include/neat/neat-genome.h src/neat/neat-genome.cpp


  src/neat/neat-test.cpp
  include/neat/neat-test.h
  include/neat/neat-neural-network.h
  src/neat/neat-neural-network.cpp

)

add_subdirectory(tests)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Engine PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(Engine PRIVATE Engine_LIBRARY)
