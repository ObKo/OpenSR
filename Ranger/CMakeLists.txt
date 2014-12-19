SET(RANGER_QT_SOURCES
    GAILoader.cpp
    GILoader.cpp
    HAILoader.cpp
    PKG.cpp
    ZLib.cpp
)

SET(RANGER_QT_HEADERS
    ${CMAKE_SOURCE_DIR}/include/OpenSR/libRangerQt.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
)

ADD_LIBRARY(RangerQt SHARED ${RANGER_QT_SOURCES})

TARGET_LINK_LIBRARIES(RangerQt Qt5::Core Qt5::Gui)
