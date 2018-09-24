
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-json main.cpp ${test_srcs})
TARGET_LINK_Y(test-json json)

