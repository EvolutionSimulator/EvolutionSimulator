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
  include/core/engine.h src/core/engine.cpp
  include/simulation/simulation.h src/simulation/simulation.cpp
  include/simulation/simulation_data.h
  include/neat/neuron.h src/neat/neuron.cpp
  include/neat/link.h src/neat/link.cpp
  include/neat/genome.h src/neat/genome.cpp

  include/core/geometry_primitives.h src/core/geometry_primitives.cpp

  include/neat/neural_network.h
  src/neat/neural_network.cpp
  include/entities/entity.h
  src/entities/entity.cpp

  include/entities/food.h
  src/entities/food.cpp
  include/simulation/environment.h
  src/simulation/environment.cpp
  include/entities/creature.h
  src/entities/creature.cpp
  src/entities/movable_entity.cpp
  include/entities/movable_entity.h
  include/core/collision_functions.h
  src/core/collision_functions.cpp
  include/core/settings.h
  include/core/data_accessor.h
  src/core/settings.cpp
  include/simulation/food_manager.h src/simulation/food_manager.cpp
  include/simulation/entity_grid.h src/simulation/entity_grid.cpp
  include/simulation/creature_manager.h src/simulation/creature_manager.cpp
  include/simulation/collision_manager.h src/simulation/collision_manager.cpp
  include/entities/mutable.h src/entities/mutable.cpp

)

add_subdirectory(tests)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Engine PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(Engine PRIVATE Engine_LIBRARY)

target_link_libraries(Engine PRIVATE nlohmann_json::nlohmann_json)
