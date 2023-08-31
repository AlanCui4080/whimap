include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/highway/hwy)
set(HWY_ENABLE_TESTS false)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/highway)
