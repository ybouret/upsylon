FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-m main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-m)

ENABLE_TESTING()
ADD_TEST(Types test-math types)

ADD_CUSTOM_TARGET(utest-m
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-m)
