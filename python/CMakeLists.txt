SET(PYTHON_SOURCES
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
)

SET(PYTHON_HEADERS 
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/ObjectWrap.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/SpriteWrap.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/WidgetWrap.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/python/Wrappers.h
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

ADD_LIBRARY(_Engine SHARED ${PYTHON_SOURCES})
TARGET_LINK_LIBRARIES(_Engine engine ${Boost_PYTHON3_LIBRARY} ${PYTHON_LIBRARIES})

SET_TARGET_PROPERTIES(_Engine PROPERTIES PREFIX "")
