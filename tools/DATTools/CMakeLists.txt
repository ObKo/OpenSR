INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

SET(DAT_CONVERTER_SRCS
    crypt.cpp
    convert.cpp
    crc32.cpp
)

SET(DAT_JSON_SRCS
    json.cpp
)

ADD_EXECUTABLE(opensr-dat-convert ${DAT_CONVERTER_SRCS})
TARGET_LINK_LIBRARIES(opensr-dat-convert RangerQt)

ADD_EXECUTABLE(opensr-dat-json ${DAT_JSON_SRCS})
TARGET_LINK_LIBRARIES(opensr-dat-json RangerQt Qt5::Core)

INSTALL(TARGETS opensr-dat-convert RUNTIME DESTINATION bin)
INSTALL(TARGETS opensr-dat-json RUNTIME DESTINATION bin)
