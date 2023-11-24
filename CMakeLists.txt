cmake_minimum_required(VERSION 3.16)
project(EvolutionSimulator)

# Global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(Engine)
add_subdirectory(UI)
