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
)

INCLUDE_DIRECTORIES(
	${SDL_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	${ICONV_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/libRanger
	${Boost_INCLUDE_DIRS}
)

IF(WIN32)
	INCLUDE_DIRECTORIES(
		${GLEW_INCLUDE_DIR}
	)
ENDIF(WIN32)

ADD_EXECUTABLE(opensr ${SOURCES} ${HEADERS})
IF(WIN32)
	TARGET_LINK_LIBRARIES(opensr ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${GLEW_LIBRARIES})
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(opensr ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(opensr ranger)