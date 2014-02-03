SET(PYTHON_ENGINE_SOURCES
    PythonBindings.cpp
    Engine.cpp
    Object.cpp
    Types.cpp
    Node.cpp
    Sprite.cpp
    Widget.cpp
    ScriptWidget.cpp
    AnimatedSprite.cpp
    Label.cpp
    NinePatch.cpp
    Button.cpp
    CheckBox.cpp
    LineEdit.cpp
    ScrollArea.cpp
    WidgetNode.cpp
    GAISprite.cpp
    ColorLabel.cpp
    Action.cpp
    ActionListener.cpp
    Font.cpp
    ResourceManager.cpp
    Shader.cpp
    ShaderProgram.cpp
    Sound.cpp
    SoundManager.cpp
    Styles.cpp
    Texture.cpp
    RadioButton.cpp
    RadioButtonGroup.cpp
    TiledLine.cpp
    SpriteWidget.cpp
    TiledPolyline.cpp
    TiledBeizerCurve.cpp
    AnimatedTexture.cpp
    LabelWidget.cpp
    ResourceObjectManager.cpp
    Ranger.cpp
)

SET(PYTHON_ENGINE_HEADERS 
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/ObjectWrap.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/SpriteWrap.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/WidgetWrap.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/Wrappers.h
)

SET(PYTHON_MODULE_ENGINE_SOURCES
    __init__.py
    _Utils.py
)

INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${LIBINTL_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_SOURCE_DIR}/libRanger
)

ADD_LIBRARY(_Engine SHARED ${PYTHON_ENGINE_SOURCES})
TARGET_LINK_LIBRARIES(_Engine engine ${Boost_PYTHON3_LIBRARY} ${PYTHON_LIBRARIES})

IF(WIN32)
    SET_TARGET_PROPERTIES(_Engine PROPERTIES PREFIX "" SUFFIX ".pyd")
ELSE(WIN32)
    SET_TARGET_PROPERTIES(_Engine PROPERTIES PREFIX "")
ENDIF(WIN32)

INSTALL(FILES ${PYTHON_MODULE_ENGINE_SOURCES} DESTINATION ${PYTHON_PACKAGE_PATH}/OpenSR/Engine)
INSTALL(TARGETS _Engine LIBRARY DESTINATION ${PYTHON_PACKAGE_PATH}/OpenSR/Engine RUNTIME DESTINATION ${PYTHON_PACKAGE_PATH}/OpenSR/Engine)
