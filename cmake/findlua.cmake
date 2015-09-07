# Locate lua
#
# This module defines
#  LUA_FOUND, if false, do not try to link to lua
#  LUA_LIBRARY, where to find lua
#  LUA_INCLUDE_DIR, where to find lua.h
#
# By default, the dynamic libraries of lua will be found. To find the static ones instead,
# you must set the LUA_STATIC_LIBRARY variable to TRUE before calling find_package(YamlCpp ...).
#
# If lua is not installed in a standard path, you can use the LUA_DIR CMake variable
# to tell CMake where lua is.

# attempt to find static library first if this is set
if(LUA_STATIC_LIBRARY)
    set(LUA_STATIC liblua.a)
endif()

# find the lua include directory
find_path(LUA_INCLUDE_DIR lua.hpp
          PATH_SUFFIXES include
          PATHS ${LUA_DIR}/include/
)

# find the lua library
find_library(LUA_LIBRARY
             NAMES ${LUA_STATIC} lua luajit
             PATH_SUFFIXES lib64 lib
             PATHS ${LUA_DIR}/lib
)

# handle the QUIETLY and REQUIRED arguments and set LUA_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  LUA DEFAULT_MSG LUA_INCLUDE_DIR LUA_LIBRARY
)

# Copy the results to the output variables.

if (LUA_FOUND)
  set (LUA_LIBRARIES ${LUA_LIBRARY})
  set (LUA_INCLUDE_DIRS ${LUA_INCLUDE_DIR})
else (LUA_FOUND)
  set (LUA_LIBRARIES)
  set (LUA_INCLUDE_DIRS)
endif (LUA_FOUND)

mark_as_advanced(LUA_INCLUDE_DIRS LUA_LIBRARIES)
