INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-ink main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-ink ink)

ENABLE_TESTING()
ADD_TEST(Bitmap  test-ink bitmap)
ADD_TEST(Areas   test-ink areas)
ADD_TEST(Pixmaps test-ink pixmaps)
ADD_TEST(Image   test-ink image)
ADD_TEST(Ops     test-ink ops)
ADD_TEST(Blur    test-ink blur)
ADD_TEST(Hist    test-ink hist)
ADD_TEST(Blob    test-ink blob)
ADD_TEST(Edges   test-ink edges)
ADD_TEST(Mask    test-ink mask)
ADD_TEST(Draw    test-ink draw)
ADD_TEST(Iso     test-ink iso)
ADD_TEST(Kr      test-ink kr)
ADD_TEST(Crop    test-ink crop)
ADD_TEST(Sub     test-ink sub)

ADD_CUSTOM_TARGET(utest-ink
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-ink)
