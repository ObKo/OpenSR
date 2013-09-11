SET(WORLD_PYTHON_SOURCES
    Artifact.cpp
    CargoHook.cpp
    DefenceGenerator.cpp
    DesertPlanet.cpp
    Droid.cpp
    Engine.cpp
    Equipment.cpp
    Goods.cpp
    HabitablePlanet.cpp
    Hull.cpp
    InteractionContext.cpp
    Item.cpp
    LandContext.cpp
    Asteroid.cpp
    Micromodulus.cpp
    Planet.cpp
    PlanetManager.cpp
    Radar.cpp
    Scanner.cpp
    Ship.cpp
    ShipContext.cpp
    PlanetarySystem.cpp
    SpaceBase.cpp
    InfoWidget.cpp
    SpaceObject.cpp
    SystemObject.cpp
    SystemPlanetWidget.cpp
    SystemWidget.cpp
    Tank.cpp
    Types.cpp
    Weapon.cpp
    World.cpp
    WorldGenHook.cpp
    WorldManager.cpp
    WorldObject.cpp
    SkinManager.cpp
    RaceManager.cpp
    SystemManager.cpp
    StyleManager.cpp
    SpaceObjectWidget.cpp
)

SET(WORLD_PYTHON_HEADERS 
    WorldObjectWrap.h
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

ADD_LIBRARY(_World SHARED ${WORLD_PYTHON_SOURCES})
TARGET_LINK_LIBRARIES(_World world ${Boost_PYTHON3_LIBRARY} ${PYTHON_LIBRARIES})

SET_TARGET_PROPERTIES(_World PROPERTIES PREFIX "")
