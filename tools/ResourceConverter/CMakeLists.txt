FIND_PACKAGE(DevIL COMPONENTS IL REQUIRED)
FIND_PACKAGE(squish REQUIRED)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include/OpenSR
    ${IL_INCLUDE_DIR}
    ${SQUISH_INCLUDE_DIR}
)

SET(RESOURCE_CONVERTER_SRCS
    main.cpp
    pkg2rpkg.cpp
    gai.cpp
    gi.cpp
    utils.cpp
    planet.cpp
)

ADD_EXECUTABLE(opensr-resourceconverter ${RESOURCE_CONVERTER_SRCS})
TARGET_LINK_LIBRARIES(opensr-resourceconverter ranger ${SQUISH_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

INSTALL(TARGETS opensr-resourceconverter RUNTIME DESTINATION bin)
