add_library(utils
    routine.cpp
)

target_include_directories(utils PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)