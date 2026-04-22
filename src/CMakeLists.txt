add_library(convolution_core
    toeplitz.cpp
)

target_include_directories(convolution_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    /opt/homebrew/opt/ntl/include
    /opt/homebrew/opt/fftw/include
)

target_link_libraries(convolution_core
    PUBLIC
        PkgConfig::FFTW3
        ${NTL_LIB}
        ${GMP_LIB}
)