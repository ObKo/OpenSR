FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

SET(CMAKE_AUTOMOC ON)

SET(QIFP_SOURCES
    GAIAnimationIO.cpp
    GAILoader.cpp
    GIImageIO.cpp
    GILoader.cpp
    HAIAnimationIO.cpp
    HAILoader.cpp
    QtImageFormatsPlugin.cpp

)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/include/OpenSR
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
)

ADD_LIBRARY(qopensr SHARED ${QIFP_SOURCES})

TARGET_LINK_LIBRARIES(qopensr Qt5::Core Qt5::Gui)