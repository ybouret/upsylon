PROJECT(test-gfx)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-gfx gfx)

 
ENABLE_TESTING()
ADD_TEST(types test-gfx types)
ADD_TEST(area  test-gfx area)
ADD_TEST(stretch test-gfx stretch)
 

ADD_CUSTOM_TARGET(utest-gfx
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-gfx)

INSTALL( TARGETS test-gfx RUNTIME DESTINATION var/y )
