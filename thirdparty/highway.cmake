include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/highway/hwy)
set(HWY_ENABLE_TESTS OFF CACHE BOOL "Highway Tests")
set(HWY_ENABLE_CONTRIB OFF CACHE BOOL "Highway Contrib")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/highway)
