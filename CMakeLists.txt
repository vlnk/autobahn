cmake_minimum_required(VERSION 3.0.2)

include(FindPackageHandleStandardArgs)

set(PROJECT_NAME auto)
project( ${PROJECT_NAME} )

############################################################
#### CONFIGURE CMAKE VARIABLES
############################################################
set (
  CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/"
)

set (
  EXECUTABLE_OUTPUT_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)

############################################################
#### MANAGE PROJECT VERSION
############################################################
set (AUTOBAHN_VERSION_MAJOR 1)
set (AUTOBAHN_VERSION_MINOR 0)
set (AUTOBAHN_VERSION_PATCH 0)
set (AUTOBAHN_VERSION
  "${AUTOBAHN_VERSION_MAJOR}
    .${AUTOBAHN_VERSION_MINOR}
    .${AUTOBAHN_VERSION_PATCH}"
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
add_definitions( -DPROJECT_NAME="${PROJECT_NAME}" )
add_definitions( -DCONFIG_DIR="/Users/vlnk/.autobahn" )
add_definitions( -DSCRIPTS_DIR="/Users/vlnk/Scripts" )
add_definitions( -DLANGUAGES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/languages" )

############################################################
#### SET CONFIGURATION VARS
############################################################
set( CMAKE_VERBOSE_MAKEFILE off )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

############################################################
#### ADD SUBDIRECTORIES
############################################################
add_subdirectory( src )

include(CTest)
add_subdirectory( tests )
