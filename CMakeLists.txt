cmake_minimum_required(VERSION 3.12)

# default flags
if(NOT DEFINED CMAKE_CXX_FLAGS)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -march=native" CACHE STRING "Flags used by the compiler during all build types.")
endif()
# build type defaults to release
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS) Debug Release RelWithdebInfo MinSizeRel.")
endif()

project(bytecode_playground LANGUAGES CXX)

find_package(doctest REQUIRED)

add_executable(calc_pi calc_pi.cpp StackVM.cpp)
target_compile_definitions(calc_pi PUBLIC DOCTEST_CONFIG_DISABLE)
target_compile_features(calc_pi PRIVATE cxx_std_20)

add_executable(test test.cpp StackVM.cpp)
target_compile_features(test PRIVATE cxx_std_20)
