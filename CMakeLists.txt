cmake_minimum_required(VERSION 3.3 FATAL_ERROR)


project(Weekend_Raytrace
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(openmc OFF "Support rendering of OpenMC geometries")
option(openmp OFF "Support OpenMP parallelism")

list(APPEND TRACER_SOURCES
  src/geom.cpp
  src/color.cpp
  src/progress_bar.cpp
  src/scene.cpp
  src/trace.cpp
  src/main.cpp)

add_executable(tracer ${TRACER_SOURCES})
target_include_directories(tracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (openmc)
    find_package(OpenMC REQUIRED HINTS ${OPENMC_DIR})
    add_definitions(-DOPENMC)
    target_link_libraries(tracer PUBLIC OpenMC::libopenmc)
endif()

if (openmp)
    find_package(OpenMP)
    if (OPENMP_FOUND)
    target_compile_options(tracer PUBLIC ${OpenMP_CXX_FLAGS})
    target_link_libraries(tracer PUBLIC ${OpenMP_CXX_FLAGS})
    endif()
endif()

install(TARGETS tracer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
