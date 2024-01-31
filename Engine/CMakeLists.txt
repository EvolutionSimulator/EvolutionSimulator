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
set(CMAKE_CSX_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
  add_compile_options(-g) # Debug symbols (GCC/Clang)
  add_compile_options(-O0) # No optimization (GCC/Clang)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(NDEBUG)
  add_compile_options(-O3) # Full optimization (GCC/Clang)
endif()

find_package(OpenMP)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(Engine STATIC
  include/core/engine.h src/core/engine.cpp
  include/simulation/simulation.h src/simulation/simulation.cpp
  include/simulation/simulation_data.h src/simulation/simulation_data.cpp
  include/simulation/environment.h src/simulation/environment.cpp

  include/neat/neuron.h src/neat/neuron.cpp
  include/neat/link.h src/neat/link.cpp
  include/neat/genome.h src/neat/genome.cpp
  include/neat/neural_network.h src/neat/neural_network.cpp
  include/neat/brain_module.h src/neat/brain_module.cpp

  include/entity/creature/mutable.h src/entity/creature/mutable.cpp

  include/entity/creature/egg.h
  src/entity/creature/egg.cpp
  include/core/mathlib.h
  src/core/mathlib.cpp
  include/entity/creature/reproduction.h
  src/entity/creature/reproduction.cpp

  include/entity/grabbing_entity.h src/entity/grabbing_entity.cpp


  include/core/data_accessor.h
  include/core/synchronization_primitives.h
  include/core/settings.h src/core/settings.cpp

  include/entity/entity.h src/entity/entity.cpp

  include/entity/food.h src/entity/food.cpp

  include/entity/creature/pheromone.h src/entity/creature/pheromone.cpp

  include/entity/movable_entity.h src/entity/movable_entity.cpp
  include/entity/alive_entity.h src/entity/alive_entity.cpp

  include/entity/creature/vision_system.h src/entity/creature/vision_system.cpp
  include/entity/creature/digestive_system.h src/entity/creature/digestive_system.cpp

  include/entity/creature/pheromones_system.h src/entity/creature/pheromones_system.cpp

  include/entity/creature/creature.h src/entity/creature/creature.cpp

  include/core/geometry_primitives.h src/core/geometry_primitives.cpp
  include/core/collision_functions.h src/core/collision_functions.cpp

  include/simulation/food_manager.h src/simulation/food_manager.cpp
  include/simulation/entity_grid.h src/simulation/entity_grid.cpp
  include/simulation/collision_manager.h src/simulation/collision_manager.cpp
  include/simulation/creature_manager.h src/simulation/creature_manager.cpp

  include/core/random.h src/core/random.cpp
)

add_subdirectory(tests)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Engine PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(Engine PRIVATE Engine_LIBRARY)

target_link_libraries(Engine PRIVATE nlohmann_json::nlohmann_json)

# Link OpenMP if found
if(OpenMP_CXX_FOUND)
  target_link_libraries(Engine PRIVATE OpenMP::OpenMP_CXX)
endif()
