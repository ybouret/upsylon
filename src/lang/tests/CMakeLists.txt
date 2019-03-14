INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.g)

ADD_EXECUTABLE(test-lang main.cpp ${srcs}
${data}
../../json/y/json/json.g
)
TARGET_LINK_Y(test-lang lang)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

ENABLE_TESTING()

ADD_TEST(Rules         test-lang rules)
ADD_TEST(Grammar       test-lang grammar)
ADD_TEST(Parser        test-lang parser)
ADD_TEST(Eval          test-lang ee)
ADD_TEST(JSON          test-lang json)
ADD_TEST(Dynamo:Hello  test-lang dyn ${HERE}/hello.g)

ADD_CUSTOM_TARGET(utest-lang
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-lang)
