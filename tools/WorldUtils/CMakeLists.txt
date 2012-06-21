INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src/world
)

ADD_EXECUTABLE(worldid WorldID.cpp)
TARGET_LINK_LIBRARIES(worldid world)

ADD_EXECUTABLE(worldcase WorldCase.cpp)
TARGET_LINK_LIBRARIES(worldcase world)
