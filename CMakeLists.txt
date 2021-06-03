cmake_minimum_required(VERSION 3.1)
project(pyksp)
# Set source directory
set(SOURCE_DIR "src")

set(SOURCES ${SOURCES}
    ${SOURCE_DIR}/ksp.cpp
    ${SOURCE_DIR}/path.cpp
    ${SOURCE_DIR}/graph.cpp
)


# Tell CMake that headers are also in SOURCE_DIR
include_directories(${SOURCE_DIR})


# Generate python module
add_subdirectory(lib/pybind11)
pybind11_add_module(pyksp ${SOURCES} "${SOURCE_DIR}/pyksp.cpp")
