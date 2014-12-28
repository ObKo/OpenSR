# - Try to find Secret Rabbit Code resampler (libsamplerate) library
# Once done this will define
#  SRC_FOUND - System has libsamplerate
#  SRC_INCLUDE_DIRS - The LibXml2 include directories
#  SRC_LIBRARIES - The libraries needed to use LibXml2

INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(SRC_INCLUDE_DIR NAMES samplerate.h)
FIND_LIBRARY(SRC_LIBRARY NAMES samplerate)

SET(SRC_LIBRARIES ${SRC_LIBRARY})
SET(SRC_INCLUDE_DIRS ${SRC_INCLUDE_DIR})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SRC DEFAULT_MSG SRC_LIBRARY SRC_INCLUDE_DIR)

MARK_AS_ADVANCED(SRC_INCLUDE_DIR SRC_LIBRARY)
