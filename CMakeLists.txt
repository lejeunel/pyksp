cmake_minimum_required(VERSION 2.8.12)
project(pyksp)
# Set source directory
set(SOURCE_DIR "src")
# Tell CMake that headers are also in SOURCE_DIR
include_directories(${SOURCE_DIR})


# Generate python module
add_subdirectory(lib/pybind11)
pybind11_add_module(pyksp ${SOURCES} "${SOURCE_DIR}/pyksp.cpp")
