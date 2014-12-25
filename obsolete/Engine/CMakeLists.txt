SET(SOURCES
    Engine.cpp
    Node.cpp
    Object.cpp
    Sprite.cpp
    Types.cpp
    Texture.cpp
    Label.cpp
    LabelWidget.cpp
    Font.cpp
    FTFont.cpp
    AnimatedTexture.cpp
    AnimatedSprite.cpp
    Widget.cpp
    Utils.cpp
    ConsoleWidget.cpp
    LineEdit.cpp
    Log.cpp
    ColorLabel.cpp
    ResourceAdapter.cpp
    RPKGAdapter.cpp
    ResourceManager.cpp 
    FSAdapter.cpp
    GAISprite.cpp
    Button.cpp
    Action.cpp
    WidgetNode.cpp
    CheckBox.cpp
    NinePatch.cpp
    ScrollArea.cpp
    Styles.cpp
    SoundManager.cpp
    Sound.cpp
    Plugin.cpp
    Shader.cpp
    ShaderProgram.cpp
    SpriteWidget.cpp
    PythonBindings.cpp
    ScriptWidget.cpp
    RadioButton.cpp
    RadioButtonGroup.cpp
    TiledLine.cpp
    TiledPolyline.cpp
    TiledBezierCurve.cpp
    ResourceObjectManager.cpp
    AFTFont.cpp
)

INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${LIBINTL_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_BINARY_DIR}/include/
    ${CMAKE_SOURCE_DIR}/libRanger
)

IF(WIN32)
  INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
ENDIF(WIN32)

ADD_LIBRARY(engine SHARED ${SOURCES})
SET_TARGET_PROPERTIES(engine PROPERTIES VERSION "${MAJOR_VERSION}.${MINOR_VERSION}" COMPILE_FLAGS "-DRANGERS_ENGINE_BUILD")
TARGET_LINK_LIBRARIES(engine ranger ${CMAKE_THREAD_LIBS_INIT})

IF(WIN32)
TARGET_LINK_LIBRARIES(engine 
    ${SDL2_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${FREETYPE_LIBRARIES}
    ${Boost_THREAD_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PYTHON3_LIBRARY}
    ${JSONCPP_LIBRARIES}
    ${SDL2_MIXER_LIBRARY}
    ${LIBINTL_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${GLEW_LIBRARIES}
)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(engine
    ${SDL2_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${FREETYPE_LIBRARIES}
    ${Boost_THREAD_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY} 
    ${Boost_PYTHON3_LIBRARY}
    ${JSONCPP_LIBRARIES}
    ${SDL2_MIXER_LIBRARY}
    ${LIBINTL_LIBRARIES}
    ${PYTHON_LIBRARIES}
    dl
)
ENDIF(WIN32)

ADD_EXECUTABLE(opensr main.cpp)
TARGET_LINK_LIBRARIES(opensr engine ranger)

INSTALL(TARGETS opensr RUNTIME DESTINATION bin)
INSTALL(TARGETS engine LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
