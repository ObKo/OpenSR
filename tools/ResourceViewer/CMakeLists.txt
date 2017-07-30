FIND_PACKAGE(Qt5Widgets REQUIRED)

SET(RV_SOURCES
    main.cpp
    MainWindow.cpp
    FileModel.cpp
    ExtractDialog.cpp
)

SET(RV_HEADERS
    MainWindow.h
    FileModel.h
    ExtractDialog.h
)

SET(RV_UI
    MainWindow.ui
    ExtractDialog.ui
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/include
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
)

QT5_WRAP_CPP(RV_MOC_SOURCES ${RV_HEADERS})
QT5_WRAP_UI(RV_UI_SOURCES ${RV_UI})

ADD_EXECUTABLE(opensr-resourceviewer ${RV_SOURCES} ${RV_UI_SOURCES} ${RV_MOC_SOURCES})

TARGET_LINK_LIBRARIES(opensr-resourceviewer RangerQt Qt5::Core Qt5::Widgets)

INSTALL(TARGETS opensr-resourceviewer RUNTIME DESTINATION bin)
