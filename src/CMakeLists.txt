############################################################
#### FIND BOOST INCLUDES
find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
  message(STATUS "Found Boost includes: ${Boost_VERSION}")
else()
  message(FATAL_ERROR "Could not find Boost includes!")
endif()

find_package( yaml )
find_package( lua )

############################################################
#### INCLUDE EXTERNAL DIRECTORIES (BOOST, YAML-CPP, UNITTEST-CPP)
include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${YAML_INCLUDES} )
include_directories( ${LUA_INCLUDE_DIRS} )

############################################################
#### GET PROJECT SOURCES
file (
  GLOB_RECURSE
  SOURCE_FILES
  ./*
)

############################################################
#### ADD EXECUTABLE
add_executable ( ${PROJECT_NAME} ${SOURCE_FILES} )

############################################################
#### LINK LIBRARIES
target_link_libraries( ${PROJECT_NAME}  ${Boost_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME}  ${YAML_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME}  ${LUA_LIBRARY} )

install(
  TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
