FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)

SET(SOURCES
    Engine.cpp
    SoundManager.cpp
    Sound.cpp
    GAIAnimatedImage.cpp
    GAITexture.cpp
    ResourceManager.cpp
    QMLQuestPlayer.cpp
    Music.cpp
    MPG123MusicDecoder.cpp
    VorbisMusicDecoder.cpp
    TexturedPolyline.cpp
    TexturedBezierCurve.cpp
    Types.cpp
    QMLHelper.cpp
    DATTranslator.cpp
    SpaceMouseArea.cpp
    PlanetDrawer.cpp
)

SET(HEADERS 
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Engine.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/SoundManager.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Sound.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/GAIAnimatedImage.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/GAITexture.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ResourceManager.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/QMLQuestPlayer.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Music.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/MusicDecoder.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/TexturedPolyline.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/TexturedBezierCurve.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Types.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/QMLHelper.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/PlanetDrawer.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/DATTranslator.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/SpaceMouseArea.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/PlanetDrawer.h
)

SET(PRIVATE_HEADERS 
    ResourceManager_p.h
    SoundManager_p.h
    MPG123MusicDecoder.h
    VorbisMusicDecoder.h
)

INCLUDE_DIRECTORIES(
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
)

QT5_WRAP_CPP(MOC_SOURCES ${HEADERS} ${PRIVATE_HEADERS})
QT5_ADD_RESOURCES(QRC_QML_SOURCES ${CMAKE_SOURCE_DIR}/QML/qml.qrc)

ADD_LIBRARY(engine SHARED ${SOURCES} ${MOC_SOURCES})
TARGET_LINK_LIBRARIES(engine RangerQt QM Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick ${OPENAL_LIBRARY})
SET_TARGET_PROPERTIES(engine PROPERTIES COMPILE_DEFINITIONS "OPENSR_ENGINE_BUILD")

IF(WIN32)
    SET_TARGET_PROPERTIES(engine PROPERTIES PREFIX "")
ENDIF()

ADD_EXECUTABLE(opensr main.cpp ${QRC_QML_SOURCES})
TARGET_LINK_LIBRARIES(opensr engine)

INSTALL(TARGETS opensr RUNTIME DESTINATION bin)
