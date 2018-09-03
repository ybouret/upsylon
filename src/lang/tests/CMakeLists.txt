INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-lang main.cpp ${srcs})
TARGET_LINK_Y(test-lang lang)

ENABLE_TESTING()
ADD_TEST(Token test-lang token)


ADD_CUSTOM_TARGET(utest-lang
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-lang)
