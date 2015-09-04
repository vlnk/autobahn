# Locate unittset-cpp
#
# This module defines
#  UNITTESTCPP_FOUND, if false, do not try to link to yaml-cpp
#  UNITTESTCPP_LIBRARY, where to find yaml-cpp
#  UNITTESTCPP_INCLUDE_DIR, where to find yaml.h
#
# By default, the dynamic libraries of yaml-cpp will be found. To find the static ones instead,
# you must set the UNITTESTCPP_STATIC_LIBRARY variable to TRUE before calling find_package(UNITTESTCPP ...).
#
# If unittset-cpp is not installed in a standard path, you can use the UNITTESTCPP_DIR CMake variable
# to tell CMake where yaml-cpp is.

# attempt to find static library first if this is set
if(UNITTESTCPP_STATIC_LIBRARY)
    set(UNITTESTCPP_STATIC libyaml-cpp.a)
endif()

# find the unittset-cpp include directory
find_path(UNITTESTCPP_INCLUDE_DIR yaml-cpp/yaml.h
          PATH_SUFFIXES include
          PATHS ${UNITTESTCPP_DIR}/include/
)

# find the unittset-cpp library
find_library(UNITTESTCPP_LIBRARY
             NAMES ${UNITTESTCPP_STATIC} yaml-cpp
             PATH_SUFFIXES lib64 lib
             PATHS ${UNITTESTCPP_DIR}/lib
)

# handle the QUIETLY and REQUIRED arguments and set UNITTESTCPP_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  UNITTESTCPP DEFAULT_MSG UNITTESTCPP_INCLUDE_DIR UNITTESTCPP_LIBRARY
)

# Copy the results to the output variables.

if (UNITTESTCPP_FOUND)
  set (UNITTESTCPP_LIBRARIES ${UNITTESTCPP_LIBRARY})
  set (UNITTESTCPP_INCLUDE_DIRS ${UNITTESTCPP_INCLUDE_DIR})
else (UNITTESTCPP_FOUND)
  set (UNITTESTCPP_LIBRARIES)
  set (UNITTESTCPP_INCLUDE_DIRS)
endif (UNITTESTCPP_FOUND)

mark_as_advanced(UNITTESTCPP_INCLUDE_DIRS UNITTESTCPP_LIBRARIES)
