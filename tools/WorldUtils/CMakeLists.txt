INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src/world
)

ADD_EXECUTABLE(opensr-worldid WorldID.cpp)
TARGET_LINK_LIBRARIES(opensr-worldid world)

ADD_EXECUTABLE(opensr-worldcase WorldCase.cpp)
TARGET_LINK_LIBRARIES(opensr-worldcase world)

INSTALL(TARGETS opensr-worldid RUNTIME DESTINATION bin)
INSTALL(TARGETS opensr-worldcase RUNTIME DESTINATION bin)