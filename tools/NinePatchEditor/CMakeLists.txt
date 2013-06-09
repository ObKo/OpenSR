FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

SET(CMAKE_AUTOMOC ON)

SET(NE_SOURCES
    main.cpp
    BackgroundItem.cpp
    EditorView.cpp
    EditorWindow.cpp
    NinePatchItem.cpp
    PatchItem.cpp
)

SET(NE_UI
    EditorWindow.ui
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
)

SET(NE_COMPILE_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

QT5_WRAP_UI(NE_UI_SOURCES ${NE_UI})

ADD_EXECUTABLE(opensr-ninepatcheditor ${NE_SOURCES} ${NE_UI_SOURCES})

SET_TARGET_PROPERTIES(opensr-ninepatcheditor PROPERTIES COMPILE_FLAGS ${NE_COMPILE_FLAGS})
TARGET_LINK_LIBRARIES(opensr-ninepatcheditor Qt5::Core Qt5::Widgets)

INSTALL(TARGETS opensr-ninepatcheditor RUNTIME DESTINATION bin)