FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)

SET(SOURCES
    Artefact.cpp
    Asteroid.cpp
    CargoHook.cpp
    Container.cpp
    DefenceGenerator.cpp
    DesertPlanet.cpp
    Droid.cpp
    Engine.cpp
    Equipment.cpp
    Goods.cpp
    Hull.cpp
    InhabitedPlanet.cpp
    Item.cpp
    MannedObject.cpp
    Micromodulus.cpp
    Planet.cpp
    PlanetarySystem.cpp
    Race.cpp
    Radar.cpp
    Scanner.cpp
    Sector.cpp
    Ship.cpp
    SpaceObject.cpp
    SpaceStation.cpp
    Tank.cpp
    Weapon.cpp
    WorldContext.cpp
    WorldObject.cpp
    WorldPlugin.cpp
    WorldManager.cpp
    WorldBindings.cpp
    ResourceManager.cpp
    Resource.cpp
)

SET(HEADERS 
    Artefact.h
    Asteroid.h
    CargoHook.h
    Container.h
    DefenceGenerator.h
    DesertPlanet.h
    Droid.h
    Engine.h
    Equipment.h
    Goods.h
    Hull.h
    InhabitedPlanet.h
    Item.h
    MannedObject.h
    Micromodulus.h
    Planet.h
    PlanetarySystem.h
    Race.h
    Radar.h
    Scanner.h
    Sector.h
    Ship.h
    SpaceObject.h
    SpaceStation.h
    Tank.h
    Weapon.h
    WorldContext.h
    WorldObject.h
    WorldPlugin.h
    WorldManager.h
    WorldBindings.h
    ResourceManager.h
    Resource.h
)

INCLUDE_DIRECTORIES(
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
)

QT5_WRAP_CPP(MOC_SOURCES ${HEADERS})

ADD_LIBRARY(world SHARED ${SOURCES} ${MOC_SOURCES})
TARGET_LINK_LIBRARIES(world engine)
SET_TARGET_PROPERTIES(world PROPERTIES COMPILE_DEFINITIONS "OPENSR_WORLD_BUILD")

IF(WIN32)
    SET_TARGET_PROPERTIES(world PROPERTIES PREFIX "")
ENDIF()
