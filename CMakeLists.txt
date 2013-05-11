project(PlasmaImport)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(MaxSDK)
find_package(HSPlasma REQUIRED)

add_subdirectory(src)