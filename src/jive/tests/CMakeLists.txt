
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-jive main.cpp ${test_srcs} sample.g)
TARGET_LINK_Y(test-jive jive)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(types     test-jive types)
ADD_TEST(scanner   test-jive scanner ${HERE}/../../main/tests/thousand.db)
ADD_TEST(scatter   test-jive scatter)
ADD_TEST(lexer     test-jive lexer)
ADD_TEST(editor    test-jive editor   ${HERE}/../../main/tests/words.db)
ADD_TEST(editor64  test-jive editor64 ${HERE}/../../main/tests/words.db)
ADD_TEST(ed_snake  test-jive ed_snake)

ADD_TEST(lang:axiom   test-jive lang_axiom)
ADD_TEST(lang:grammar test-jive lang_grammar)

ADD_TEST(axiom     test-jive axiom)
ADD_TEST(grammar   test-jive grammar)
ADD_TEST(perr      test-jive perr)
ADD_TEST(parser    test-jive parser)
ADD_TEST(endl      test-jive endl)
ADD_TEST(json1     test-jive parser ${HERE}/../../json/tests/sample1.js)
ADD_TEST(json2     test-jive parser ${HERE}/../../json/tests/sample2.js)
ADD_TEST(json3     test-jive parser ${HERE}/../../json/tests/sample3.js)
ADD_TEST(json4     test-jive parser ${HERE}/../../json/tests/sample4.js)
ADD_TEST(json5     test-jive parser ${HERE}/../../json/tests/db.js)

ADD_CUSTOM_TARGET(utest-jive
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-jive)

INSTALL( TARGETS test-jive RUNTIME DESTINATION var/y )
