set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

############################################################
#### FIND BOOST INCLUDES
find_package(Boost 1.58.0 COMPONENTS unit_test_framework REQUIRED)

############################################################
#### INCLUDE EXTERNAL DIRECTORIES (BOOST)
include_directories(${Boost_INCLUDE_DIRS})

############################################################
#### GET PROJECT TEST SOURCES
file (
  GLOB_RECURSE
  TEST_SOURCE_FILES
  ./*
)

############################################################
#### ADD EXECUTABLE
add_executable ( ${PROJECT_TEST_NAME} ${TEST_SOURCE_FILES} )

############################################################
#### LINK LIBRARIES
target_link_libraries(${PROJECT_TEST_NAME} ${Boost_LIBRARIES})

############################################################
#### ADD TESTS
add_test(${PROJECT_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_TEST_NAME})
