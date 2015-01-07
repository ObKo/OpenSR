FIND_PACKAGE(Qt5Widgets REQUIRED)

SET(QP_SOURCES
    main.cpp
    PlayerWindow.cpp
)

SET(QP_HEADERS
    PlayerWindow.h
)

SET(QP_UI
    PlayerWindow.ui
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/include
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
)

QT5_WRAP_UI(QP_UI_SOURCES ${QP_UI})
QT5_WRAP_CPP(QP_MOC_SOURCES ${QP_HEADERS})

ADD_EXECUTABLE(opensr-questplayer ${QP_SOURCES} ${QP_UI_SOURCES} ${QP_MOC_SOURCES})
TARGET_LINK_LIBRARIES(opensr-questplayer QM Qt5::Core Qt5::Widgets)
