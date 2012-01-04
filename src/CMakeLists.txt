SET(SOURCES
    Engine.cpp
    Node.cpp
    Object.cpp
    Sprite.cpp
    Types.cpp
    main.cpp
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
)

SET(LUA_PACKAGES
    lua/Engine.pkg
    lua/libRanger.pkg
    lua/Object.pkg
    lua/Types.pkg
    lua/ResourceManager.pkg
    lua/Sprite.pkg
    lua/AnimatedSprite.pkg
    lua/LuaBindings.pkg
    lua/Texture.pkg
    lua/AnimatedTexture.pkg
    lua/Widget.pkg
    lua/LuaWidget.pkg
    lua/GAISprite.pkg
    lua/Button.pkg
    lua/Action.pkg
    lua/ActionListener.pkg
    lua/Node.pkg
    lua/WidgetNode.pkg
    lua/CheckBox.pkg
    lua/NinePatch.pkg
    lua/ScrollArea.pkg
)

INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/libRanger
    ${Boost_INCLUDE_DIRS}
    ${TOLUAPP_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

IF(WIN32)
    ADD_DEFINITIONS(-DNOMINMAX)
    INCLUDE_DIRECTORIES(
        ${GLEW_INCLUDE_DIR}
    )
ENDIF(WIN32)

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lua)
FOREACH(PACKAGE ${LUA_PACKAGES})
    SET(LUACPP ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE}.cpp)
    ADD_CUSTOM_COMMAND(OUTPUT ${LUACPP} 
                       COMMAND ${TOLUAPP_BINARY} -o ${LUACPP} ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE} DEPENDS ${PACKAGE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lua)
    LIST(APPEND TOLUACPP_SOURCES ${LUACPP})	
ENDFOREACH(PACKAGE)
#ADD_CUSTOM_TARGET(lua_compile DEPENDS ${TOLUACPP_SOURCES})

ADD_EXECUTABLE(opensr ${SOURCES} ${HEADERS} ${TOLUACPP_SOURCES})
#ADD_DEPENDENCIES(opensr lua_compile)

IF(WIN32)
TARGET_LINK_LIBRARIES(opensr 
    ${SDL_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${FREETYPE_LIBRARIES} 
    ${LUA_LIBRARIES} 
    ${TOLUAPP_LIBRARIES} 
    ${Boost_LIBRARIES} 
    ${GLEW_LIBRARIES}
)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(opensr 
    ${SDL_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${FREETYPE_LIBRARIES} 
    ${LUA_LIBRARIES} 
    ${TOLUAPP_LIBRARIES} 
    ${Boost_LIBRARIES}
)
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(opensr ranger)
