
INCLUDE_DIRECTORIES(../../math ../../main/tests)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs})
TARGET_LINK_Y(test-spade spade)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(types     test-spade types)
ADD_TEST(layout    test-spade layout)
ADD_TEST(partition test-spade partition)
ADD_TEST(field     test-spade field)
ADD_TEST(topology  test-spade topology)
ADD_TEST(layouts   test-spade layouts)
ADD_TEST(polint    test-spade polint)
ADD_TEST(ghosts    test-spade ghosts)

ADD_CUSTOM_TARGET(utest-spade
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-spade)

