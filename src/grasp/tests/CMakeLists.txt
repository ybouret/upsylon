INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.g)

ADD_EXECUTABLE(test-grasp main.cpp ${srcs} ${data})
TARGET_LINK_Y(test-grasp grasp)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

ENABLE_TESTING()

ADD_TEST(Rules   test-grasp rules)
ADD_TEST(Grammar test-grasp grammar)
ADD_TEST(Parser  test-grasp parser)
ADD_TEST(Eval    test-grasp ee)
ADD_TEST(JSON    test-grasp json)
ADD_TEST(Dynamo  test-grasp dyn)

ADD_CUSTOM_TARGET(utest-grasp
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-grasp)
