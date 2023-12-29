cmake_minimum_required(VERSION 3.16)

project(Engine LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

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
  include/simulation_data.h
  include/neat/neat-neuron.h src/neat/neat-neuron.cpp
  include/neat/neat-link.h src/neat/neat-link.cpp
  include/neat/neat-genome.h src/neat/neat-genome.cpp


  include/neat/neat-neural-network.h
  src/neat/neat-neural-network.cpp
  include/entity.h
  src/entity.cpp
  src/simulation_data.cpp
  include/food.h
  src/food.cpp
  include/environment.h
  src/environment.cpp
  include/creature.h
  src/creature.cpp
  src/movable_entity.cpp
  include/movable_entity.h
  include/collision_functions.h
  src/collision_functions.cpp
  include/settings.h
  include/data_accessor.h
  src/settings.cpp
  include/food_manager.h src/food_manager.cpp
  include/entity_grid.h src/entity_grid.cpp
  include/creature_manager.h src/creature_manager.cpp
  include/collision_manager.h src/collision_manager.cpp

)

add_subdirectory(tests)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Engine PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(Engine PRIVATE Engine_LIBRARY)

target_link_libraries(Engine PRIVATE nlohmann_json::nlohmann_json)
