INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/libRanger
)

ADD_EXECUTABLE(librangerTextUtilsTest TextUtilsTest.cpp)
TARGET_LINK_LIBRARIES(librangerTextUtilsTest ranger)
ADD_TEST(librangerTextUtilsTest librangerTextUtilsTest)

