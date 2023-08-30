pkg_check_modules(HIGHWAY libhwy REQUIRED)
include_directories(${HIGHWAY_INCLUDE_DIRS})
link_libraries(${HIGHWAY_LIBRARIES})
