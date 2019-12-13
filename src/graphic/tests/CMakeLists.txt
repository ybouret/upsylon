PROJECT(test-graphic)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-graphic main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-graphic graphic)

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
ADD_TEST(Image    test-graphic image)
ADD_TEST(Ramp     test-graphic ramp)
ADD_TEST(Img2Ramp test-graphic img2ramp)

ADD_CUSTOM_TARGET(utest-graphic
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-graphic)
