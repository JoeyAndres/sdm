include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC_UTILITY_FILES "*.cpp")
add_library(sdmUtility OBJECT ${SRC_UTILITY_FILES})

