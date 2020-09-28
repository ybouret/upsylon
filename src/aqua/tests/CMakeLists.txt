PROJECT(test-aqua)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-aqua main.cpp ${srcs} )
TARGET_LINK_Y(test-aqua aqua)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
ENABLE_TESTING()
ADD_TEST(species test-aqua species)
ADD_TEST(library test-aqua library)
ADD_TEST(eqs     test-aqua eqs)

ADD_CUSTOM_TARGET(utest-aqua
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-aqua)

INSTALL( TARGETS test-aqua RUNTIME DESTINATION var/y )
