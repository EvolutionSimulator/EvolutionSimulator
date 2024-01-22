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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
  add_compile_options(-g) # Debug symbols (GCC/Clang)
  add_compile_options(-O0) # No optimization (GCC/Clang)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(NDEBUG)
  add_compile_options(-O3) # Full optimization (GCC/Clang)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(Engine STATIC
  include/engine.h src/engine.cpp
  include/simulation.h src/simulation.cpp
  include/simulationdata.h src/simulationdata.cpp
  include/environment.h src/environment.cpp

  include/neat/neat-neuron.h src/neat/neat-neuron.cpp
  include/neat/neat-link.h src/neat/neat-link.cpp
  include/neat/neat-genome.h src/neat/neat-genome.cpp
  include/neat/neat-neural-network.h src/neat/neat-neural-network.cpp
  include/neat/brain_module.h src/neat/brain_module.cpp

  include/mutable.h src/mutable.cpp
  include/egg.h
  src/egg.cpp
  include/mathlib.h
  src/mathlib.cpp
  include/reproduction.h
  src/reproduction.cpp


  include/data_accessor.h
  include/synchronization_primitives.h
  include/settings.h src/settings.cpp

  include/entity.h src/entity.cpp

  include/food.h src/food.cpp

  include/movable_entity.h src/movable_entity.cpp
  include/alive_entity.h src/alive_entity.cpp

  include/vision_system.h src/vision_system.cpp
  include/digestive_system.h src/digestive_system.cpp

  include/creature.h src/creature.cpp

  include/geometry_primitives.h src/geometry_primitives.cpp
  include/collision_functions.h src/collision_functions.cpp
  include/config.h

  include/food_manager.h src/food_manager.cpp
  include/entity_grid.h src/entity_grid.cpp
  include/collision_manager.h src/collision_manager.cpp
  include/creature_manager.h src/creature_manager.cpp
)

add_subdirectory(tests)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Engine PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(Engine PRIVATE Engine_LIBRARY)

target_link_libraries(Engine PRIVATE nlohmann_json::nlohmann_json)
