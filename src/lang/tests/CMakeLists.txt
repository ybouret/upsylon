INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.g)

ADD_EXECUTABLE(test-lang main.cpp ${srcs} ${data} ../../json/y/json/json.g)
TARGET_LINK_Y(test-lang lang)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

ENABLE_TESTING()

ADD_TEST(Scanner  test-lang scanner NULL)
ADD_TEST(Lex      test-lang lex     NULL)
ADD_TEST(Grammar  test-lang grammar)
ADD_TEST(Parser   test-lang parser)
ADD_TEST(Eval     test-lang eval NULL)
ADD_TEST(Dyn      test-lang dyn)
ADD_TEST(Gen:List test-lang gen ${HERE}/list.g)
ADD_TEST(Gen:Eval test-lang gen ${HERE}/eval.g)
ADD_TEST(Gen:JSON test-lang gen ${HERE}/../../json/y/json/json.g)

ADD_CUSTOM_TARGET(utest-lang
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-lang)
