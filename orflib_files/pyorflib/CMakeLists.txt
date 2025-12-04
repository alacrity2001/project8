# set location of python headers and libraries
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")           # VC++ on Windows
    set(PYTHON_HOME $ENV{LOCALAPPDATA}/Programs/Miniconda3/envs/orf531)
    set(PYTHON_INCLUDE_DIRECTORY ${PYTHON_HOME}/include)
    set(NUMPY_INCLUDE_DIRECTORY ${PYTHON_HOME}/Lib/site-packages/numpy/_core/include)
    set(PYTHON_LIBRARY_PATH ${PYTHON_HOME}/libs/python312.lib)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")       # GCC on Linux
    set(PYTHON_HOME $ENV{HOME}/miniconda3/envs/orf531)
    set(PYTHON_INCLUDE_DIRECTORY ${PYTHON_HOME}/include/python3.12)
    set(NUMPY_INCLUDE_DIRECTORY ${PYTHON_HOME}/lib/python3.12/site-packages/numpy/_core/include)
    set(PYTHON_LIBRARY_PATH ${PYTHON_HOME}/lib/libpython3.12.so)
else()
    message(FATAL_ERROR "Unsupported C++ compiler")

endif()

set(pyorflib_SOURCES
    pymodule.cpp 	
)

add_library(pyorflib SHARED ${pyorflib_SOURCES})

add_dependencies(pyorflib orflib)

target_include_directories(pyorflib PRIVATE
    .. 
    ${PYTHON_INCLUDE_DIRECTORY} 
    ${NUMPY_INCLUDE_DIRECTORY}
    ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/include
)

# remove prefix from the DLL name
set_target_properties(pyorflib PROPERTIES PREFIX "")
set_target_properties(pyorflib PROPERTIES DEBUG_POSTFIX "")
# silence the numpy deprecation message
set_target_properties(pyorflib PROPERTIES COMPILE_DEFINITIONS "NPY_NO_DEPRECATED_API")

# linker option adjustments for supported compilers
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")         # VC++ on Windows
    # adjust suffix and output location
    set_target_properties(pyorflib PROPERTIES SUFFIX ".pyd")
    set_target_properties(pyorflib PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/orflib)   
    # link libraries and options
    target_link_libraries(pyorflib PRIVATE
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/orflib${CMAKE_DEBUG_POSTFIX}.lib 
        ${PYTHON_LIBRARY_PATH}
        ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/lib/${PLATFORM_TARGET}/lapack${CMAKE_DEBUG_POSTFIX}.lib
        ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/lib/${PLATFORM_TARGET}/blas${CMAKE_DEBUG_POSTFIX}.lib
        ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/lib/${PLATFORM_TARGET}/f2c${CMAKE_DEBUG_POSTFIX}.lib
    )
    target_link_options(pyorflib PRIVATE /MANIFEST:NO /INCREMENTAL:NO /DEBUG)    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")       # GCC on Linux
    # adjust output location
    set_target_properties(pyorflib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/orflib)
    # link libraries and options (assuming GNU)
    target_link_libraries(pyorflib PRIVATE 
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/liborflib${CMAKE_DEBUG_POSTFIX}.a 
        ${PYTHON_LIBRARY_PATH}
        ### use the three lines below to link against the f2c-ed BLAS/LAPACK packaged with armadillo
        ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/lib/${PLATFORM_TARGET}/liblapack${CMAKE_DEBUG_POSTFIX}.a  
        ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/lib/${PLATFORM_TARGET}/libblas${CMAKE_DEBUG_POSTFIX}.a  
        ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/lib/${PLATFORM_TARGET}/libf2c${CMAKE_DEBUG_POSTFIX}.a  
        ### use the four lines below to link against BLAS/LAPACK pre-installed on the Linux machine
        # /usr/lib/x86_64-linux-gnu/lapack/liblapack.a         
        # /usr/lib/x86_64-linux-gnu/blas/libblas.a 
        # libgfortran.so.4 
        # libquadmath.so.0
    )
endif()
