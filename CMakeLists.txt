cmake_minimum_required(VERSION 3.15)

project(whimap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 23)
find_package(PkgConfig REQUIRED)

file(GLOB PROJ_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_compile_options(-Wall -Wextra)
add_compile_options(-fsanitize=leak -fsanitize=address -fsanitize=undefined)
add_compile_options(-mavx2)
add_link_options(-fsanitize=leak -fsanitize=address -fsanitize=undefined)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
add_executable(whimap ${PROJ_SRC})
include(thirdparty)

enable_testing()
add_subdirectory(test)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
