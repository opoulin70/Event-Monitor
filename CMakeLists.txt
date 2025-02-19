cmake_minimum_required(VERSION 3.14)

project(EventMonitor)

# Options for controlling builds
option(ENABLE_TESTS "Build test executable" ON)
option(BUILD_MAIN_EXECUTABLE "Build main application" ON)

# Propagate test flag to subdirectories
set(ENABLE_TESTS ${ENABLE_TESTS} CACHE INTERNAL "Propagate test flag")
set(BUILD_MAIN_EXECUTABLE ${BUILD_MAIN_EXECUTABLE} CACHE INTERNAL "Propagate build main executable flag")

# Enable testing support
include(CTest)
enable_testing()

# Add source directory
add_subdirectory(src/EventMonitor)