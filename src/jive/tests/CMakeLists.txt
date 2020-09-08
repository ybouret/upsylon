
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-jive main.cpp ${test_srcs})
TARGET_LINK_Y(test-jive jive)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(scanner test-jive scanner ${HERE}/../../main/tests/words.db)
ADD_TEST(scatter test-jive scatter)
ADD_TEST(lexer   test-jive lexer)
ADD_TEST(editor  test-jive editor ${HERE}/../../main/tests/words.db)

ADD_CUSTOM_TARGET(utest-jive
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-jive)

INSTALL( TARGETS test-jive RUNTIME DESTINATION var/y )
