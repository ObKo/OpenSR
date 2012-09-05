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
    LuaBindings.cpp
    LuaWidget.cpp
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
)

SET(HEADERS 
    Engine.h
    Node.h
    Object.h
    Sprite.h
    Types.h
    Texture.h
    Label.h
    Font.h
    AnimatedTexture.h
    AnimatedSprite.h
    Widget.h
    Utils.h
    ConsoleWidget.h
    LineEditWidget.h
    Log.h
    ColorLabel.h
    ResourceAdapter.h
    RPKGAdapter.h
    ResourceManager.h
    FSAdapter.h
    LuaBindings.h
    LuaWidget.h
    GAISprite.h
    Button.h
    Action.h
    ActionListener.h
    WidgetNode.h
    CheckBox.h
    NinePatch.h
    ScrollArea.h
    JSONHelper.h
    Styles.h
    SoundManager.h
    Sound.h
)

INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/libRanger
    ${Boost_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
	${LIBINTL_INCLUDE_DIR}
)

CONFIGURE_FILE(version.h.in version.h @ONLY)

IF(WIN32)
    ADD_DEFINITIONS(-DNOMINMAX)
    INCLUDE_DIRECTORIES(
        ${GLEW_INCLUDE_DIR}
    )
ENDIF(WIN32)

ADD_LIBRARY(engine SHARED ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(engine ranger)

IF(WIN32)
TARGET_LINK_LIBRARIES(engine 
    ${SDL_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${FREETYPE_LIBRARIES} 
    ${LUA_LIBRARIES}
    ${Boost_LIBRARIES} 
    ${GLEW_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${LUABIND_LIBRARY}
    ${SDLMIXER_LIBRARY}
	${LIBINTL_LIBRARIES}
)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(engine
    ${SDL_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${FREETYPE_LIBRARIES} 
    ${LUA_LIBRARIES}
    ${Boost_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${LUABIND_LIBRARY}
    ${SDLMIXER_LIBRARY}
	${LIBINTL_LIBRARIES}
    dl
)
ENDIF(WIN32)

ADD_EXECUTABLE(opensr main.cpp)
TARGET_LINK_LIBRARIES(opensr engine ranger)