cmake_minimum_required(VERSION 3.15)

project(whimap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 23)
find_package(PkgConfig REQUIRED)

file(GLOB PROJ_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
if(${OPTIZIME})
add_compile_options(-${OPTIZIME})
elseif()
endif()
add_compile_options(-fsanitize=undefined -fsanitize=leak -fsanitize=address)
add_compile_options(-Wextra -Wall)
add_compile_options(-mavx2 -mavx -msha -maes -mbmi -mbmi2 -mfma -mf16c -mpclmul)
add_link_options(-fsanitize=undefined -fsanitize=leak -fsanitize=address)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
add_executable(whimap ${PROJ_SRC})
include(thirdparty)

enable_testing()
add_subdirectory(test)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CTestCustom.cmake ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
