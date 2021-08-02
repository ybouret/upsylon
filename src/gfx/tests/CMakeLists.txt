PROJECT(test-gfx)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-gfx main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-gfx gfx)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
SET( IMG  ${HERE}/logo.png )

ENABLE_TESTING()
add_test(area    test-gfx area)
add_test(bitmap  test-gfx bitmap)
add_test(pixmaps test-gfx pixmaps)
add_test(image   test-gfx image ${IMG})
add_test(options test-gfx options)
add_test(stack   test-gfx stack)
add_test(tiles   test-gfx tiles)
add_test(tess    test-gfx tess)
add_test(ops     test-gfx ops     ${IMG})
add_test(filter  test-gfx filter  ${IMG})
add_test(filters test-gfx filters ${IMG})
add_test(blobs   test-gfx blobs   ${IMG})
add_test(edges   test-gfx edges   ${IMG} 1.2)
add_test(blur    test-gfx blur    2.1 ${IMG})
add_test(patch   test-gfx patch)
add_test(ramp    test-gfx ramp ${IMG})
add_test(mask    test-gfx mask)
add_test(draw    test-gfx draw)
add_test(split   test-gfx split ${IMG})
add_test(eda     test-gfx eda ${IMG} 2.1)


ADD_CUSTOM_TARGET(utest-gfx
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-gfx)

INSTALL( TARGETS test-gfx RUNTIME DESTINATION var/y )
