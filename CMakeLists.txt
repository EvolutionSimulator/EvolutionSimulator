cmake_minimum_required(VERSION 3.16)
project(EvolutionSimulator)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler specific options for Debug and Release modes
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
  add_compile_options(-g) # Debug symbols (GCC/Clang)
  add_compile_options(-O0) # No optimization (GCC/Clang)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(NDEBUG)
  add_compile_options(-O3) # Full optimization (GCC/Clang)
endif()

# Add subdirectories
add_subdirectory(Engine)
add_subdirectory(UI)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# Download and setup Google Test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()
