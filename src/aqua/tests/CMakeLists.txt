PROJECT(test-aqua)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.lua)

ADD_EXECUTABLE(test-aqua main.cpp ${srcs} ${data})
TARGET_LINK_Y(test-aqua aqua mkl lua++ jive)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
ENABLE_TESTING()
ADD_TEST(species      test-aqua species)
ADD_TEST(library      test-aqua library)
ADD_TEST(eqs          test-aqua eqs)
ADD_TEST(engine       test-aqua engine)
ADD_TEST(boot         test-aqua boot)
ADD_TEST(citric       test-aqua citric)
ADD_TEST(lua:acetic   test-aqua lua ${HERE}/acetic.lua)
ADD_TEST(lua:citric   test-aqua lua ${HERE}/citric.lua)
ADD_TEST(lua:ammoniac test-aqua lua ${HERE}/ammoniac.lua)


ADD_CUSTOM_TARGET(utest-aqua
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-aqua)

INSTALL( TARGETS test-aqua RUNTIME DESTINATION var/y )
