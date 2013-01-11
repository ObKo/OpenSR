# Try to find Libintl functionality
# Once done this will define
#
#  LIBINTL_FOUND - system has Libintl
#  LIBINTL_INCLUDE_DIR - Libintl include directory
#  LIBINTL_LIBRARIES - Libraries needed to use Libintl
#
# TODO: This will enable translations only if Gettext functionality is
# present in libc. Must have more robust system for release, where Gettext
# functionality can also reside in standalone Gettext library, or the one
# embedded within kdelibs (cf. gettext.m4 from Gettext source).
 
# Copyright (c) 2006, Chusslove Illich, <caslav.ilic@gmx.net>
# Copyright (c) 2007, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 
IF(LIBINTL_INCLUDE_DIR AND LIBINTL_LIB_FOUND)
  SET(Libintl_FIND_QUIETLY TRUE)
ENDIF(LIBINTL_INCLUDE_DIR AND LIBINTL_LIB_FOUND)
 
FIND_PATH(LIBINTL_INCLUDE_DIR libintl.h)
 
SET(LIBINTL_LIB_FOUND FALSE)
 
IF(LIBINTL_INCLUDE_DIR)
  INCLUDE(CheckFunctionExists)
  CHECK_FUNCTION_EXISTS(dgettext LIBINTL_LIBC_HAS_DGETTEXT)
 
  IF (LIBINTL_LIBC_HAS_DGETTEXT)
    SET(LIBINTL_LIBRARIES)
    SET(LIBINTL_LIB_FOUND TRUE)
  ELSE (LIBINTL_LIBC_HAS_DGETTEXT)
    FIND_LIBRARY(LIBINTL_LIBRARIES NAMES intl libintl )
    IF(LIBINTL_LIBRARIES)
      SET(LIBINTL_LIB_FOUND TRUE)
    ENDIF(LIBINTL_LIBRARIES)
  ENDIF (LIBINTL_LIBC_HAS_DGETTEXT)
 
ENDIF(LIBINTL_INCLUDE_DIR)
 
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Libintl DEFAULT_MSG LIBINTL_INCLUDE_DIR LIBINTL_LIB_FOUND)
 
MARK_AS_ADVANCED(LIBINTL_INCLUDE_DIR LIBINTL_LIBRARIES LIBINTL_LIBC_HAS_DGETTEXT LIBINTL_LIB_FOUND)