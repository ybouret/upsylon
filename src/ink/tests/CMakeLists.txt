INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-ink main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-ink ink)

ENABLE_TESTING()
ADD_TEST(Bitmap test-ink bitmap)
ADD_TEST(Areas  test-ink areas)

ADD_CUSTOM_TARGET(utest-ink
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-ink)
