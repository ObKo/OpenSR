INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include/OpenSR/World
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

ADD_EXECUTABLE(opensr-worldid WorldID.cpp)
TARGET_LINK_LIBRARIES(opensr-worldid world)

ADD_EXECUTABLE(opensr-worldcase WorldCase.cpp)
TARGET_LINK_LIBRARIES(opensr-worldcase world)

INSTALL(TARGETS opensr-worldid RUNTIME DESTINATION bin)
INSTALL(TARGETS opensr-worldcase RUNTIME DESTINATION bin)