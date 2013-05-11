SET(SOURCES
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
    LineEditWidget.cpp
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
)

INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${LIBINTL_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/include/OpenSR
    ${CMAKE_SOURCE_DIR}/libRanger
)

ADD_LIBRARY(Engine SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(Engine engine boost_python3 ${PYTHON_LIBRARIES})

SET_TARGET_PROPERTIES(Engine PROPERTIES PREFIX "_")