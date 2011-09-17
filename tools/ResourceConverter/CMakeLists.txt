FIND_PACKAGE(DevIL REQUIRED)
FIND_PACKAGE(squish REQUIRED)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/libRanger
    ${IL_INCLUDE_DIR}
    ${SQUISH_INCLUDE_DIR}
)

SET(RESOURCE_CONVERTER_SRCS
    main.cpp
    pkg2rpkg.cpp
    dds.cpp
    gai.cpp
    gi.cpp
    utils.cpp
)

ADD_EXECUTABLE(rangersresconv ${RESOURCE_CONVERTER_SRCS})
TARGET_LINK_LIBRARIES(rangersresconv ranger ${SQUISH_LIBRARIES} ${IL_LIBRARIES} pthread)