FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-main main.cpp ${srcs})
TARGET_LINK_Y(test-main)

ENABLE_TESTING()
ADD_TEST(Platform test-main platform)
ADD_TEST(Types    test-main types)
ADD_TEST(Lockable test-main lockable)
ADD_TEST(Swap     test-main swap)
ADD_TEST(AtExit   test-main at_exit)
ADD_TEST(Sys      test-main sys)
ADD_TEST(Code     test-main code)
ADD_TEST(Bits     test-main bits)
ADD_TEST(Chunk    test-main chunk)
ADD_TEST(Alloc    test-main alloc)
ADD_TEST(Alloc    test-main core)

ADD_CUSTOM_TARGET(utest-main
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-main)
