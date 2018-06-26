FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-main main.cpp ${srcs})
TARGET_LINK_Y(test-main)

ENABLE_TESTING()
ADD_TEST(Platform test-main platform)
ADD_TEST(Types    test-main types)
ADD_TEST(Lockable test-main lockable)

ADD_CUSTOM_TARGET(utest-main
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-main)
