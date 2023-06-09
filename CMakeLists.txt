cmake_minimum_required(
    VERSION
        3.20
        FATAL_ERROR
)

project(ktutils)


# Complier settings.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_BUILD_TYPE release)


set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++20")


include_directories(
    ktutils
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)





file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(ktutils ${SOURCES})





target_include_directories(ktutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)


file (GENERATE
    OUTPUT "ktutilsConfig.cmake" 
    CONTENT
"\n\
if(\n\
    TARGET\n\
        ktutils::ktutils\n\
)\n\
    return()\n\
endif()\n\
\n\
add_library(\n\
    ktutils::ktutils\n\
    INTERFACE\n\
        IMPORTED\n\
)\n\
\n\
target_include_directories(\n\
    ktutils::ktutils\n\
    INTERFACE\n\
        ${CMAKE_INSTALL_PREFIX}/include/ktutils\n\
)\n\
target_link_libraries(\n\
    ktutils::ktutils\n\
    INTERFACE\n\
        ${CMAKE_INSTALL_PREFIX}/lib/libktutils.a\n\
)\n\
target_compile_features(\n\
    ktutils::ktutils\n\
    INTERFACE\n\
        cxx_std_20\n\
)\n\
\n\
if (\n\
    NOT\n\
        DEFINED\n\
            ktutils_FIND_QUIETLY\n\
)\n\
    message(STATUS \"Found ktutils: \${CMAKE_CURRENT_LIST_DIR}\")\n\
endif()\n\
"
    )
install(TARGETS ktutils)
INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include/ktutils)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ktutilsConfig.cmake DESTINATION lib/cmake/ktutils)





add_subdirectory(test)

add_dependencies(ktutils-test ktutils)





message(${CMAKE_INSTALL_PREFIX})