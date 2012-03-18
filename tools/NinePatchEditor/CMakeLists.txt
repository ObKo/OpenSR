FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})

SET(NE_HEADERS
    BackgroundItem.h
    EditorView.h
    EditorWindow.h
    NinePatchItem.h
    PatchItem.h
)

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
)

QT4_WRAP_CPP(NE_MOC_SOURCES ${NE_HEADERS})
QT4_WRAP_UI(NE_UI_SOURCES ${NE_UI})

ADD_EXECUTABLE(NinePatchEditor ${NE_SOURCES} ${NE_UI_SOURCES} ${NE_MOC_SOURCES} ${NE_HEADERS})
IF(WIN32)
    SET_TARGET_PROPERTIES(NinePatchEditor PROPERTIES COMPILE_FLAGS "/Zc:wchar_t-")
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(NinePatchEditor ${QT_LIBRARIES})
