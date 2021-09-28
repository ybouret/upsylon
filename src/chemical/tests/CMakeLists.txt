PROJECT(test-chemical)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.lua)

ADD_EXECUTABLE(test-chemical main.cpp ${srcs} ${data})
TARGET_LINK_Y(test-chemical chemical)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
ENABLE_TESTING()
ADD_TEST(types   test-chemical types)
ADD_TEST(lib     test-chemical lib)
ADD_TEST(actors  test-chemical actors)
ADD_TEST(eqs     test-chemical eqs)
ADD_TEST(sys     test-chemical sys)
ADD_TEST(simple  test-chemical lua ${HERE}/simple.lua)
ADD_TEST(hairy   test-chemical lua ${HERE}/hairy.lua)
ADD_TEST(replica test-chemical lua ${HERE}/replica.lua)


ADD_CUSTOM_TARGET(utest-chemical
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-chemical)

INSTALL( TARGETS test-chemical RUNTIME DESTINATION var/y )
