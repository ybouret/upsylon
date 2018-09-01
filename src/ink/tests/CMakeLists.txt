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


ADD_CUSTOM_TARGET(utest-ink
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-ink)
