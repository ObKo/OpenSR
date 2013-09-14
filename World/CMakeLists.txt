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
    Tank.cpp
    Types.cpp
    Weapon.cpp
    WorldGenHook.cpp
    WorldHelper.cpp
    WorldObject.cpp
    GameInterface.cpp
    WorldManager.cpp
    SystemWidget.cpp
    SkinManager.cpp
    RaceManager.cpp
    SystemManager.cpp
    StyleManager.cpp
    SpaceObjectWidget.cpp
)

INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${ICONV_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_BINARY_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/OpenSR/World/
    ${CMAKE_BINARY_DIR}/include/OpenSR/World/
    ${CMAKE_SOURCE_DIR}/libRanger
)

ADD_LIBRARY(world SHARED ${WORLD_SOURCES})
TARGET_LINK_LIBRARIES(world engine)
IF(MSVC)
    SET_TARGET_PROPERTIES(world PROPERTIES COMPILE_FLAGS "-DRANGERS_WORLD_API=__declspec(dllexport)")
ELSE(MSVC)
    SET_TARGET_PROPERTIES(world PROPERTIES COMPILE_FLAGS "-DRANGERS_WORLD_API=")
ENDIF(MSVC)

INSTALL(TARGETS world LIBRARY DESTINATION lib/opensr ARCHIVE DESTINATION lib/opensr RUNTIME DESTINATION bin)

