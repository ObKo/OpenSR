ADD_SUBDIRECTORY(world)

SET(SOURCES
    Engine.cpp
    Node.cpp
    Object.cpp
    Sprite.cpp
    Types.cpp
    Texture.cpp
    Label.cpp
    Font.cpp
    AnimatedTexture.cpp
    AnimatedSprite.cpp
    Widget.cpp
    Utils.cpp
    ConsoleWidget.cpp
    LineEditWidget.cpp
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
    JSONHelper.cpp
    Styles.cpp
    SoundManager.cpp
    Sound.cpp
    Plugin.cpp
    Shader.cpp
    ShaderProgram.cpp
    SpriteWidget.cpp
    PythonBindings.cpp
    ScriptWidget.cpp
)

SET(HEADERS
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Action.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Button.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Engine.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/global.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Log.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Node.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ResourceAdapter.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Sound.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Texture.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/WidgetNode.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ActionListener.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/CheckBox.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Font.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/JSONHelper.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Object.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ResourceManager.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/SoundManager.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Types.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/AnimatedSprite.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ColorLabel.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/FSAdapter.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Label.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Plugin.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/RPKGAdapter.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Sprite.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Utils.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/AnimatedTexture.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ConsoleWidget.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/GAISprite.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/LineEditWidget.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/NinePatch.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/PluginInterface.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ScrollArea.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Styles.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/PythonBindings.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Widget.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ScriptWidget.h
    ${CMAKE_BINARY_DIR}/include/OpenSR/version.h
    ${CMAKE_BINARY_DIR}/include/OpenSR/config.h
)

SET(PRIVATE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/AnimatedSprite_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/CheckBox_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/ConsoleWidget_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/GAISprite_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/LineEditWidget_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/NinePatch_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Object_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Sprite_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Widget_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Button_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/ColorLabel_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Engine_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Label_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/Node_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/ScrollArea_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/ScriptWidget_p.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/private/WidgetNode_p.h
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
  IF(MSVC)
    ADD_DEFINITIONS(-DNOMINMAX)
    ADD_DEFINITIONS(-wd4251)
  ENDIF(MSVC)
  INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
ENDIF(WIN32)

ADD_LIBRARY(engine SHARED ${SOURCES})
SET_TARGET_PROPERTIES(engine PROPERTIES VERSION "${MAJOR_VERSION}.${MINOR_VERSION}" COMPILE_FLAGS "-DRANGERS_ENGINE_BUILD")
TARGET_LINK_LIBRARIES(engine ranger)

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
INSTALL(FILES ${HEADERS} DESTINATION include/OpenSR)
INSTALL(FILES ${PRIVATE_HEADERS} DESTINATION include/OpenSR/private)
