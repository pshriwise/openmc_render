cmake_minimum_required(VERSION 3.3 FATAL_ERROR)


project(Weekend_Raytrace)

option(openmc OFF "Support rendering of OpenMC geometries")
option(openmp OFF "Support OpenMP parallelism")

set(TRACER_SOURCES geom.cpp color.cpp progress_bar.cpp scene.cpp trace.cpp main.cpp)

add_executable(tracer ${TRACER_SOURCES})
target_include_directories(tracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (openmc)
    find(OpenMC REQUIRED HINTS ${OPENMC_DIR})
    add_definitions(tracer OPENMC)
    target_link_libraries(tracer PUBLIC openmc)
endif()

if (openmp)
    find_package(OpenMP)
    if (OPENMP_FOUND)
    target_compile_options(tracer PUBLIC ${OpenMP_CXX_FLAGS})
    target_link_libraries(tracer PUBLIC ${OpenMP_CXX_FLAGS})
    endif()
endif()