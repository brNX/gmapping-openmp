cmake_minimum_required(VERSION 2.4.6)

include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries

set(ROS_BUILD_TYPE Release)

rosbuild_init()

rosbuild_add_boost_directories()

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#SET (CMAKE_C_COMPILER	"/usr/bin/clang")
#SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")

include_directories(${PROJECT_SOURCE_DIR}/src/gmapping/)

ADD_DEFINITIONS(-DLINUX -ffast-math)

rosbuild_add_executable(bin/slam_gmapping src/slam_gmapping.cpp src/main.cpp ${gmapping_src})
target_link_libraries(bin/slam_gmapping)
rosbuild_link_boost(bin/slam_gmapping signals)
