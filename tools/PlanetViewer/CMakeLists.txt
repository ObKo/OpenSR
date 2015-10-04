FIND_PACKAGE(Qt5OpenGL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

SET(PV_SOURCES
    main.cpp
    MainWindow.cpp
    PlanetViewer.cpp
    PlanetModel.cpp
)

SET(PV_HEADERS
    MainWindow.h
    PlanetViewer.h
    PlanetModel.h
)

SET(PV_UI
    MainWindow.ui
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/include
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
)

QT5_WRAP_UI(PV_UI_SOURCES ${PV_UI})
QT5_ADD_RESOURCES(PV_RES_SOURCES resources.qrc)

ADD_EXECUTABLE(opensr-planetviewer ${PV_SOURCES} ${PV_UI_SOURCES} ${PV_RES_SOURCES})
TARGET_LINK_LIBRARIES(opensr-planetviewer Qt5::Core Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES})
SET_TARGET_PROPERTIES(opensr-planetviewer PROPERTIES COMPILE_FLAGS -DGL_GLEXT_PROTOTYPES)

INSTALL(TARGETS opensr-planetviewer RUNTIME DESTINATION bin)
