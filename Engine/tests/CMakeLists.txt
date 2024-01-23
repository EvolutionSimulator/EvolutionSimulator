cmake_minimum_required(VERSION 3.16)

# Specify the test executable
add_executable(Engine_tests
    neat.cpp
    collisions.cpp
    creature.cpp
    movement.cpp
    reproduction.cpp
)

# Link against Google Test and the Engine library
target_link_libraries(
  Engine_tests
  Engine
  gtest_main
  gmock_main
)
add_test(NAME Engine_tests COMMAND Engine_tests)

include(GoogleTest)
gtest_discover_tests(Engine_tests DISCOVERY_MODE PRE_TEST)

