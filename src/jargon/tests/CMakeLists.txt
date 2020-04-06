
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-jargon main.cpp ${test_srcs})
TARGET_LINK_Y(test-jargon jargon)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(jargon:types    test-jargon jargon_types)
ADD_TEST(jargon:module   test-jargon jargon_module ${HERE}/../../main/tests/words.db)
ADD_TEST(jargon:source   test-jargon jargon_source )
ADD_TEST(jargon:pattern  test-jargon jargon_pattern)
ADD_TEST(jargon:posix    test-jargon jargon_posix)
ADD_TEST(jargon:regexp   test-jargon jargon_regexp)
ADD_TEST(jargon:regcom   test-jargon jargon_regcom)
ADD_TEST(jargon:scanner  test-jargon jargon_scanner)
ADD_TEST(jargon:lexer    test-jargon jargon_lexer)
ADD_TEST(jargon:matching test-jargon jargon_matching)
ADD_TEST(types   test-jargon types)
ADD_TEST(xnode   test-jargon xnode)
ADD_TEST(grammar test-jargon grammar)
ADD_TEST(parser  test-jargon parser)

ADD_CUSTOM_TARGET(utest-jargon
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-jargon)

