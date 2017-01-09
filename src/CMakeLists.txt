include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(utility)

add_library(sdm
        $<TARGET_OBJECTS:sdmUtility>)
target_link_libraries(sdm ${PTHREAD_LIB} gmpxx gmp)

install(TARGETS sdm DESTINATION lib)
