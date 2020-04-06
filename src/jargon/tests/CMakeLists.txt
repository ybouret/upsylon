
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-jargon main.cpp ${test_srcs})
TARGET_LINK_Y(test-jargon jargon)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(types    test-jargon types)
ADD_TEST(module   test-jargon module ${HERE}/../../main/tests/words.db)
ADD_TEST(source   test-jargon source )
ADD_TEST(pattern  test-jargon pattern)
ADD_TEST(posix    test-jargon posix)
ADD_TEST(regexp   test-jargon regexp)
ADD_TEST(regcom   test-jargon regcom)
ADD_TEST(scanner  test-jargon scanner)
ADD_TEST(lexer    test-jargon lexer)
ADD_TEST(matching test-jargon matching)
ADD_TEST(xnode    test-jargon xnode)
ADD_TEST(grammar  test-jargon grammar)
ADD_TEST(parser   test-jargon parser)
ADD_TEST(expr     test-jargon expr)

ADD_CUSTOM_TARGET(utest-jargon
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-jargon)

