# - Try to find jsoncpp
# Once done this will define 
# 
#  JSONCPP_FOUND - system has jsoncpp
#  JSONCPP_INCLUDE_DIRS - the jsoncpp include directories
#  JSONCPP_LIBRARIES - link these to use jsoncpp
# 

INCLUDE(FindPackageHandleStandardArgs)

# Using features.h, because libjson library also have json/json.h file
FIND_PATH(JSONCPP_INCLUDE_DIR NAMES json/features.h
          PATH_SUFFIXES jsoncpp
)
FIND_LIBRARY(JSONCPP_LIBRARY NAMES jsoncpp)

SET(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
SET(JSONCPP_INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(jsoncpp DEFAULT_MSG JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR)

MARK_AS_ADVANCED(JSONCPP_INCLUDE_DIR JSONCPP_LIBRARY)
