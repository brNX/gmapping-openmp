cmake_minimum_required(VERSION 2.8.3)
project(gmapping_openmp)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  map_server
  message_filters
  nav_msgs
  rosbag
  rosconsole
  roscpp
  std_msgs
  tf
)

find_package(Boost REQUIRED COMPONENTS signals)

option(WITH_OPENMP      "Build with OpenMP support for multithreading"                  ON)
# add OpenMP support
if (WITH_OPENMP)
    find_package(OpenMP QUIET)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
    if (NOT OPENMP_FOUND)
        set(WITH_OPENMP OFF)
    endif()
endif()


catkin_package(

)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  src/gmapping
)


ADD_DEFINITIONS(-DLINUX -ffast-math)

set (gmapping_src src/gmapping/configfile/configfile.cpp src/gmapping/gridfastslam/gridslamprocessor_tree.cpp src/gmapping/gridfastslam/motionmodel.cpp src/gmapping/gridfastslam/gfsreader.cpp src/gmapping/gridfastslam/gridslamprocessor.cpp src/gmapping/scanmatcher/scanmatcher.cpp src/gmapping/scanmatcher/eig3.cpp src/gmapping/scanmatcher/scanmatcherprocessor.cpp src/gmapping/scanmatcher/smmap.cpp src/gmapping/utils/movement.cpp src/gmapping/utils/stat.cpp src/gmapping/utils/printmemusage.cpp src/gmapping/log/configuration.cpp src/gmapping/log/carmenconfiguration.cpp src/gmapping/log/sensorstream.cpp src/gmapping/log/sensorlog.cpp src/gmapping/sensor/sensor_range/rangereading.cpp src/gmapping/sensor/sensor_range/rangesensor.cpp src/gmapping/sensor/sensor_base/sensor.cpp src/gmapping/sensor/sensor_base/sensorreading.cpp src/gmapping/sensor/sensor_odometry/odometryreading.cpp src/gmapping/sensor/sensor_odometry/odometrysensor.cpp)


## Declare a cpp executable
add_executable(slam_gmapping src/slam_gmapping.cpp src/main.cpp ${gmapping_src})

## Specify libraries to link a library or executable target against
target_link_libraries(slam_gmapping
  ${catkin_LIBRARIES}
  ${boost_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS gmapping_openmp gmapping_openmp_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )
