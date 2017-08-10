FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)

INCLUDE_DIRECTORIES(
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
)

SET(PV_QML_SOURCES
    PlanetViewer.qml
)

SET(PV_JS_SOURCES
    PlanetViewer.js
)

QT5_ADD_RESOURCES(PV_RESOURCES ${CMAKE_SOURCE_DIR}/QML/qml.qrc PlanetViewer.qrc)

ADD_EXECUTABLE(opensr-planet-viewer main.cpp ${PV_RESOURCES})
TARGET_LINK_LIBRARIES(opensr-planet-viewer engine)
