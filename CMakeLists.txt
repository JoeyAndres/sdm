cmake_minimum_required(VERSION 2.8.11)
project(sdm)

include(CMakeFiles/CMakeLibraryCheck)
include(CMakeFiles/CMakePlatformCheck)
include(CMakeFiles/CMakeGenerateMainHeader)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -O3 -g -lgmpxx -lgmp")

add_subdirectory(src)
#add_subdirectory(lib)
add_subdirectory(test)

# This is where the sub-headers will be installed in /usr/local/include directory.
set(SUB_HEADERS_PREFIX sdm_bits)

# Include all header file to just one.
GenerateMainHeader(
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
        ${SUB_HEADERS_PREFIX}
        ${CMAKE_SOURCE_DIR}/lib)
MESSAGE(STATUS "Generated global header file")

set(INSTALL_DIRECTORY /usr/local/include)
install(FILES ${PROJECT_BINARY_DIR}/sdm
        DESTINATION ${INSTALL_DIRECTORY})
install(DIRECTORY ${PROJECT_BINARY_DIR}/${SUB_HEADERS_PREFIX}/
        DESTINATION ${INSTALL_DIRECTORY}/${SUB_HEADERS_PREFIX})
