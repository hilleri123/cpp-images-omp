cmake_minimum_required(VERSION 3.13)
project(Task VERSION 1.0 LANGUAGES CXX)

set (CMAKE_VERBOSE_MAKEFILE on)

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo

    -fopenmp
)

set (SOURCES
	main.cpp
)

FIND_PACKAGE( OpenMP)
  if(OPENMP_FOUND)
     message("OPENMP FOUND")
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
include_directories(SYSTEM ${OpenMP_INCLUDE_PATH})


add_executable (${PROJECT_NAME} ${SOURCES} main.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${OpenMP_CXX_LIBRARIES} ${PROJECT_LINK_LIBS})
