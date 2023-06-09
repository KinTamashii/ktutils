cmake_minimum_required(
    VERSION
        3.20
        FATAL_ERROR
)

project(ktutils-test)





# Complier settings.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++20")


file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include_directories(
    ktutils-test
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
)


add_executable(ktutils-test ${SOURCES})

target_link_libraries(ktutils-test ${CMAKE_CURRENT_BINARY_DIR}/../libktutils.a)





