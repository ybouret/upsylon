PROJECT(test-gfx)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-gfx gfx)

ENABLE_TESTING()
add_test(area    test-gfx area)
add_test(bitmap  test-gfx bitmap)
add_test(pixmaps test-gfx pixmaps)
add_test(image   test-gfx image)
add_test(options test-gfx options)
add_test(stack   test-gfx stack)
add_test(tiles   test-gfx tiles)
add_test(tess    test-gfx tess)
add_test(ops     test-gfx ops)
add_test(filter  test-gfx filter)
add_test(filter  test-gfx filters)
add_test(blobs   test-gfx blobs)
add_test(edges   test-gfx edges)
add_test(blur    test-gfx blur)

ADD_CUSTOM_TARGET(utest-gfx
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-gfx)

INSTALL( TARGETS test-gfx RUNTIME DESTINATION var/y )
