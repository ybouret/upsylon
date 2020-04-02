
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-jargon main.cpp ${test_srcs})
TARGET_LINK_Y(test-jargon jargon)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(types   test-jargon types)
ADD_TEST(xnode   test-jargon xnode)
ADD_TEST(grammar test-jargon grammar)
ADD_TEST(parser  test-jargon  parser)

ADD_CUSTOM_TARGET(utest-jargon
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-jargon)

