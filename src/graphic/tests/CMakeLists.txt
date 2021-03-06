PROJECT(test-graphic)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-graphic main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-graphic graphic)

SET(IMG ${CMAKE_CURRENT_SOURCE_DIR}/tux.jpg)
MESSAGE( STATUS "IMG=${IMG}" )

ENABLE_TESTING()
ADD_TEST(Types    test-graphic types)
ADD_TEST(Area     test-graphic area)
ADD_TEST(Bitmap   test-graphic bitmap)
ADD_TEST(Pixmap   test-graphic pixmap)
ADD_TEST(Stack    test-graphic stack)
ADD_TEST(Parallel test-graphic parallel)
ADD_TEST(Gradient test-graphic gradient)
ADD_TEST(PNG      test-graphic png)
ADD_TEST(JPEG     test-graphic jpeg)
ADD_TEST(TIFF     test-graphic tiff)
ADD_TEST(Image    test-graphic image    ${IMG})
ADD_TEST(Ramp     test-graphic ramp)
ADD_TEST(Img2Ramp test-graphic img2ramp ${IMG})
ADD_TEST(ImGrad   test-graphic imgrad   ${IMG})
ADD_TEST(Filter   test-graphic filter   ${IMG})
ADD_TEST(Grads    test-graphic grads    ${IMG})
ADD_TEST(Blobs    test-graphic blobs    ${IMG})
ADD_TEST(3x3      test-graphic 3x3      ${IMG})
ADD_TEST(Blur     test-graphic blur     ${IMG})
ADD_TEST(Edges    test-graphic edges    ${IMG})
ADD_TEST(Channels test-graphic channels ${IMG})
ADD_TEST(Draw     test-graphic draw)

ADD_CUSTOM_TARGET(utest-graphic
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-graphic)

INSTALL( TARGETS test-graphic RUNTIME DESTINATION var/y )
