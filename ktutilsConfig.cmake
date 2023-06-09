
if(
    TARGET
        ktutils::ktutils
)
    return()
endif()

add_library(
    ktutils::ktutils
    INTERFACE
        IMPORTED
)

target_include_directories(
    ktutils::ktutils
    INTERFACE
        /usr/local/include/ktutils
)
target_link_libraries(
    ktutils::ktutils
    INTERFACE
        /usr/local/lib/libktutils.a
)
target_compile_features(
    ktutils::ktutils
    INTERFACE
        cxx_std_20
)

if (
    NOT
        DEFINED
            ktutils_FIND_QUIETLY
)
    message(STATUS "Found ktutils: ${CMAKE_CURRENT_LIST_DIR}")
endif()
