INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-m main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-m m)

ENABLE_TESTING()
ADD_TEST(Types test-m types)
ADD_TEST(TAO   test-m tao)
ADD_TEST(TAO   test-m tao2)
ADD_TEST(LU    test-m LU)

ADD_CUSTOM_TARGET(utest-m
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-m)
