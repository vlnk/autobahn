cmake_minimum_required(VERSION 2.8.4)
project(autoban)

############################################################
#### CONFIGURE CMAKE VARIABLES
############################################################
set (
  CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
)

############################################################
#### MANAGE PROJECT VERSION
############################################################
set (AUTOBAN_VERSION_MAJOR 1)
set (AUTOBAN_VERSION_MINOR 0)
set (AUTOBAN_VERSION_PATCH 0)
set (AUTOBAN_VERSION
  "${AUTOBAN_VERSION_MAJOR}
    .${AUTOBAN_VERSION_MINOR}
    .${AUTOBAN_VERSION_PATCH}"
)

############################################################
#### TURN ON USING SOLUTION FOLDERS
############################################################
set_property (
  GLOBAL PROPERTY
  USE_FOLDERS ON
)

############################################################
#### SET BUILD VARIABLES
############################################################
#add_definitions( -DCONFIG_DIR="/Users/vlnk/.autoban" )
#add_definitions( -DSCRIPTS_DIR="/Users/vlnk/Scripts" )

############################################################
#### SET CONFIGURATION VARS
############################################################
set( CMAKE_VERBOSE_MAKEFILE off )
set ( EXECUTABLE_OUTPUT_PATH ../bin )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

############################################################
#### FIND BOOST INCLUDES
find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
  message(STATUS "Found Boost includes: ${Boost_VERSION}")
else()
  message(FATAL_ERROR "Could not find Boost includes!")
endif()

find_package( yaml-cpp )
find_package( unittest-cpp )

############################################################
#### INCLUDE EXTERNAL DIRECTORIES (BOOST, YAML-CPP, UNITTEST-CPP)
include_directories( ${Boost_INCLUDE_DIRS} )

############################################################
#### GET PROJECT SOURCES
file (
  GLOB_RECURSE
  SOURCE_FILES
  src/*
)

############################################################
#### ADD EXECUTABLE
add_executable ( autoban ${SOURCE_FILES} )

############################################################
#### LINK LIBRARIES
TARGET_LINK_LIBRARIES( autoban ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( autoban ${YAMLCPP_LIBRARY} )
TARGET_LINK_LIBRARIES( autoban ${UNITTESTCPP_LIBRARY} )
