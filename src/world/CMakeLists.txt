SET(WORLD_SOURCES
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
    Meteor.cpp
    Micromodulus.cpp
    Planet.cpp
    Radar.cpp
    Scanner.cpp
    Ship.cpp
    ShipContext.cpp
    SolarSystem.cpp
    SpaceBase.cpp
    SpaceObject.cpp
    SystemObject.cpp
    Tank.cpp
    Types.cpp
    Weapon.cpp
    WorldHelper.cpp
    WorldObject.cpp
    GameInterface.cpp
)

#SET(WORLD_HEADERS
#    Artifact.h
#    CargoHook.h
#    DefenceGenerator.h
#    DesertPlanet.h
#    Droid.h
#    Engine.h
#    Equipment.h
#    Goods.h
#    HabitablePlanet.h
#    Hull.h
#    InteractionContext.h
#    Item.h
#    LandContext.h
#    Meteor.h
#    Micromodulus.h
#    Planet.h
#    Radar.h
#    Scanner.h
#    Ship.h
#    ShipContext.h
#    SolarSystem.h
#    SpaceBase.h
#    SpaceObject.h
#    SystemObject.h
#    Tank.h
#    Types.h
#    Weapon.h
#    WorldHelper.h
#    WorldObject.h
#)

#CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/include/OpenSR/version.h.in version.h @ONLY)

INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_BINARY_DIR}/include/OpenSR/
    ${CMAKE_SOURCE_DIR}/libRanger
)

ADD_LIBRARY(world SHARED ${WORLD_SOURCES})
TARGET_LINK_LIBRARIES(world engine)
