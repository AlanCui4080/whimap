cmake_minimum_required(VERSION 3.15)

project(whimap LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
find_package(PkgConfig REQUIRED)
find_package(FLEX REQUIRED)


if(${OPTIZIME})
add_compile_options(-${OPTIZIME})
elseif()
add_compile_options()
endif()

add_compile_options(-fsanitize=undefined -fsanitize=address -fsanitize=leak)
add_compile_options(-Wextra -Wall)
add_compile_options(-mavx2 -msse4.1 -msse4.2 -mssse3 -msse2 -mavx -msha -maes -mbmi -mbmi2 -mfma -mf16c -mpclmul)
add_link_options(-fsanitize=undefined -fsanitize=address -fsanitize=leak)

if(DEBUG)
add_compile_options(-g)
endif()


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include_directories(GLOB ${PROJECT_SOURCE_DIR}/src)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lexbison)
flex_target(sql_scanner ${PROJECT_SOURCE_DIR}/src/sql.l ${CMAKE_CURRENT_BINARY_DIR}/lexbison/sqllex.cpp.hpp)
include_directories(GLOB ${FLEX_sql_scanner_OUTPUT_HEADER})
include_directories(GLOB ${CMAKE_CURRENT_BINARY_DIR}/lexbison)


file(GLOB PROJ_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(whimap ${PROJ_SRC} ${FLEX_sql_scanner_OUTPUTS})
target_link_libraries(whimap ${FLEX_LIBRARIES})
link_libraries(hwy)

enable_testing()
add_subdirectory(test)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CTestCustom.cmake ${CMAKE_CURRENT_BINARY_DIR})


