PROJECT(test-gfx)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-gfx gfx)

ENABLE_TESTING()
add_test(area   test-gfx area)
add_test(bitmap test-gfx bitmap)

ADD_CUSTOM_TARGET(utest-gfx
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-gfx)

INSTALL( TARGETS test-gfx RUNTIME DESTINATION var/y )
