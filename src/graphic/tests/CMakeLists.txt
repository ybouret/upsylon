PROJECT(test-graphic)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-graphic main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-graphic graphic)

ENABLE_TESTING()
ADD_TEST(Types  test-graphic types)
ADD_TEST(Area   test-graphic area)

ADD_CUSTOM_TARGET(utest-graphic
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-graphic)
