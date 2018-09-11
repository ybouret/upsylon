INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang main.cpp ${srcs})
TARGET_LINK_Y(test-lang lang)

ENABLE_TESTING()
ADD_TEST(Token   test-lang token)
ADD_TEST(Source  test-lang source NULL)
ADD_TEST(Pattern test-lang pattern)
ADD_TEST(Posix   test-lang posix)
ADD_TEST(Regex   test-lang regex)
ADD_TEST(Scanner test-lang scanner NULL)


ADD_CUSTOM_TARGET(utest-lang
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-lang)
