INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/libRanger
)

ADD_EXECUTABLE(rangersresconv main.cpp)
TARGET_LINK_LIBRARIES(rangersresconv ranger)